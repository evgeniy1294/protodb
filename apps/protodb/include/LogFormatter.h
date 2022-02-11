#pragma once

#include <QObject>
#include <QColor>
#include <QFont>
#include <QMap>

#include <protodb/shared_types/LogTypes.h>
#include <protodb/configurable/Configurable.h>

class LogFormatter: public QObject//, public Configurable
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


