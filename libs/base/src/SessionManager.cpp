#include "protodb/SessionManager.h"

#include "protodb/utils/JsonBaseUtils.h"
#include "protodb/utils/ziputils.h"

#include <zip.h>

#include <QDir>
#include <QCoreApplication>
#include <QTemporaryDir>

#include <filesystem>

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

    if ( !QDir(path).exists() ) {
        if ( !QDir().mkdir(path) ) {
            return false;
        }
    }

    m_working_dir_path = path;

    nlohmann::json sessions;
        readFromFile(path + "/sessions.json", sessions);

    if ( !sessions.contains("sessions") ) {
        return true;
    }

    if ( !sessions["sessions"].is_array() ) {
        return true;
    }

    m_current_session_name = sessions.value("current", QString());
    m_user_data            = sessions.value("user_data", nlohmann::json::object());

    m_sessions.clear();
    for (auto& session: sessions["sessions"]) {
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

    return true;
}

bool SessionManager::saveCurrentState()
{
    nlohmann::json json;
        json["current"] = m_current_session_name;

    nlohmann::json sessions = nlohmann::json::array();
    for (auto& s: m_sessions) {
        nlohmann::json session;
            session["name"]         = s.name;
            session["last_changed"] = s.last_changed.toString();
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

bool SessionManager::createSession(const QString &name, const QString &description, const QString &origin)
{
    // Если имя не пустое и такая сессия уже существует не создавать новую сессию
    if ( !name.isEmpty() ) {
        if ( containsSession(name) ) {
            m_last_error = tr("This session already exist");
            return false;
        }
    }

    Session s;
        s.name = name;
        s.description  = description;
        s.last_changed = QDateTime::currentDateTime();

    // Если имя пустое, подобрать значение по умолчанию
    if ( s.name.isEmpty() ) {
        s.name = "new session";

        for (int i = 1; findSessionByName(s.name) >= 0; i++) {
            s.name = QString("new session(%1)").arg(i);
            if (i > 255) {
                m_last_error = tr("Can't create session");
                return false;
            }
        }
    }

    QDir session_dir( m_working_dir_path );
        if ( !session_dir.mkdir(s.name) ) {
            m_last_error = tr("Can't create session");
            return false;
        }
        session_dir.cd(s.name);

    if ( !origin.isEmpty() ) {
        int id = findSessionByName(origin);
        if (id >= 0) {
            QDir origin_dir(m_working_dir_path + "/" + m_sessions.at(id).name);

            if ( origin_dir.exists() ) {
                auto origin_entry = origin_dir.entryInfoList({"*.json"});

                for (auto& file_info: origin_entry) {
                    QFile::copy(file_info.absoluteFilePath(), session_dir.absolutePath() + "/" + file_info.fileName());
                }
            }
        }
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());  // Может быть баг с индексами, проверить
        m_sessions.push_back(s);
    endInsertRows();

    emit sSessionsAdded({s.name});

    return true;
}

bool SessionManager::removeSession(const QString &name)
{
    return removeSession( findSessionByName(name) );
}

bool SessionManager::removeSession(int id)
{
    if ( m_sessions.empty() )
        return false;

    if ( id >= 0 && id <= m_sessions.count() ) {
        beginRemoveRows(QModelIndex(), id, id);
            Session s = m_sessions.takeAt(id);

            QDir session_dir(m_working_dir_path);
                session_dir.cd(s.name);
                session_dir.removeRecursively(); // TODO: проверять результат удаления папки
        endRemoveRows();

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
    if ( id > 0 && id < m_sessions.size() ) {
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
    QFileInfo file_info(path);
    if ( file_info.exists() ) {
        // 1 - проверить, что файл - архив

        // 2 - распаковать во временную папку

        // 3 - есть ли файл meta.json?

        // 4 - скопировать данные в рабочий каталог

        // 5 - создать сессию по метаинформации и добавить её в модель
    }

    m_last_error = tr("Import failed");
    return false;
}

bool SessionManager::exportSession(const QString &name, const QString &path)
{
    return exportSession( findSessionByName(name), path );
}

bool SessionManager::exportSession(int id, const QString &path)
{
    if ( id >= 0 && id < m_sessions.size() ) {
        // 0 - подготовить метаинформацию
        auto s = m_sessions.at(id);
        nlohmann::json meta;
            nlohmann::json session;
                session["name"]         = s.name;
                session["description"]  = s.description;

            meta["application"] = QCoreApplication::applicationName();
            meta["version"]     = QCoreApplication::applicationVersion();
            meta["session"]     = session;

        QTemporaryDir tmp_dir;
        if ( tmp_dir.isValid() ) {
            // 1 - создать во временной директории файл с метаинформацией
            if ( !writeToFile(tmp_dir.path() + "/meta.json", meta) ) {
                return false;
            }

            // 2 - скопировать папку сессии во временную директорию
            std::filesystem::path session_dir = (m_working_dir_path + "/" + s.name).toStdString();

            if ( std::filesystem::exists(session_dir) ) {
                std::filesystem::path tmp_session_dir  = tmp_dir.filePath(s.name).toStdString();

                std::error_code err;
                std::filesystem::copy(session_dir, tmp_session_dir, err);

                if (err.value() != 0) {
                    return false;
                }
            }

            // 3 - создать из временной папки архив по требуемому пути
            zipDirectory(tmp_dir.path(), path);
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
    if ( id > 0 && id < m_sessions.size() ) {
        m_curr_session_id = id;
        m_current_session_name = m_sessions[id].name;
    }

    return;
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
            // TODO: выделить выбранную сессию

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
