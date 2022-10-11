#include "NetIOWidgetCreator.h"
#include "NetIOWidget.h"

QString NetIOWIdgetCreator::cid() const
{
    return creatorId();
}

QString NetIOWIdgetCreator::name() const
{
    return QString(QObject::tr("NetIOWidget Creator"));
}

QString NetIOWIdgetCreator::description() const
{
    return QString(QObject::tr("Creator for network io widget"));
}

QString NetIOWIdgetCreator::iconName() const {
    return QString();
}

IOWidget* NetIOWIdgetCreator::create() const
{
    return new NetIOWidget;
}

IOWidget* NetIOWIdgetCreator::create(const nlohmann::json& json) const
{
    auto widget = new NetIOWidget;
    // widget->fromJson(json);

    return widget;
}
