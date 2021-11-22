#pragma once

#include <nlohmann/json.hpp>
#include <QtCore>

class Configurable
{
public:
    virtual void fromJson(nlohmann::json& json);
    virtual void toJson(nlohmann::json& json) const;

    virtual void setName(const QString& name);
    virtual const QString& name() const;

    virtual void setDescription(const QString& text);
    virtual const QString& description() const;

    void setDefaultConfig();
    virtual void defaultConfig(nlohmann::json& json) const; 
};
