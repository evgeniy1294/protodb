#pragma once

#include <QtCore>

class PluginAbstract {
public:
    enum Type {
        DeviceType,
        ToolType,
    };

    struct Version { int major; int minor; };

public:
    virtual Type type() const = 0;

    virtual QString pid() const = 0;
    virtual const QString& name() const = 0;
    virtual const QString& description() const = 0;
    virtual const QString& vendor() const = 0;
    virtual const QStringList& dependencies() const = 0;
    virtual Version version() const = 0;
};

class ConfigFrameCreator;
class IoDeviceCreator;

class DevicePlugin: public PluginAbstract
{
public:
    virtual ConfigFrameCreator* configCreator() = 0;
    virtual IoDeviceCreator* deviceCreator() = 0;

    Type type() const override {
        return DeviceType;
    }
};

class ToolWidgetCreator;

class ToolPlugin: public PluginAbstract
{
public:
    virtual ToolWidgetCreator* toolCreator() = 0;

    Type type() const override {
        return ToolType;
    }
};

