#include "protodb/SerialConnection.h"

#include <protodb/utils/JsonBaseUtils.h>

#include <QSerialPort>
#include <QSerialPortInfo>

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

SerialConnection::SerialConnection(QObject* parent)
    : Connection(parent)
    , m_serial_port(new QSerialPort(this))
{
    m_description = tr("Serial disconnected");
    connectSignals();
}

bool SerialConnection::isEnabled() const
{
    return m_serial_port->isOpen();
}

bool SerialConnection::setEnable(bool enabled)
{
    if (enabled) {
        QString desc = QString("%1 %2 %3%4%5");

        bool ok = false;

        auto port_name = m_cfg.value("PortName", QString());
        m_serial_port->setPortName(port_name);
        desc = desc.arg(m_serial_port->portName());

        auto baudrate = m_cfg.value("Baudrate", QString()).toInt(&ok);
        if (ok) m_serial_port->setBaudRate(baudrate);
        desc = desc.arg(m_serial_port->baudRate());

        auto data_bits = toDataBits(m_cfg.value("DataBits", QString()));
        m_serial_port->setDataBits(data_bits);
        desc = desc.arg(m_serial_port->dataBits());

        auto parity = toParity(m_cfg.value("Parity", QString()));
        m_serial_port->setParity(parity);
        desc = desc.arg(toShortParityName(m_serial_port->parity()));

        auto stop_bits = toStopBits(m_cfg.value("StopBits", QString()));
        m_serial_port->setStopBits(stop_bits);
        desc = desc.arg(m_serial_port->stopBits());

        auto flow_control = toFlowControl(m_cfg.value("FlowControl", QString()));
        m_serial_port->setFlowControl(flow_control);

        if (m_cfg.value("OpenMode", QString()) == "Monitoring") {
            m_serial_port->open(QIODevice::ReadOnly);
        }
        else {
            m_serial_port->open(QIODevice::ReadWrite);
        }

        QSerialPortInfo portInfo(m_serial_port->portName());
        #ifdef _WIN32
            m_port_name = portInfo.portName();
        #else
            m_port_name = portInfo.systemLocation();
        #endif

        if (!m_serial_port->isOpen()) emit errorOccurred(m_serial_port->errorString());
        else m_description = desc;
    }
    else {
        m_serial_port->close();
        m_description = tr("Serial disconnected");
    }

    return m_serial_port->isOpen();
}

void SerialConnection::setDisable()
{
    m_serial_port->close();
    m_description = tr("Serial disconnected");
}

bool SerialConnection::isWritable() const
{
    return m_serial_port->isWritable();
}

qint64 SerialConnection::writeTo(const char* data, qint64 maxSize)
{
    if (m_serial_port->isOpen()) {
        return m_serial_port->write(data, maxSize);
    }

    return 0;
}

qint64 SerialConnection::writeTo(const char* data)
{
    if (m_serial_port->isOpen()) {
        return m_serial_port->write(data);
    }

    return 0;
}

qint64 SerialConnection::writeTo(const QByteArray& byteArray)
{
    if (m_serial_port->isOpen()) {
        return m_serial_port->write(byteArray);
    }

    return 0;
}

qint64 SerialConnection::readFrom(char* data, qint64 maxSize)
{
    if (m_serial_port->isOpen()) {
        return m_serial_port->read(data, maxSize);
    }

    return 0;
}

QByteArray SerialConnection::readFrom(qint64 maxSize)
{
    if (m_serial_port->isOpen()) {
        return m_serial_port->read(maxSize);
    }

    return QByteArray();
}

QByteArray SerialConnection::readAllFrom()
{
    if (m_serial_port->isOpen()) {
        return m_serial_port->readAll();
    }

    return QByteArray();
}

qint64 SerialConnection::bytesAvailable() const
{
    return m_serial_port->bytesAvailable();
}

qint64 SerialConnection::bytesToWrite() const
{
    return m_serial_port->bytesToWrite();
}

QString SerialConnection::lastError() const
{
    return QString("%1: %2").arg(m_port_name, m_serial_port->errorString());
}

void SerialConnection::serialReadyRead()
{
    emit readyRead();
}

void SerialConnection::serialErrorOccurred()
{
    emit errorOccurred(QString("%1: %2").arg(m_port_name, m_serial_port->errorString()));
    emit aboutToClose();
}

void SerialConnection::connectSignals()
{
    connect(m_serial_port, &QSerialPort::readyRead, this, &SerialConnection::serialReadyRead);
    connect(m_serial_port, &QSerialPort::errorOccurred, this, &SerialConnection::serialErrorOccurred);
}
