#include "LogFormatter.h"
#include <protodb/utils/JsonUtils.h>

LogFormatter::LogFormatter(QObject *parent)
    : QObject(parent)
{
    m_byte_format = Logger::ByteFormat::HexFormat;
    m_separator   = ' ';

    m_time_format = "hh:mm:ss.zzz";

    m_ch_names [Logger::ChannelFirst]   = "RX";
    m_ch_names [Logger::ChannelSecond]  = "TX";
    m_ch_names [Logger::ChannelComment] = "LUA";
    m_ch_names [Logger::ChannelError]   = "ERR";
}

LogFormatter::LogFormatter(const LogFormatter &other, QObject *parent)
    : QObject(parent)
{
    m_ch_names    = other.m_ch_names;
    m_time_format = other.m_time_format;
    m_separator   = other.m_separator;
}

void LogFormatter::setChannelName(Logger::Channel ch, const QString &name)
{
    m_ch_names[ch] = name;
}

QString LogFormatter::channelName(Logger::Channel ch) const
{
    return m_ch_names[ch];
}

void LogFormatter::setTimeFormat(const QString &format)
{
    m_time_format = format;
}

const QString &LogFormatter::timeFormat() const
{
    return m_time_format;
}

void LogFormatter::setSeparator(char s)
{
    m_separator = s;
}

char LogFormatter::separator() const
{
    return m_separator;
}

QString LogFormatter::channelName(const Logger::Event &event) const
{
    return m_ch_names[event.channel];
}

QString LogFormatter::timestamp(const Logger::Event &event) const
{
    return event.timestamp.toString(m_time_format);
}

QString LogFormatter::data(const Logger::Event &event, ByteFormat format) const
{
    bool ascii = event.channel == kCommentLogChannel || event.channel == kErrorLogChannel || format == kAsciiFormat;
    return ascii ? event.message : event.message.toHex(m_separator);
}



