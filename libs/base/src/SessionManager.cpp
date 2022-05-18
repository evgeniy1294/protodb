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
    : QObject(this)
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
