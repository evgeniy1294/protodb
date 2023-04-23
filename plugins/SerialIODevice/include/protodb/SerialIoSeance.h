#pragma once

#include <protodb/Seance.h>

class QSerialPort;

namespace protodb {

class SerialIoSeance: public Seance
{
    Q_OBJECT

public:
    SerialIoSeance(QObject* parent = nullptr);
    ~SerialIoSeance();

    bool setActive(bool active = true) override;
    void setDisactive() override;

    qint64 write(int channel, const char *data, qint64 maxSize) override;
    qint64 write(int channel, const char *data) override;
    qint64 write(int channel, const QByteArray &byteArray) override;

    qint64 read(int channel, char *data, qint64 maxSize) override;
    QByteArray read(int channel, qint64 maxSize) override;
    QByteArray readAll(int channel) override;

    qint64 bytesAvailable(int channel) const override;
    qint64 bytesToWrite(int channel) const override;

    QString lastError() const override;

private:
    void connectSignals();

private:
    QSerialPort* m_serial_port;
};

} // namespace protodb
