#pragma once

#include <QtCore>
#include "ConfigFrame.h"

class PluginAbstract {
public:
    enum Type {
        IoDeviceType,
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
    virtual bool alwaysEnabled() const = 0;

    bool enabled() const;
    void setEnabled(bool enable);
    void setDisabled(bool disable);

private:
    bool m_enabled = false;
};

/*
class IoPlugin: public PluginAbstract
{
public:
    virtual ConfigFrame* createFrame() = 0;
    virtual QIODevice* createIoDevice() = 0; // TODO: сюда нужно пихать конфиг

    Type type() const override {
        return IoDeviceType;
    }
};

class ToolPlugin: public PluginAbstract
{
public:
    virtual QWidget* createWidget() = 0;

    // Эти действия будут добавлены в контекстное меню логгера для каналов с данными
    // Запихать в ToolWidget
    // virtual const QList<QAction*>& exportableActions() = 0;

    Type type() const override {
        return ToolType;
    }
};
*/

const QString& toString(PluginAbstract::Type type);
QString toString(PluginAbstract::Version version);
