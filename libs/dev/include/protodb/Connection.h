#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <nlohmann/json.hpp>

#include <QObject>

namespace protodb {

class PROTODB_DEV_EXPORT Connection: public QObject
{
    Q_OBJECT

public:
    Connection(QObject* parent = nullptr)
        : QObject(parent) { }
    ~Connection() = default;

    virtual QString statusString() const { return m_description; };

    void setConfig(const nlohmann::json& cfg) { m_cfg = cfg; }
    void config(nlohmann::json& cfg) const { cfg = m_cfg; }

    virtual bool isEnabled() const = 0;
    virtual bool setEnable(bool enable = true) = 0;
    virtual void setDisable() = 0;

    virtual bool isWritable() const = 0;

    virtual qint64 writeTo(const char *data, qint64 maxSize) = 0;
    virtual qint64 writeTo(const char *data) = 0;
    virtual qint64 writeTo(const QByteArray &byteArray) = 0;

    virtual qint64 readFrom(char *data, qint64 maxSize) = 0;
    virtual QByteArray readFrom(qint64 maxSize) = 0;
    virtual QByteArray readAllFrom() = 0;

    virtual qint64 bytesAvailable() const = 0;
    virtual qint64 bytesToWrite() const = 0;

    virtual QString lastError() const { return QString(); };

signals:
    void readyRead();
    void aboutToClose();
    void errorOccurred(QString what);

protected:
    QString m_description;
    nlohmann::json m_cfg;
};

} // namespace protodb
