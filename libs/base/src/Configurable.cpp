#include "configurable/Configurable.h"
#include <nlohmann/json.hpp>

using namespace protodb;

QString Configurable::gcid() const
{
    return QString();
}

void Configurable::setDefaultConfig()
{
    nlohmann::json json;
        defaultConfig( json );
        setConfig(json);
}

void Configurable::setConfig(const nlohmann::json &json)
{
    Q_UNUSED(json);
}

void Configurable::config(nlohmann::json& json) const
{
    Q_UNUSED(json);
}

void Configurable::defaultConfig(nlohmann::json& json) const
{
    Q_UNUSED(json);
}

void Configurable::setDefaultState()
{
    nlohmann::json json;
        defaultState( json );
        setState(json);
}

void Configurable::setState(const nlohmann::json &json)
{
    Q_UNUSED(json);
}

void Configurable::state(nlohmann::json& json) const
{
    Q_UNUSED(json);
}

void Configurable::defaultState(nlohmann::json& json) const
{
    Q_UNUSED(json);
}

bool Configurable::writeConfig( const QString &path, const QString& extension ) const
{
    nlohmann::json json;
        config( json );

    return writeToFile( path + (extension.isEmpty() ? "" : ("." + extension)), json );
}

bool Configurable::readConfig( const QString &path, const QString &extension )
{
    nlohmann::json json;

    bool success = readFromFile( path + (extension.isEmpty() ? "" : ("." + extension)), json );
    if (success) {
        setConfig(json);
    }

    return success;
}

bool Configurable::writeState( const QString &path, const QString &extension ) const
{
    nlohmann::json json;
        state( json );

    return writeToFile( path + (extension.isEmpty() ? "" : ("." + extension)), json );
}

bool Configurable::readState( const QString &path, const QString &extension )
{
    nlohmann::json json;

    bool success = readFromFile( path + (extension.isEmpty() ? "" : ("." + extension)), json );
    if (success) {
        setState(json);
    }

    return success;
}
