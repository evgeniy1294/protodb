#include "protodb/SerialIoSeance.h"

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

static char toShortParityName(QSerialPort::Parity p) {
    if (p == QSerialPort::EvenParity)  return 'E';
    if (p == QSerialPort::OddParity)   return 'O';
    if (p == QSerialPort::SpaceParity) return 'S';
    if (p == QSerialPort::MarkParity)  return 'M';

    return 'N';
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

static QString toString(QSerialPort::SerialPortError err) {
    switch(err) {
        case QSerialPort::NoError: return "None";
        case QSerialPort::DeviceNotFoundError: return "Device not found";
        case QSerialPort::PermissionError: return "Permission error";
        case QSerialPort::OpenError: return "Open error";
        case QSerialPort::WriteError: return "Write error";
        case QSerialPort::ReadError: return "Read error";
        case QSerialPort::ResourceError: return "Resource error";
        case QSerialPort::UnsupportedOperationError: return "Unsupported operation";
        case QSerialPort::TimeoutError: return "Timeout";
        case QSerialPort::NotOpenError: return "Not open";

        default: break;
    }

    return "Unknown error";
}

SerialIoSeance::SerialIoSeance(QObject* parent)
    : Seance(parent)
    , m_serial_port(new QSerialPort(this))
{
    m_description = tr("Serial unknown");
    connectSignals();
}

bool SerialIoSeance::setActive(bool active)
{
    if (active) {
        m_description = QString("%1 %2 %3%4%5");

        bool ok = false;

        auto port_name = m_cfg.value("PortName", QString());
            m_serial_port->setPortName(port_name);
            m_description = m_description.arg(m_serial_port->portName());

        auto baudrate = m_cfg.value("Baudrate", QString()).toInt(&ok);
            if (ok) m_serial_port->setBaudRate(baudrate);
            m_description = m_description.arg(m_serial_port->baudRate());

        auto data_bits = toDataBits(m_cfg.value("DataBits", QString()));
            m_serial_port->setDataBits(data_bits);
            m_description = m_description.arg(m_serial_port->dataBits());

        auto parity = toParity(m_cfg.value("Parity", QString()));
            m_serial_port->setParity(parity);
            m_description = m_description.arg(toShortParityName(m_serial_port->parity()));

        auto stop_bits = toStopBits(m_cfg.value("StopBits", QString()));
            m_serial_port->setStopBits(stop_bits);
            m_description = m_description.arg(m_serial_port->stopBits());

        auto flow_control = toFlowControl(m_cfg.value("FlowControl", QString()));
            m_serial_port->setFlowControl(flow_control);

        if (m_cfg.value("OpenMode", QString()) == "Monitoring") {
            m_active = m_serial_port->open(QIODevice::ReadOnly);
        }
        else {
            m_active = m_serial_port->open(QIODevice::ReadWrite);
        }

        if (!m_active) emit errorOccurred(m_serial_port->errorString());
    }
    else {
        m_serial_port->close();
        m_active = false;
    }

    return m_active;
}

void SerialIoSeance::setDisactive()
{
    m_serial_port->close();
    m_active = false;
}

qint64 SerialIoSeance::write(int channel, const char* data, qint64 maxSize)
{
    if (channel == TxChannel) {
        return m_serial_port->write(data, maxSize);
    }

    return 0;
}

qint64 SerialIoSeance::write(int channel, const char* data)
{
    if (channel == TxChannel) {
        return m_serial_port->write(data);
    }

    return 0;
}

qint64 SerialIoSeance::write(int channel, const QByteArray& byteArray)
{
    if (channel == TxChannel) {
        return m_serial_port->write(byteArray);
    }

    return 0;
}

qint64 SerialIoSeance::read(int channel, char* data, qint64 maxSize)
{
    if (channel == RxChannel) {
        return m_serial_port->read(data, maxSize);
    }

    return 0;
}

QByteArray SerialIoSeance::read(int channel, qint64 maxSize)
{
    if (channel == RxChannel) {
        return m_serial_port->read(maxSize);
    }

    return QByteArray();
}

QByteArray SerialIoSeance::readAll(int channel)
{
    if (channel == RxChannel) {
        return m_serial_port->readAll();
    }

    return QByteArray();
}

qint64 SerialIoSeance::bytesAvailable(int channel) const
{
    return channel == RxChannel ? m_serial_port->bytesAvailable() : 0;
}

qint64 SerialIoSeance::bytesToWrite(int channel) const
{
    return channel == TxChannel ? m_serial_port->bytesToWrite() : 0;
}

QString SerialIoSeance::lastError() const
{
    return m_serial_port->errorString();
}

void SerialIoSeance::connectSignals()
{
    connect(m_serial_port, &QSerialPort::readyRead, this, [this]() {
        emit readyRead();
    });

    connect(m_serial_port, &QSerialPort::aboutToClose, this, [this]() {
        emit aboutToClose();
    });


    connect(m_serial_port, &QSerialPort::errorOccurred, this, [this](QSerialPort::SerialPortError error){
        emit errorOccurred(toString(error));
    });
}


