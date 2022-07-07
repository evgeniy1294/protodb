#include "ProtodbSessionManager.h"
#include "MainClass.h"
#include "SequenceModel.h"

#include <protodb/utils/JsonUtils.h>

#include <QDebug>

#include <iostream>

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

    auto& main_class = MainClass::instance();
        main_class.stop();

        auto incoming_seq = main_class.incomingSequences();
            // incoming_seq->clear();
        auto outgoing_seq = main_class.outgoingSequences();
            // outgoing_seq->clear();

    // Загрузка последовательностей
    nlohmann::json sequences;
    bool ok = readFromFile(path_to_folder+"/sequences.json", sequences);

    if (ok) {
        if (sequences["incoming"].is_array()) {
            incoming_seq->fromJson(sequences["incoming"]);
        }

        if (sequences["outgoing"].is_array()) {
            outgoing_seq->fromJson(sequences["outgoing"]);
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
    auto& main_class = MainClass::instance();
    nlohmann::json sequences;

    nlohmann::json outgoing;
        main_class.outgoingSequences()->toJson(outgoing);
        sequences["outgoing"] = outgoing;

    nlohmann::json incoming;
        main_class.incomingSequences()->toJson(incoming);
        sequences["incoming"] = incoming;

    if (! writeToFile(path_to_folder+"/sequences.json", sequences) ) {
        std::cerr << "Error when write to file: " << (path_to_folder+"/sequences.json").toStdString() << std::endl;
        return false;
    }


    return true;
}
