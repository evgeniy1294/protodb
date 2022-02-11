#include "SerialIOWidgetCreator.h"

QString SerialIOWIdgetCreator::cid() const
{
    return QString("SerialIOWidgetCreator");
}

QString SerialIOWIdgetCreator::name() const
{
    return QString(QObject::tr("SerialIOWidget Creator"));
}

QString SerialIOWIdgetCreator::description() const
{
    return QString(QObject::tr("Creator for serial io widget"));
}

QString SerialIOWIdgetCreator::iconName() const {
    return QString();
}

IOWidget* SerialIOWIdgetCreator::create() const
{
    return new SerialIOWIdget;
}

IOWidget* SerialIOWIdgetCreator::create(const nlohmann::json& json) const
{
    auto widget = new SerialIOWIdget;
    // widget->fromJson(json);

    return widget;
}
