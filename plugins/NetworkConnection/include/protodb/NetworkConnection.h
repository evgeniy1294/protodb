#pragma once

#include "protodb/Connection.h"

#include <QHostAddress>

class QAbstractSocket;
class QTcpServer;

namespace protodb {

class NetworkConnection: public Connection
{
    Q_OBJECT

    enum Mode { server, client };
    enum Protocol { tcp, udp };

public:
    NetworkConnection(QObject* parent = nullptr);
    ~NetworkConnection();

    bool isEnabled() const override;
    bool setEnable(bool active = true) override;
    void setDisable() override;

    bool isWritable() const override;

    qint64 writeTo(const char* data, qint64 size) override;
    qint64 writeTo(const char* data) override;
    qint64 writeTo(const QByteArray& bytes) override;

    qint64 readFrom(char *data, qint64 maxSize) override ;
    QByteArray readFrom(qint64 maxSize) override ;
    QByteArray readAllFrom() override ;

    qint64 bytesAvailable() const override;
    qint64 bytesToWrite() const override;

    QString lastError() const override;

private slots:
    void newServerConnection();
    void socketReadyRead();
    void socketStateChanged(QAbstractSocket::SocketState state);
    void socketErrorOccurred(QAbstractSocket::SocketError err);

private:
    void connectSignals();

private:
    int m_mode;
    int m_protocol;

    QTcpServer* m_tcp_server;
    QAbstractSocket* m_socket;
    QHostAddress m_remote_ip;
    quint16 m_remote_port;
    quint16 m_local_port;
};

} // namespace protodb
