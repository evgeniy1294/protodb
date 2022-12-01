#pragma once

#include <protodb/factories/CreatorAbstract.h>
#include <nlohmann/json.hpp>

namespace protodb
{

class ScriptInterface;

class ScriptInterfaceCreator: public CreatorAbstract
{
public:
    virtual ~ScriptInterfaceCreator() override = default;

    virtual ScriptInterface* create() const = 0;
    virtual ScriptInterface* create(const nlohmann::json&) const = 0;
};

} // namespace protodb

Q_DECLARE_INTERFACE(protodb::ScriptInterfaceCreator, "ProtoDb.ScriptInterface")
