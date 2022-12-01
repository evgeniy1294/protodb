#pragma once

#include <protodb/utils/JsonBaseUtils.h>

#include <QString>

namespace protodb {

class Configurable
{
public:
    // Creator ID for widget factory
    virtual QString gcid() const;

    void setDefaultConfig();
    virtual void setConfig(const nlohmann::json& json);
    virtual void config(nlohmann::json& json) const;
    virtual void defaultConfig(nlohmann::json& json) const;

    void setDefaultState();
    virtual void setState(const nlohmann::json& json);
    virtual void state(nlohmann::json& json) const;
    virtual void defaultState(nlohmann::json& json) const;

    virtual bool writeConfig( const QString &path, const QString &extension = "json" ) const;
    virtual bool readConfig( const QString &path, const QString &extension = "json" );

    virtual bool writeState( const QString &path, const QString &extension = "json") const;
    virtual bool readState( const QString &path, const QString &extension = "json");
};

} // namespace protodb
