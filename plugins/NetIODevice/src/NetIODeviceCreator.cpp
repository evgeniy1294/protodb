#include "protodb/NetIODeviceCreator.h"
#include "protodb/TcpSocket.h"
#include "protodb/UdpSocket.h"

#include <protodb/utils/JsonBaseUtils.h>
#include <nlohmann/json.hpp>

#include <QTcpSocket>
#include <QUdpSocket>

using namespace protodb;

QString NetIODeviceCreator::cid() const
{
    return creatorId();
}

QString NetIODeviceCreator::name() const
{
    return QString(QObject::tr("SerialIOWidget Creator"));
}

QString NetIODeviceCreator::description() const
{
    return QString(QObject::tr("Creator for serial io widget"));
}

QString NetIODeviceCreator::iconName() const {
    return QString();
}

QIODevice* NetIODeviceCreator::create(QString& desc) const
{
    desc = QString("TCP -:-");
    auto socket = new TcpSocket();

    return socket;
}

QIODevice* NetIODeviceCreator::create(const nlohmann::json& json, QString& desc) const
{
    if (json.is_null())
        return create(desc);

    desc = QString("%1 %3:%4");
    auto protocol = json.value("Protocol", QString());
    auto remote_ip   = json.value("RemoteIp", QString());
    bool ok = false;
    auto remote_port = json.value("Port", QString()).toInt(&ok);
        remote_port = ok ? remote_port : 0;

    desc = desc.arg(protocol);
    desc = desc.arg(remote_ip);
    desc = desc.arg(remote_port);

    QAbstractSocket* ret;
    if (protocol == "UDP") {
        auto socket = new UdpSocket();
            socket->setHostAddress(QHostAddress(remote_ip));
            socket->setPort(remote_port);
        ret = socket;
    }
    else {
        auto socket = new TcpSocket();
            socket->setHostAddress(QHostAddress(remote_ip));
            socket->setPort(remote_port);
        ret = socket;
    }

    return ret;
}
