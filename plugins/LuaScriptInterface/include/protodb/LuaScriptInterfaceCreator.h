#pragma once

#include <protodb/creators/ScriptInterfaceCreator.h>

class LuaScriptInterfaceCreator: public QObject, public ScriptInterfaceCreator
{
    Q_OBJECT
    Q_INTERFACES(ScriptInterfaceCreator)
    Q_PLUGIN_METADATA(IID "ProtoDb.LuaScriptInterfaceCreator" FILE "LuaScriptInterfacePluginMetadata.json")

public:
    explicit LuaScriptInterfaceCreator() = default;
    ~LuaScriptInterfaceCreator() override = default;

    static QString creatorId() { return QString("LuaScriptInterfaceCreator"); }
    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    ScriptInterface* create() const override;
    ScriptInterface* create(const nlohmann::json& json) const override;
};
