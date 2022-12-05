#include "protodb/sessions/SessionManager.h"

#include "protodb/utils/JsonBaseUtils.h"
#include "protodb/utils/ziputils.h"

#include <zip.h>

#include <QApplication>
#include <QDebug>
#include <QUuid>
#include <QFont>

#include <filesystem>

using namespace protodb;

SessionManager::SessionManager(QObject *parent)
    : QAbstractTableModel(parent)
    , m_user_data(nlohmann::json::object())
{

}

bool SessionManager::setWorkingDirectory(const QString &path)
{
    if ( m_working_dir_path == path ) {
        return true;
    }

    std::filesystem::path working_dir = path.toStdString();
    if (!std::filesystem::exists(working_dir)) {
        std::error_code ec;
        std::filesystem::create_directories(working_dir, ec);

        if (ec) {
            qDebug() << path << QString::fromStdString(ec.message());
            return false;
        }
    }

    m_working_dir_path = path;
    m_sessions.clear();

    nlohmann::json sessions = nlohmann::json::object();
        readFromFile(path + "/sessions.json", sessions);

    nlohmann::json session_list = sessions.value("sessions", nlohmann::json::array());

    for (auto& session: session_list) {
        Session s;

        s.description = session.value("description", QString());
        s.last_changed = QDateTime::fromString( session.value("last_changed", QString()), "dd.MM.yyyy hh:mm:ss" );

        if ( session.contains("name") ) {
           if ( session["name"].is_string() ) {
               s.name = session["name"].get<QString>();

               beginInsertRows(QModelIndex(), rowCount(), rowCount());  // Может быть баг с индексами, проверить
                   m_sessions.push_back(s);
               endInsertRows();
            }
        }
    }

    m_user_data = sessions.value("user_data", nlohmann::json::object());

    auto last_used = sessions.value("last_used", QString("Default"));
    if ( findSessionByName(last_used) < 0) {
        last_used = createSession("Default");
    }

    return loadSession(last_used);
}

bool SessionManager::saveCurrentState()
{
    nlohmann::json json;
        json["last_used"] = m_current_session_name;

    nlohmann::json sessions = nlohmann::json::array();
    for (auto& s: m_sessions) {
        nlohmann::json session;
            session["name"]         = s.name;
            session["last_changed"] = s.last_changed.toString("dd.MM.yyyy hh:mm:ss");
            session["description"]  = s.description;

        sessions.push_back(session);
    }

    json["sessions"]  = sessions;
    json["user_data"] = m_user_data;

    return writeToFile(m_working_dir_path + "/sessions.json", json);
}

bool SessionManager::containsSession(const QString &name) const
{
    if (name.isEmpty())
        return false;

    for ( auto& s: qAsConst(m_sessions) ) {
        if (s.name == name) return true;
    }

    return false;
}

QString SessionManager::createSession(const QString &name, const QString &description, const QString &origin)
{
    Session s;
        s.name = getUniqueSessionName(name);
        s.description  = description;
        s.last_changed = QDateTime::currentDateTime();

    if (s.name.isEmpty()) {
        m_last_error = tr("Can't create session");
        return QString();
    }

    std::filesystem::path session_dir = (m_working_dir_path + "/" + s.name).toStdString();
    std::filesystem::create_directory(session_dir);

    if ( !origin.isEmpty() ) {
        int id = findSessionByName(origin);
        if (id >= 0) {
            std::filesystem::path origin_dir = (m_working_dir_path + "/" + m_sessions.at(id).name).toStdString();

            if ( std::filesystem::exists(origin_dir) ) {
                std::error_code ec;
                std::filesystem::copy(origin_dir, session_dir, std::filesystem::copy_options::recursive, ec);

                if (ec) {
                    std::filesystem::remove_all(session_dir);
                    m_last_error = tr("Can't create session");
                    return QString();
                }
            }
        }
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());  // Может быть баг с индексами, проверить
        m_sessions.push_back(s);
    endInsertRows();

    emit sSessionsAdded({s.name});

    return s.name;
}

bool SessionManager::renameSession(const QString& curr_name, const QString& new_name)
{
    int idx = findSessionByName(curr_name);
    return renameSession(idx, new_name);
}

bool SessionManager::renameSession(int id, const QString& name)
{
    if ( id >= 0 && id < m_sessions.count() ) {
        auto& s = m_sessions[id];

        std::filesystem::path old_p = (m_working_dir_path + "/" + s.name).toStdString();
        std::filesystem::path new_p = (m_working_dir_path + "/" + name).toStdString();

        std::error_code ec;
        std::filesystem::rename(old_p, new_p, ec);

        if (ec) {
            m_last_error = tr("Can't rename session %1").arg(s.name);
            return false;
        }

        s.name = name;
        s.last_changed = QDateTime::currentDateTime();

        emit dataChanged( index(id, 0), index(id, kColumnLastChanged), {Qt::EditRole} );

        return true;
    }

    return false;
}

bool SessionManager::removeSession(const QString &name)
{
    return removeSession( findSessionByName(name) );
}

bool SessionManager::removeSession(int id)
{
    if ( m_sessions.empty() )
        return false;

    if ( id >= 0 && id < m_sessions.count() ) {
            Session s = m_sessions.at(id);

            std::error_code err;
            std::filesystem::path session_dir = (m_working_dir_path + "/" + s.name).toStdString();
                std::filesystem::remove_all(session_dir, err);

            if (err) {
                m_last_error = tr("Can't remove session: ") + QString(err.message().c_str());
            }
            else {
                beginRemoveRows(QModelIndex(), id, id);
                    m_sessions.removeAt(id);
                endRemoveRows();
            }

        emit sSessionsRemoved({s.name});

        return true;
    }

    m_last_error = tr("Wrong session id");
    return false;
}

bool SessionManager::loadSession(const QString &name)
{
    int id = name.isEmpty() ? findSessionByName(m_current_session_name) : findSessionByName(name);
    return loadSession(id);
}

bool SessionManager::loadSession(int id) {
    if ( id >= 0 && id < m_sessions.size() ) {
        auto session_dir_path = QString(m_working_dir_path + "/" + m_sessions.at(id).name);

        if ( load_session(session_dir_path) ) {
            markLoaded(id);
            return true;
        }
    }

    m_last_error = tr("Wrong session id");
    return false;
}

bool SessionManager::saveCurrentSession()
{
    if (m_sessions.empty())
        return true;

    auto session_dir_path = QString(m_working_dir_path + "/" + m_sessions.at(m_curr_session_id).name);
    if ( save_session( session_dir_path ) ) {
        m_sessions[m_curr_session_id].last_changed = QDateTime::currentDateTime();
        return true;
    }

    return false;
}

bool SessionManager::importSession(const QString &path)
{
    std::filesystem::path fp = path.toStdString();

    if ( !std::filesystem::exists(fp) ) {
        m_last_error = tr("Import session failed: archive not exists");
        return false;
    }

    // 1 - проверить, что файл - архив
    if (fp.extension() != ".zip") {
        m_last_error = tr("Import session failed: this file is not supported");
        return false;
    }

    // 2 - распаковать во временную папку
    std::filesystem::path tmp = std::filesystem::temp_directory_path() / QUuid::createUuid().toString().toStdString();
    std::filesystem::create_directory(tmp);

    if ( !unzipToDirectory( fp, tmp) ) {
        m_last_error = tr("Import session failed: unzip failed");
        return false;
    }

    // 3 - есть ли файл meta.json?
    std::filesystem::path meta_path = tmp / "meta.json";
    if (std::filesystem::exists(meta_path)) {
        nlohmann::json meta;
        if ( readFromFile(meta_path.c_str(), meta) ) {
            // 4 - Проверить метаинформацию
            if ( QCoreApplication::applicationName() != meta.value( "application", QString("Unknown") ) ) {
                m_last_error = tr("Import session failed: session is not supported");
                std::filesystem::remove_all(tmp);

                return false;
            }

            if ( !sessionSupported(meta) ) {
                m_last_error = tr("Import session failed: this session is not supported");
                std::filesystem::remove_all(tmp);

                return false;
            }

            if ( !meta.contains("session") ) {
                m_last_error = tr("Import session failed: broken format");
                std::filesystem::remove_all(tmp);

                return false;
            }

            if ( !meta["session"].is_object() ) {
                m_last_error = tr("Import session failed: broken format");
                std::filesystem::remove_all(tmp);

                return false;
            }

            nlohmann::json session = meta["session"];

            // 5 - создать сессию по метаинформации и добавить её в модель
            QString name = session.value("name", QString());
            QString desc = session.value("description", QString());

            Session s;
                s.name = getUniqueSessionName(name);
                s.description  = desc;
                s.last_changed = QDateTime::currentDateTime();

            // 6 - скопировать данные в рабочий каталог
            {
                std::filesystem::path from = tmp / name.toStdString();
                if ( !std::filesystem::exists(from) ) {
                    m_last_error = tr("Import session failed: broken format");
                    std::filesystem::remove_all(tmp);

                    return false;
                }

                std::error_code ec;
                std::filesystem::path to = (m_working_dir_path + "/" + s.name).toStdString();
                std::filesystem::rename(from, to, ec);

                if (ec) {
                    std::filesystem::remove_all(tmp);
                    std::filesystem::remove_all(to);

                    m_last_error = tr("Import session failed: can't copy session data to folder");

                    return false;
                }
            }

            beginInsertRows(QModelIndex(), rowCount(), rowCount());  // Может быть баг с индексами, проверить
                m_sessions.push_back(s);
            endInsertRows();

            emit sSessionsAdded({s.name});
        }
    }
    else {
        std::filesystem::remove_all(tmp);
        m_last_error = tr("Import session failed: session struct is broken");
        return false;
    }

    std::filesystem::remove_all(tmp);

    return true;
}

bool SessionManager::sessionSupported(const nlohmann::json& meta)
{
    return true;
}

bool SessionManager::exportSession(const QString &name, const QString &path)
{
    return exportSession( findSessionByName(name), path );
}

bool SessionManager::exportSession(int id, const QString &path)
{
    if ( m_sessions.empty() ) {
        m_last_error = tr("Export session failed: no session");
        return false;
    }

    if ( id < 0 && id >= m_sessions.count() ) {
        m_last_error = tr("Export session failed: select session");
        return false;
    }

    if (path.isEmpty()) {
        m_last_error = tr("Export session failed: target path is incorrect");
        return false;
    }

    if ( id >= 0 && id < m_sessions.size() ) {
        auto s = m_sessions.at(id);
        nlohmann::json meta;
            nlohmann::json session;
                session["name"]         = s.name;
                session["description"]  = s.description;

            meta["application"] = QCoreApplication::applicationName();
            meta["version"]     = QCoreApplication::applicationVersion();
            meta["session"]     = session;

        std::filesystem::path tmp = std::filesystem::temp_directory_path() / QUuid::createUuid().toString().toStdString();
        std::filesystem::create_directory(tmp);

        if ( std::filesystem::exists(tmp) ) {
            if ( !writeToFile( QString(tmp.c_str()) + "/meta.json", meta) ) {
                return false;
            }

            std::filesystem::path session_dir = (m_working_dir_path + "/" + s.name).toStdString();

            if ( std::filesystem::exists(session_dir) ) {
                std::filesystem::path tmp_session_dir  = tmp / s.name.toStdString();

                std::error_code ec;
                std::filesystem::copy(session_dir, tmp_session_dir, ec);

                if (ec) {
                    return false;
                }
            }

            zipDirectory(tmp, std::filesystem::path(path.toStdString()));
            std::filesystem::remove_all(tmp);
        }

        return true;
    }

    return false;
}

void SessionManager::userData(nlohmann::json &json) const
{
    json = m_user_data;
}

void SessionManager::setUserData(const nlohmann::json& json)
{
    m_user_data = json;
}

void SessionManager::markLoaded(int id)
{
    if ( id >= 0 && id < m_sessions.size() ) {
        m_curr_session_id = id;
        m_current_session_name = m_sessions[id].name;
    }

    return;
}

QString SessionManager::getUniqueSessionName(const QString& name)
{
    QString base_name = name.isEmpty() ? "New session" : name;
    QString generated_name = base_name;

    for (int i = 1; findSessionByName(generated_name) >= 0; i++) {
        generated_name = base_name + QString("(%1)").arg(i);
        if (i > 255) {
            return QString();
        }
    }

    return generated_name;
}

QString SessionManager::lastError() const
{
    return m_last_error;
}

int SessionManager::rowCount(const QModelIndex &parent) const {
    return m_sessions.size();
}

int SessionManager::columnCount(const QModelIndex &parent) const {
    return kColumnCount;
}

QVariant SessionManager::data(const QModelIndex &index, int role) const
{
    auto row = index.row();
    auto col = index.column();


    if (checkIndex(index)) {
        auto& s = m_sessions.at(row);

        if (role == Qt::DisplayRole) {
            switch (col) {
                case kColumnName:
                    return s.name;

                case kColumnLastChanged:
                    return s.last_changed;

                case kColumnDescription: {
                    return s.description;
                }
            }
        }


        if (role == Qt::FontRole) {
            if (index.row() == m_curr_session_id) {
                auto font = QApplication::font();
                    font.setBold(true);
                    font.setItalic(true);
                return font;
            }
        }
    }

    return QVariant();
}

QVariant SessionManager::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case kColumnName:
                    return QString(tr("Name"));

                case kColumnLastChanged:
                    return QString(tr("Changed"));

                case kColumnDescription:
                    return QString(tr("Description"));

                default: break;
            }
        }
        else {
            return QString::number(section+1);
        }

    }

    return QVariant();
}

bool SessionManager::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if ( !index.isValid() ) {
        return false;
    }

    if (role != Qt::EditRole) {
        return false;
    }

    // setData can change description only
    if (index.column() != kColumnDescription) {
        return false;
    }

    auto& s = m_sessions[index.row()];
        s.description = value.toString();

    emit dataChanged( index, index, {Qt::EditRole} );

    return true;
}

int SessionManager::findSessionByName(const QString &name) const
{
    if ( !(name.isEmpty() || m_sessions.empty()) ) {
        for ( int i = 0; i < m_sessions.size(); i++ ) {
            if (m_sessions[i].name == name) {
                return i;
            }
        }
    }

    return -1;
}
