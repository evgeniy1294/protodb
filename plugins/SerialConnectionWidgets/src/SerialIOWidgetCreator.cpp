#include "protodb/SerialIOWidgetCreator.h"
#include "protodb/SerialIOWidget.h"
#include "protodb/SerialControlWidget.h"

using namespace protodb;

QString SerialConnectionWidgetCreator::cid() const
{
    return creatorId();
}

QString SerialConnectionWidgetCreator::name() const
{
    return QString(QObject::tr("SerialIOWidget Creator"));
}

QString SerialConnectionWidgetCreator::description() const
{
    return QString(QObject::tr("Creator for serial io widget"));
}

QString SerialConnectionWidgetCreator::iconName() const {
    return QString();
}

ConnectionAdvanceControlWidget* SerialConnectionWidgetCreator::createAdvanceControlWidget() const
{
    return new SerialControlWidget;
}

ConnectionConfigWidget* SerialConnectionWidgetCreator::createConfigWidget() const
{
    return new SerialConfigWidget;
}

ConnectionConfigWidget* SerialConnectionWidgetCreator::createConfigWidget(const nlohmann::json& json) const
{
    auto widget = new SerialConfigWidget;
    // widget->fromJson(json);

    return widget;
}
