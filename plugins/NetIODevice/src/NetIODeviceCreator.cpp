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

QIODevice* NetIODeviceCreator::create() const
{
    return new QTcpSocket();
}

QIODevice* NetIODeviceCreator::create(const nlohmann::json& json) const
{
    if (json.is_null())
        return create();

    auto protocol = json.value("Protocol", QString());
    QAbstractSocket* socket;
        if (protocol == "UDP") {
            socket = new QUdpSocket();
        }
        else {
            socket = new QUdpSocket();
        }

    bool ok = false;
    auto remote_ip   = json.value("RemoteIp", QString());
    auto remote_port = json.value("Port", QString()).toInt(&ok);
        remote_port = ok ? remote_port : 0;

    socket->bind(QHostAddress(remote_ip), remote_port);

    return socket;
}
