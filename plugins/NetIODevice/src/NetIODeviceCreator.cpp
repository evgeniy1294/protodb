#include "protodb/NetIODeviceCreator.h"

#include <protodb/utils/JsonBaseUtils.h>

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
    desc = QString("TCP:- -:-");
    auto socket = new QTcpSocket();

    return socket;
}

QIODevice* NetIODeviceCreator::create(const nlohmann::json& json, QString& desc) const
{
    if (json.is_null())
        return create(desc);

    desc = QString("%1:%2 %3:%4");
    auto protocol = json.value("Protocol", QString());
    QAbstractSocket* socket;
        if (protocol == "UDP") {
            socket = new QUdpSocket();
            desc = desc.arg("UDP");
        }
        else {
            socket = new QTcpSocket();
            desc = desc.arg("TCP");
        }

    bool ok = false;
    auto remote_ip   = json.value("RemoteIp", QString());
    auto remote_port = json.value("Port", QString()).toInt(&ok);
        remote_port = ok ? remote_port : 0;

    socket->bind(QHostAddress(remote_ip), remote_port);
        desc = desc.arg(socket->localPort());
        desc = desc.arg(socket->peerAddress().toString());
        desc = desc.arg(socket->peerPort());

    return socket;
}
