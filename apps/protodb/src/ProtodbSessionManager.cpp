#include "ProtodbSessionManager.h"

#include "MainClass.h"
#include "mainwindow.h"
#include "SequenceModel.h"

#include <nlohmann/json_fwd.hpp>
#include <protodb/utils/JsonUtils.h>

#include <QDebug>
#include <QApplication>

#include <iostream>

using namespace protodb;

MainWindow* getMainWindow() {
    QWidgetList topLevelWidgets = QApplication::topLevelWidgets();

    for (auto it = topLevelWidgets.begin(); it != topLevelWidgets.end(); it++)
    {
        auto wgt = qobject_cast<MainWindow*>(*it);
        if (wgt)
            return wgt;
    }

    return nullptr;
}

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
    nlohmann::json seances;
    bool ok = readFromFile(path_to_folder+"/seances.json", seances);
    if (ok) {
        if (!seances.empty())
            MainClass::instance().setSeanceConfigs(seances);
    }

    auto& main_class = MainClass::instance();
        main_class.stop();

        auto incoming_seq = main_class.incomingSequences();
            // incoming_seq->clear();
        auto outgoing_seq = main_class.outgoingSequences();
            // outgoing_seq->clear();

    // Загрузка последовательностей
    nlohmann::json sequences;
    ok = readFromFile(path_to_folder+"/sequences.json", sequences);

    if (ok) {
        if (sequences["incoming"].is_array()) {
            incoming_seq->fromJson(sequences["incoming"]);
        }

        if (sequences["outgoing"].is_array()) {
            outgoing_seq->fromJson(sequences["outgoing"]);
        }
    }

    // Загрузка состояний окон
    nlohmann::json gui;
    ok = readFromFile(path_to_folder+"/gui.json", gui);
    if (ok) {
        if (!gui.empty()) {
            auto main_window = getMainWindow();
            if (main_window != nullptr) {
                main_window->setState(gui);
            }
        }
    }

    return true;
}

bool ProtodbSessionManager::save_session(const QString& path_to_folder)
{
    auto& main_class = MainClass::instance();
    nlohmann::json seances;
        main_class.seanceConfigs(seances);

    if (! writeToFile(path_to_folder+"/seances.json", seances, 4) ) {
        std::cerr << "Error when write to file: " << (path_to_folder+"/seances.json").toStdString() << std::endl;
        return false;
    }

    nlohmann::json sequences;

    nlohmann::json outgoing;
        main_class.outgoingSequences()->toJson(outgoing);
        sequences["outgoing"] = outgoing;

    nlohmann::json incoming;
        main_class.incomingSequences()->toJson(incoming);
        sequences["incoming"] = incoming;

    if (! writeToFile(path_to_folder+"/sequences.json", sequences, 4) ) {
        std::cerr << "Error when write to file: " << (path_to_folder+"/sequences.json").toStdString() << std::endl;
        return false;
    }

    nlohmann::json gui;
    auto main_window = getMainWindow();
    if (main_window != nullptr) {
        main_window->getState(gui);
    }

    if (! writeToFile(path_to_folder+"/gui.json", gui) ) {
        std::cerr << "Error when write to file: " << (path_to_folder+"/gui.json").toStdString() << std::endl;
        return false;
    }

    return true;
}
