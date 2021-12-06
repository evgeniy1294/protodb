#include "qnlohmann.h"
#include "LogFormatter.h"

LogFormatter::LogFormatter(QObject *parent)
    : QObject(parent)
{
    setDefaultConfig();
}

LogFormatter::LogFormatter(const LogFormatter &other, QObject *parent)
    : QObject(parent)
{
    m_names       = other.m_names;
    m_time_format = other.m_time_format;
    m_separator   = other.m_separator;
}

void LogFormatter::setChannelName(LogChannel ch, const QString &name)
{
    m_names[ch] = name;
    emit sConfigChanged();
}

QString LogFormatter::channelName(LogChannel ch) const
{
    return m_names[ch];
}

void LogFormatter::setTimestampFormat(const QString &format)
{
    m_time_format = format;
    emit sConfigChanged();
}

const QString &LogFormatter::timestampFormat() const
{
    return m_time_format;
}

void LogFormatter::setSeparator(char s)
{
    m_separator = s;
    emit sConfigChanged();
}

char LogFormatter::separator() const
{
    return m_separator;
}

void LogFormatter::defaultConfig(nlohmann::json &json) const
{
    json["Separator"]          = ' ';
    json["TimestampFormat"]    = "hh:mm:ss.zzz";
    json["FirstChannelName"]   = "[RX]";
    json["SecondChannelName"]  = "[TX]";
    json["CommentChannelName"] = "[LUA]";
    json["ErrorChannelName"]   = "[ERR]";
}

void LogFormatter::fromJson(const nlohmann::json &json)
{
    m_separator   = json["Separator"].get<char>();
    m_time_format = json["TimestampFormat"].get<QString>();
    m_names[kFirstLogChannel]   = json["FirstChannelName"].get<QString>();
    m_names[kSecondLogChannel]  = json["SecondChannelName"].get<QString>();
    m_names[kCommentLogChannel] = json["CommentChannelName"].get<QString>();
    m_names[kErrorLogChannel]   = json["ErrorChannelName"].get<QString>();

    emit sConfigChanged();
}

void LogFormatter::toJson(nlohmann::json &json) const
{
    json["Separator"]          = m_separator;
    json["TimestampFormat"]    = m_time_format;
    json["FirstChannelName"]   = m_names[kFirstLogChannel];
    json["SecondChannelName"]  = m_names[kSecondLogChannel];
    json["CommentChannelName"] = m_names[kCommentLogChannel];
    json["ErrorChannelName"]   = m_names[kErrorLogChannel];
}

QString LogFormatter::channelName(const LogEvent &event) const
{
    return m_names[event.channel];
}

QString LogFormatter::timestamp(const LogEvent &event) const
{
    return event.timestamp.toString(m_time_format);
}

QString LogFormatter::data(const LogEvent &event, ByteFormat format) const
{
    bool ascii = event.channel == kCommentLogChannel || event.channel == kErrorLogChannel || format == kAsciiFormat;
    return ascii ? event.message : event.message.toHex(m_separator);
}



