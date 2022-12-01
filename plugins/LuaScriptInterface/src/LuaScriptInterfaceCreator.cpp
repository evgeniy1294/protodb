#include "protodb/LuaScriptInterfaceCreator.h"

#include "protodb/LuaScriptInterface.h"

using namespace protodb;

QString LuaScriptInterfaceCreator::cid() const
{
    return creatorId();
}

QString LuaScriptInterfaceCreator::name() const
{
    return QString(QObject::tr("Lua Script Creator"));
}

QString LuaScriptInterfaceCreator::description() const
{
    return QString(QObject::tr("Creator for Lua Script interface"));
}

QString LuaScriptInterfaceCreator::iconName() const {
    return QString();
}

ScriptInterface* LuaScriptInterfaceCreator::create() const
{
    return new LuaScriptInterface();
}

ScriptInterface* LuaScriptInterfaceCreator::create(const nlohmann::json& json) const
{
    Q_UNUSED(json)
    auto lua_script_interface = new LuaScriptInterface();

    return lua_script_interface;
}
