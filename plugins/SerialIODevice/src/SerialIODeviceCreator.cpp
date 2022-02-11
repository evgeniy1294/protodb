#include "SerialIODeviceCreator.h"

#include <QSerialPort>

QString SerialIODeviceCreator::cid() const
{
    return QString("SerialIOWidgetCreator");
}

QString SerialIODeviceCreator::name() const
{
    return QString(QObject::tr("SerialIOWidget Creator"));
}

QString SerialIODeviceCreator::description() const
{
    return QString(QObject::tr("Creator for serial io widget"));
}

QString SerialIODeviceCreator::iconName() const {
    return QString();
}

QIODevice* SerialIODeviceCreator::create() const
{
    return new QSerialPort();
}

QIODevice* SerialIODeviceCreator::create(const nlohmann::json& json) const
{
    auto serial_port = new QSerialPort();

    return serial_port;
}
