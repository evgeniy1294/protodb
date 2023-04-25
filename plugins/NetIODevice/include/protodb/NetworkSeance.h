#pragma once

#include "protodb/Seance.h"

#include <QHostAddress>

class QAbstractSocket;

namespace protodb {

class NetworkSeance: Seance
{
    Q_OBJECT

    enum Mode { server, client };
    enum Protocol { tcp, udp };

public:
    NetworkSeance(QObject* parent = nullptr);
    ~NetworkSeance() = default;

    bool setActive(bool active = true) override;
    void setDisactive() override;

    qint64 write(int channel, const char *data, qint64 maxSize) override;
    qint64 write(int channel, const char *data) override;
    qint64 write(int channel, const QByteArray &byteArray) override;

    qint64 read(int channel, char *data, qint64 maxSize) override ;
    QByteArray read(int channel, qint64 maxSize) override ;
    QByteArray readAll(int channel) override ;

    qint64 bytesAvailable(int channel) const override;
    qint64 bytesToWrite(int channel) const override;

    QString lastError() const override;

private:
    void connectSignals();

private:
    int m_mode;
    int m_protocol;

    QAbstractSocket* m_socket;
    QHostAddress m_remote_address;
    quint16 m_remote_port;
};

} // namespace protodb
