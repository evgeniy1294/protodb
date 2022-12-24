#include "protodb/UdpSocket.h"

using namespace protodb;

UdpSocket::UdpSocket(QObject* parent)
    : QUdpSocket(parent)
    , m_port(0)
{

}

void UdpSocket::setHostAddress(const QHostAddress& address)
{
    m_host_address = address;
}

QHostAddress UdpSocket::hostAddress() const
{
    return m_host_address;
}

void UdpSocket::setPort(quint16 port)
{
    m_port = port;
}

quint16 UdpSocket::port() const
{
    return m_port;
}

bool UdpSocket::open(OpenMode mode)
{
    connectToHost(m_host_address, m_port, mode);
    return true;
}

void UdpSocket::close()
{
    disconnectFromHost();
    QAbstractSocket::close();
}
