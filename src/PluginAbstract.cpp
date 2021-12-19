#include "PluginAbstract.h"

bool PluginAbstract::enabled() const
{
    return alwaysEnabled() || m_enabled;
}

void PluginAbstract::setEnabled(bool enable)
{
    m_enabled = enable;
}

void PluginAbstract::setDisabled(bool disable)
{
    m_enabled = !disable;
}

const QString &toString(PluginAbstract::Type type)
{
    static const QString IoDevice = "Io Device";
    static const QString Tool = "Tool";
    static const QString Unknown = "Unknown";

    switch(type) {
        case PluginAbstract::IoDeviceType:
            return IoDevice;
        case PluginAbstract::ToolType:
            return Tool;
    }

    return Unknown;
}

QString toString(PluginAbstract::Version version)
{
    return QString("%1.%2").arg(version.major, version.minor);
}
