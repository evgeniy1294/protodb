#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/CreatorAbstract.h>
#include <nlohmann/json.hpp>

namespace protodb
{

class ScriptInterface;

class PROTODB_DEV_EXPORT ScriptInterfaceCreator: public CreatorAbstract
{
public:
    virtual ~ScriptInterfaceCreator() override = default;

    virtual ScriptInterface* create() const = 0;
    virtual ScriptInterface* create(const nlohmann::json&) const = 0;
};

} // namespace protodb

Q_DECLARE_INTERFACE(protodb::ScriptInterfaceCreator, "ProtoDb.ScriptInterface")
