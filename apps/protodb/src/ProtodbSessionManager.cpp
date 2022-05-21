#include "ProtodbSessionManager.h"

#include <protodb/utils/JsonUtils.h>

#include <QDebug>

ProtodbSessionManager &ProtodbSessionManager::instance()
{
    static ProtodbSessionManager m_instance;
    return m_instance;
}

ProtodbSessionManager::ProtodbSessionManager(QObject *parent)
    : SessionManager(parent)
{

}

bool ProtodbSessionManager::load_session(const QString& path_to_folder)
{
    // Загрузка настроек и параметров
    QFile configs(path_to_folder+"/configs.json");
    if ( configs.exists() ) {

    }

    // Загрузка последовательнгостей
    nlohmann::json sequences;
    bool ok = readFromFile(path_to_folder+"/sequences.json", sequences);

    if (ok) {
        if (sequences["incoming"].is_array()) {
            qDebug() << "Incoming sequences loaded";
        }

        if (sequences["outgoing"].is_array()) {
            qDebug() << "Outgoing sequences loaded";
        }
    }

    // Загрузка состояний окон
    QFile gui(path_to_folder+"/gui.json");
    if ( gui.exists() ) {

    }

    return true;
}

bool ProtodbSessionManager::save_session(const QString& path_to_folder)
{
    nlohmann::json sequences;

    nlohmann::json outgoing = nlohmann::json::array();
        sequences["outgoing"] = outgoing;

    nlohmann::json incoming = nlohmann::json::array();
        sequences["incoming"] = incoming;

    if (! writeToFile(path_to_folder+"/sequences.json", sequences) ) {
        return false;
    }

    qDebug() << "Session saved";

    return true;
}
