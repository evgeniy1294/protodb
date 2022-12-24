#pragma once

#include <QTcpSocket>
#include <QHostAddress>

namespace protodb {

class TcpSocket: public QTcpSocket
{
    Q_OBJECT

public:
    explicit TcpSocket(QObject *parent = nullptr);
   ~TcpSocket() = default;

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
