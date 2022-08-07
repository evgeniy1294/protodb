#pragma once

#include <protodb/factories/CreatorAbstract.h>
#include <nlohmann/json.hpp>

class ScriptInterface;

class ScriptInterfaceCreator: public CreatorAbstract
{
public:
    virtual ~ScriptInterfaceCreator() override = default;

    virtual ScriptInterface* create() const = 0;
    virtual ScriptInterface* create(const nlohmann::json&) const = 0;
};

Q_DECLARE_INTERFACE(ScriptInterfaceCreator, "ProtoDb.ScriptInterface")
