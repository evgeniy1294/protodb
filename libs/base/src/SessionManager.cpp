#include "protodb/SessionManager.h"

SessionManager& SessionManager::instance()
{
    static SessionManager session_manager;
    return session_manager;
}

SessionManager::SessionManager(QObject *parent)
{

}
