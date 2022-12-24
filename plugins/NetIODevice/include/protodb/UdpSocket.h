#pragma once

#include <QUdpSocket>
#include <QHostAddress>

namespace protodb {

class UdpSocket: public QUdpSocket
{
    Q_OBJECT

public:
    explicit UdpSocket(QObject *parent = nullptr);
   ~UdpSocket() = default;

    void setHostAddress(const QHostAddress& address);
    QHostAddress hostAddress() const;

    void setPort(quint16 port);
    quint16 port() const;

    bool open(OpenMode mode) override;
    void close() override;

private:
    QHostAddress m_host_address;
    quint16 m_port;
};


} // namespace protodb

