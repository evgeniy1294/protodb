#include "protodb/SerialIODeviceCreator.h"

#include <protodb/utils/JsonBaseUtils.h>

#include <QSerialPort>

using namespace protodb;

static QSerialPort::DataBits toDataBits(const QString& str) {
    if (str == "5") return QSerialPort::Data5;
    if (str == "6") return QSerialPort::Data6;
    if (str == "7") return QSerialPort::Data7;

    return QSerialPort::Data8;
}

static QSerialPort::Parity toParity(const QString& str) {
    if (str == "Even")  return QSerialPort::EvenParity;
    if (str == "Odd")   return QSerialPort::OddParity;
    if (str == "Space") return QSerialPort::SpaceParity;
    if (str == "Mark")  return QSerialPort::MarkParity;

    return QSerialPort::NoParity;
}

static QSerialPort::StopBits toStopBits(const QString& str) {
    if (str == "2") return QSerialPort::TwoStop;
    return QSerialPort::OneStop;
}

static QSerialPort::FlowControl toFlowControl(const QString& str) {
    if (str == "Software") return QSerialPort::SoftwareControl;
    if (str == "Hardware") return QSerialPort::HardwareControl;

    return QSerialPort::NoFlowControl;
}

QString SerialIODeviceCreator::cid() const
{
    return creatorId();
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
    if (json.is_null())
        return create();

    bool ok = false;
    auto serial_port = new QSerialPort();
        auto port_name = json.value("PortName", QString());
            serial_port->setPortName(port_name);

        auto baudrate = json.value("Baudrate", QString()).toInt(&ok);
            if (ok) serial_port->setBaudRate(baudrate);

        auto data_bits = toDataBits(json.value("DataBits", QString()));
            serial_port->setDataBits(data_bits);

        auto parity = toParity(json.value("Parity", QString()));
            serial_port->setParity(parity);

        auto stop_bits = toStopBits(json.value("StopBits", QString()));
            serial_port->setStopBits(stop_bits);

        auto flow_control = toFlowControl(json.value("FlowControl", QString()));
            serial_port->setFlowControl(flow_control);

    return serial_port;
}
