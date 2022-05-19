#include "protodb/SessionManager.h"

#include "protodb/utils/JsonBaseUtils.h"

#include <QDir>
#include <QStandardPaths>
#include <QCoreApplication>

SessionManager& SessionManager::instance()
{
    static SessionManager session_manager;
    return session_manager;
}

SessionManager::SessionManager(QObject *parent)
    : QAbstractTableModel(this)
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

    // Load sissions info
    nlohmann::json sessions;
        readFromFile(path + "/sessions.json", sessions);

    if ( sessions.contains("last") ) {
        if ( sessions["last"].is_string() ) {
            m_last_session_name = sessions["last"].get<QString>();
        }
    }

    if ( !sessions.contains("sessions") ) {
        return true;
    }

     if ( !sessions["sessions"].is_array() ) {
         return true;
     }

     // Очистка модели
     m_sessions.clear();

     for (auto& session: sessions["sessions"]) {
         Session s;
         if ( session.contains("description") ) {
             if ( session["description"].is_string() ) {
                s.description = session["description"].get<QString>();
             }
         }

         if ( session.contains("last_changed") ) {
             if ( session["last_changed"].is_string() ) {
                s.last_changed = QDateTime::fromString( session["last_changed"].get<QString>(), "dd.MM.yyyy hh:mm:ss" );
             }
         }

         if ( session.contains("name") ) {
            if ( session["name"].is_string() ) {
                s.name = session["name"].get<QString>();

                // Добавление элементов в модель
                m_sessions.push_back(s);
             }
         }
    }

     return true;
}

int SessionManager::currentSessionId() const
{
    return m_curr_session_id;
}

bool SessionManager::createSession(const QString &name, const QString &description, const QString &origin)
{
    // Если имя не пустое и такая сессия уже существует не создавать новую сессию
    if ( !name.isEmpty() ) {
        if (findSessionByName(name) >= 0) {
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

    return true;
}

bool SessionManager::removeSession(int id)
{
    if ( id >= 0 && id <= m_sessions.count() ) {
        beginRemoveRows(QModelIndex(), id, id);
            Session s = m_sessions.takeAt(id);

            QDir session_dir(m_working_dir_path);
                session_dir.cd(s.name);
                session_dir.removeRecursively();

        endRemoveRows();
    }

    return false;
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
    for ( int i = 0; i < m_sessions.size(); i++ ) {
        if (m_sessions[i].name == name) {
            return i;
        }
    }

    return -1;
}
