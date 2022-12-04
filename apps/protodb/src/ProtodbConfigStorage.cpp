#include "ProtodbConfigStorage.h"

#include <QStandardPaths>

#include <protodb/utils/JsonBaseUtils.h>

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
    }
}

void ProtodbConfigStorage::config(nlohmann::json& cfg) const
{
    cfg = nlohmann::json::object();
        cfg["UserPluginsLocation"] = m_user_plugins_location;
        cfg["SessionsLocation"]    = m_sessons_locaction;
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
    writeToFile(applicationConfigLocation() + "/config.json", cfg);
}


