#pragma once

#include <protodb/Connection.h>

class QSerialPort;

namespace protodb {

class SerialConnection final: public Connection
{
    Q_OBJECT

public:
    SerialConnection(QObject* parent = nullptr);
    ~SerialConnection() = default;

    bool isEnabled() const override;
    bool setEnable(bool enable = true) override;
    void setDisable() override;

    bool isWritable() const override;

    qint64 writeTo(const char *data, qint64 maxSize) override;
    qint64 writeTo(const char *data) override;
    qint64 writeTo(const QByteArray &byteArray) override;

    qint64 readFrom(char *data, qint64 maxSize) override;
    QByteArray readFrom(qint64 maxSize) override;
    QByteArray readAllFrom() override;

    qint64 bytesAvailable() const override;
    qint64 bytesToWrite() const override;

    QString lastError() const override;

private slots:
    void serialReadyRead();
    void serialErrorOccurred();

private:
    void connectSignals();

private:
    QSerialPort* m_serial_port;
    QString m_port_name;
};

} // namespace protodb

