#include "Configurable.h"

void Configurable::fromJson(const nlohmann::json &json)
{
    Q_UNUSED(json);
}

void Configurable::toJson(nlohmann::json& json) const
{
    Q_UNUSED(json);
}

void Configurable::setDefaultConfig()
{
    nlohmann::json json;

    defaultConfig( json );
    fromJson(json);
}

void Configurable::defaultConfig(nlohmann::json& json) const
{
    Q_UNUSED(json);
}

void Configurable::setName(const QString &name)
{
    Q_UNUSED(name);
}

const QString& Configurable::name() const
{
    static QString dummyName;

    return dummyName;
}

void Configurable::setDescription(const QString &text)
{
    Q_UNUSED(text);
}

const QString& Configurable::description() const
{
    static QString dummyDescription;

    return dummyDescription;
}
