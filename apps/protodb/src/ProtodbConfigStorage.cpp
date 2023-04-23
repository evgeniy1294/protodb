#include "ProtodbConfigStorage.h"

#include <QStandardPaths>

#include <nlohmann/json.hpp>

#include <protodb/utils/JsonBaseUtils.h>
#include <protodb/plugins/PluginManager.h>

using namespace protodb;

ProtodbConfigStorage::ProtodbConfigStorage()
{
    setDefaultConfig();
}

ProtodbConfigStorage& ProtodbConfigStorage::instance()
{
    static auto m_instance = ProtodbConfigStorage();
    return m_instance;
}

QString ProtodbConfigStorage::sessionsLocation() const
{
    return m_sessons_locaction;
}

QString ProtodbConfigStorage::userPluginsLocation() const
{
    return m_user_plugins_location;
}

QMap<QString, bool> ProtodbConfigStorage::lastPluginsState() const
{
    return m_last_plugins_state;
}

QString ProtodbConfigStorage::defaultSessionsLocation()
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/sessions";
}

QString ProtodbConfigStorage::defaultUserPluginsLocation()
{
    return applicationConfigLocation() + "/plugins";
}

QString ProtodbConfigStorage::applicationConfigLocation()
{
    return QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/protodb";
}

void ProtodbConfigStorage::setSessionsLocation(const QString& path)
{
    m_sessons_locaction = path;
}

void ProtodbConfigStorage::setUserPluginsLocation(const QString& path)
{
    m_user_plugins_location = path;
}

void ProtodbConfigStorage::setConfig(const nlohmann::json& cfg)
{
    if (cfg.is_object()) {
        m_user_plugins_location = cfg.value("UserPluginsLocation", defaultUserPluginsLocation());
        m_sessons_locaction = cfg.value("SessionsLocation", defaultSessionsLocation());

        auto plugins = cfg.value("Plugins", nlohmann::json::object());
        for (auto& [key, value]: plugins.items()) {
            m_last_plugins_state[QString::fromStdString(key)] = value;
        }
    }
}

void ProtodbConfigStorage::config(nlohmann::json& cfg) const
{
    cfg = nlohmann::json::object();
        cfg["UserPluginsLocation"] = m_user_plugins_location;
        cfg["SessionsLocation"]    = m_sessons_locaction;

    auto plugins_state = PluginManager::instance().getPluginsState();

    nlohmann::json plugins = nlohmann::json::object();
    for (auto it = plugins_state.constKeyValueBegin(); it != plugins_state.constKeyValueEnd(); ++it) {
        auto name = it->first.toStdString();
        plugins[name] = it->second;
    }
    cfg["Plugins"] = plugins;

    return;
}

void ProtodbConfigStorage::setDefaultConfig()
{
    m_user_plugins_location = defaultUserPluginsLocation();
    m_sessons_locaction = defaultSessionsLocation();
}

void ProtodbConfigStorage::loadConfig()
{
    nlohmann::json cfg;
    readFromFile(applicationConfigLocation() + "/config.json", cfg);

    setConfig(cfg);
}

void ProtodbConfigStorage::saveConfig() const
{
    nlohmann::json cfg; config(cfg);
    writeToFile(applicationConfigLocation() + "/config.json", cfg, 4);
}


