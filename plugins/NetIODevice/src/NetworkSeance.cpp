#include "protodb/NetworkSeance.h"
#include "protodb/utils/JsonBaseUtils.h"

#include <QTcpSocket>
#include <QUdpSocket>

protodb::NetworkSeance::NetworkSeance(QObject* parent)
    : Seance(parent)
{

}

bool protodb::NetworkSeance::setActive(bool active)
{
    auto mode = m_cfg.value("Mode", QString());
    auto protocol = m_cfg.value("Protocol", QString());

    if (protocol == "UDP") {
        auto socket = new QUdpSocket();

        socket->setHostAddress(QHostAddress(remote_ip));
        socket->setPort(remote_port);
        ret = socket;

        m_socket = socket;
    }
    else {
        auto socket = new TcpSocket();
        socket->setHostAddress(QHostAddress(remote_ip));
        socket->setPort(remote_port);
        ret = socket;
    }

}

void protodb::NetworkSeance::setDisactive()
{

}
