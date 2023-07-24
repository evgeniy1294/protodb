#include "protodb/NetworkConnection.h"

#include <protodb/utils/JsonBaseUtils.h>

#include <QTcpServer>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QNetworkDatagram>

using namespace protodb;

NetworkConnection::NetworkConnection(QObject* parent)
    : Connection(parent)
    , m_tcp_server(new QTcpServer(this))
{
    m_tcp_server->setMaxPendingConnections(1);
    m_description = tr("Network disconnected");

    connectSignals();
}

NetworkConnection::~NetworkConnection()
{
}

bool NetworkConnection::isEnabled() const
{
    if (m_protocol == tcp && m_mode == server) {
        return m_tcp_server->isListening();
    }
    else {
        if (m_socket) {
            return m_socket->isOpen();
        }
    }

    return false;
}

void NetworkConnection::connectSignals()
{
    connect(m_tcp_server, &QTcpServer::newConnection, this, &NetworkConnection::newServerConnection);
}

bool NetworkConnection::setEnable(bool enabled)
{
    if (enabled) {
        m_mode = m_cfg.value("Mode", QString()) == "Server" ? server : client;
        m_protocol = m_cfg.value("Protocol", QString()) == "TCP" ? tcp : udp;
        m_local_port  = m_cfg.value("LocalPort", int(0));
        m_remote_port = m_cfg.value("RemotePort", int(0));
        m_remote_ip   = QHostAddress(m_cfg.value("RemoteIP", QString()));

        if (m_protocol == udp) {
            m_socket = new QUdpSocket(this);
            m_socket->bind(QHostAddress::LocalHost, m_local_port);
            m_description = tr("Network UDP:%1").arg(QString::number(m_local_port));

            m_socket->open(QIODevice::ReadWrite);
            connect(m_socket, &QAbstractSocket::readyRead, this, &NetworkConnection::socketReadyRead);
        }
        else {
            if (m_mode == client) {
                m_socket = new QTcpSocket(this);
                m_socket->connectToHost(m_remote_ip, m_remote_port);

                m_description = tr("Network TCP:Client:%1:%2").arg(m_remote_ip.toString(), QString::number(m_remote_port));

                connect(m_socket, &QAbstractSocket::stateChanged, this, &NetworkConnection::socketStateChanged);
                connect(m_socket, &QAbstractSocket::readyRead, this, &NetworkConnection::socketReadyRead);
                connect(m_socket, &QAbstractSocket::errorOccurred, this, &NetworkConnection::socketErrorOccurred);
            }
            else {
                m_tcp_server->listen(QHostAddress::Any, m_local_port);

                m_description = tr("Network TCP:Server:%1").arg(QString::number(m_local_port));

                connect(m_tcp_server, &QTcpServer::newConnection, this, &NetworkConnection::newServerConnection);
                connect(m_tcp_server, &QTcpServer::acceptError, this, &NetworkConnection::socketErrorOccurred);
            }
        }
    }
    else {
        setDisable();
    }

    return true;
}

void NetworkConnection::setDisable()
{
    m_tcp_server->close();
    if (m_socket) {
        m_socket->deleteLater();
    }

    m_description = tr("Network disconnected");
    emit aboutToClose();
}

bool NetworkConnection::isWritable() const
{
    if (m_socket) {
        return m_socket->isWritable();
    }

    return false;
}

qint64 NetworkConnection::writeTo(const char* data, qint64 size)
{
    if (m_socket != nullptr) {
        if (m_protocol == udp) {
            auto socket = qobject_cast<QUdpSocket*>(m_socket);
            return socket->writeDatagram(data, size, m_remote_ip, m_remote_port);
        }
        else {
            return m_socket->write(data, size);
        }
    }

    return 0;
}

qint64 NetworkConnection::writeTo(const char* data)
{
    if (m_socket != nullptr) {
        if (m_protocol == udp) {
            auto socket = qobject_cast<QUdpSocket*>(m_socket);
            return socket->writeDatagram(data, m_remote_ip, m_remote_port);
        }
        else {
            return m_socket->write(data);
        }
    }

    return 0;
}

qint64 NetworkConnection::writeTo(const QByteArray& bytes)
{
    if (m_socket != nullptr) {
        if (m_protocol == udp) {
            auto socket = qobject_cast<QUdpSocket*>(m_socket);
            return socket->writeDatagram(bytes, m_remote_ip, m_remote_port);
        }
        else {
            return m_socket->write(bytes);
        }
    }

    return 0;
}

qint64 NetworkConnection::readFrom(char* data, qint64 maxSize)
{
    if (m_socket != nullptr) {
        return m_socket->read(data, maxSize);
    }

    return 0;
}

QByteArray NetworkConnection::readFrom(qint64 maxSize)
{
    if (m_socket != nullptr) {
        return m_socket->read(maxSize);
    }

    return QByteArray();
}

QByteArray NetworkConnection::readAllFrom()
{
    if (m_socket != nullptr) {
        if (m_protocol == udp) {
            QByteArray ret;
            QUdpSocket* udpSocket = qobject_cast<QUdpSocket*>(m_socket);

            while (udpSocket->hasPendingDatagrams()) {
                QNetworkDatagram datagram = udpSocket->receiveDatagram();
                ret.append(datagram.data());
            }

            return ret;
        }
        else {
            return m_socket->readAll();
        }
    }

    return QByteArray();
}

qint64 NetworkConnection::bytesAvailable() const
{
    return m_socket->bytesAvailable();
}

qint64 NetworkConnection::bytesToWrite() const
{
    return m_socket->bytesToWrite();
}

QString NetworkConnection::lastError() const
{
    if (m_socket != nullptr)
        return m_socket->errorString();

    return QString();
}

void NetworkConnection::newServerConnection()
{
    if (m_tcp_server->hasPendingConnections()) {
        m_socket = m_tcp_server->nextPendingConnection();
            m_socket->setParent(this);

        connect(m_socket, &QAbstractSocket::errorOccurred, this, &NetworkConnection::socketErrorOccurred);
        connect(m_socket, &QAbstractSocket::stateChanged, this, &NetworkConnection::socketStateChanged);
        connect(m_socket, &QAbstractSocket::readyRead, this, &NetworkConnection::socketReadyRead);
    }
}

void NetworkConnection::socketReadyRead()
{
    emit readyRead();
}

void NetworkConnection::socketErrorOccurred(QAbstractSocket::SocketError err)
{
    emit errorOccurred(sender() == m_tcp_server ?
        m_tcp_server->errorString() : m_socket->errorString());
}

void NetworkConnection::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    if (socketState == QAbstractSocket::UnconnectedState) {
        m_socket->deleteLater();
        if (m_mode == client) {
            emit aboutToClose();
        }
    }
}
