#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <nlohmann/json.hpp>

#include <QObject>

namespace protodb {

class PROTODB_DEV_EXPORT Seance: public QObject
{
    Q_OBJECT

public:
    enum Channels {
        FirstChannel,
        SecondChannel,

        RxChannel = FirstChannel,
        TxChannel = SecondChannel
    };

    Seance(QObject* parent = nullptr);
   ~Seance() = default;

    QString seanceDescription() const;

    void setConfig(const nlohmann::json& cfg);
    void config(nlohmann::json& cfg) const;

    bool isActive() const;
    virtual bool setActive(bool active = true);
    virtual void setDisactive();

    virtual qint64 write(int channel, const char *data, qint64 maxSize) = 0;
    virtual qint64 write(int channel, const char *data) = 0;
    virtual qint64 write(int channel, const QByteArray &byteArray) = 0;

    virtual qint64 read(int channel, char *data, qint64 maxSize) = 0;
    virtual QByteArray read(int channel, qint64 maxSize) = 0;
    virtual QByteArray readAll(int channel) = 0;

    virtual qint64 bytesAvailable(int channel) const;
    virtual qint64 bytesToWrite(int channel) const;

    virtual QString lastError() const;
signals:
    void readyRead();
    void aboutToClose();
    void errorOccurred(QString what);

protected:
    bool m_active;
    QString m_description;

    nlohmann::json m_cfg;
};

} // namespace protodb
