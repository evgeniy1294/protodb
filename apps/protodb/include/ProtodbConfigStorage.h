#pragma once

#include <QString>
#include <nlohmann/json.hpp>

namespace protodb {

class ProtodbConfigStorage {
public:
    static ProtodbConfigStorage& instance();

    QString sessionsLocation() const;
    QString userPluginsLocation() const;

    static QString defaultSessionsLocation();
    static QString defaultUserPluginsLocation();
    static QString applicationConfigLocation();

    void setSessionsLocation(const QString& path);
    void setUserPluginsLocation(const QString& path);

    void setConfig(const nlohmann::json& cfg);
    void config(nlohmann::json& cfg) const;
    void setDefaultConfig();

    void loadConfig();
    void saveConfig() const;

private:
    ProtodbConfigStorage();
   ~ProtodbConfigStorage() = default;

private:
    QString m_sessons_locaction;
    QString m_user_plugins_location;
};

} // namespace protodb
