#include "protodb/NetIOWidgetCreator.h"
#include "protodb/NetIOWidget.h"

using namespace protodb;

QString NetworkConnectionWidgetCreator::cid() const
{
    return creatorId();
}

QString NetworkConnectionWidgetCreator::name() const
{
    return QString(QObject::tr("NetIOWidget Creator"));
}

QString NetworkConnectionWidgetCreator::description() const
{
    return QString(QObject::tr("Creator for network io widget"));
}

QString NetworkConnectionWidgetCreator::iconName() const {
    return QString();
}

ConnectionConfigWidget* NetworkConnectionWidgetCreator::createConfigWidget() const
{
    return new NetworkConfigWidget;
}

ConnectionConfigWidget* NetworkConnectionWidgetCreator::createConfigWidget(const nlohmann::json& json) const
{
    auto widget = new NetworkConfigWidget;
    // widget->fromJson(json);

    return widget;
}

ConnectionAdvanceControlWidget* NetworkConnectionWidgetCreator::createAdvanceControlWidget() const
{
    return nullptr;
}
