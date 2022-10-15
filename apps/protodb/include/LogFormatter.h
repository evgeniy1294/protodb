#pragma once

#include "Logger.h"
//#include <protodb/configurable/Configurable.h>

#include <QColor>
#include <QFont>
#include <QMap>

class LogFormatter
{
public:
    enum ByteFormat {
        HexFormat,
        AsciiFormat,
    };

    enum Flags {
        ChannelNameEnabled = 0b001,
        TimeStampEnabled   = 0b010,
        DataEnabled        = 0b100,
        AllEnabled = ChannelNameEnabled | TimeStampEnabled | DataEnabled
    };

    static const QString DefaultTimeFormat;
    static const ByteFormat DefaultByteFormat;
    static const char DefaultSeparator;

public:
    LogFormatter();
    LogFormatter(const LogFormatter& other);

    void setDefault();
    void toJson(nlohmann::json& json) const;
    void fromJson(const nlohmann::json& json);

    void setChannelName(Logger::Channel ch, const QString& name);
    QString channelName(Logger::Channel ch) const;

    void setTimeFormat(const QString& format);
    const QString& timeFormat() const;

    void setSeparator(char s);
    char separator() const;

    void setByteFormat(ByteFormat format);
    ByteFormat byteFormat() const;

    static QMap<Logger::Channel, QString> defaultChannelNames();

    static QString defaultFormat(Logger::Channel ch);
    static QString defaultFormat(const QDateTime& dt);
    static QString defaultFormat(Logger::Channel ch, const QByteArray& msg);
    static QString defaultFormat(const Logger::Event& event);

    QString channelName(const Logger::Event& event) const;
    QString timestamp  (const Logger::Event& event) const;
    QString data       (const Logger::Event& event) const;
    QString format     (const Logger::Event& event, Flags flags = AllEnabled) const;

    QString format(Logger::Channel ch) const;
    QString format(const QDateTime& dt) const;
    QString format(Logger::Channel ch, const QByteArray& msg) const;

private:
    QMap<Logger::Channel, QString> m_ch_names;
    QString m_time_format;
    ByteFormat m_byte_format;
    char m_separator;
};


