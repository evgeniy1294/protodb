#pragma once

#include <QObject>
#include <QColor>
#include <QFont>
#include <QMap>
#include "logging.h"
#include "Configurable.h"

class LogFormatter: public QObject, public Configurable
{
    Q_OBJECT

public:
    LogFormatter(QObject* parent = nullptr);
    LogFormatter(const LogFormatter& other, QObject* parent = nullptr);

    void setChannelName(LogChannel ch, const QString& name);
    QString channelName(LogChannel ch) const;

    void setTimestampFormat(const QString& format);
    const QString& timestampFormat() const;

    void setSeparator(char s);
    char separator() const;

    void defaultConfig(nlohmann::json& json) const override;
    void fromJson(const nlohmann::json& json) override;
    void toJson(nlohmann::json& json) const override;
    const QString& jsonPrefix() const override;

    QString channelName(const LogEvent& event) const;
    QString timestamp(const LogEvent& event) const;
    QString data(const LogEvent& event, ByteFormat format) const;

signals:
    void sConfigChanged();

private:
    QMap<LogChannel, QString> m_names;
    QString m_time_format;
    char m_separator;
};


