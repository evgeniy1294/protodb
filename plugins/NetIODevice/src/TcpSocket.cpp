#include "protodb/TcpSocket.h"

using namespace protodb;

TcpSocket::TcpSocket(QObject* parent)
    : QTcpSocket(parent)
    , m_port(0)
{

}

void TcpSocket::setHostAddress(const QHostAddress& address)
{
    m_host_address = address;
}

QHostAddress TcpSocket::hostAddress() const
{
    return m_host_address;
}

void TcpSocket::setPort(quint16 port)
{
    m_port = port;
}

quint16 TcpSocket::port() const
{
    return m_port;
}

bool TcpSocket::open(OpenMode mode)
{
    connectToHost(m_host_address, m_port, mode);
    return waitForConnected();
}

void TcpSocket::close()
{
    disconnectFromHost();
        waitForDisconnected();
    QAbstractSocket::close();
}
