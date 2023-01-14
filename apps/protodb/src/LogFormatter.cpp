#include "LogFormatter.h"
#include <protodb/utils/JsonUtils.h>

namespace protodb {

const QString LogFormatter::DefaultTimeFormat = "hh:mm:ss.zzz";
const LogFormatter::ByteFormat LogFormatter::DefaultByteFormat = LogFormatter::HexFormat;
const char LogFormatter::DefaultSeparator = ' ';

LogFormatter::LogFormatter()
{
    setDefault();
}

LogFormatter::LogFormatter(const LogFormatter &other)
{
    m_ch_names    = other.m_ch_names;
    m_time_format = other.m_time_format;
    m_separator   = other.m_separator;
    m_byte_format = other.m_byte_format;
}

void LogFormatter::setDefault()
{
    auto names = defaultChannelNames();

    m_byte_format = DefaultByteFormat;
    m_separator   = DefaultSeparator;

    m_time_format = DefaultTimeFormat;

    m_ch_names[Logger::ChannelFirst]   = names[Logger::ChannelFirst];
    m_ch_names[Logger::ChannelSecond]  = names[Logger::ChannelSecond];
    m_ch_names[Logger::ChannelComment] = names[Logger::ChannelComment];
    m_ch_names[Logger::ChannelError]   = names[Logger::ChannelError];
}

void LogFormatter::toJson(nlohmann::json& json) const
{
    json["ByteFormat"] = m_byte_format == HexFormat ? "HEX" : "ASCII";
    json["Separator"]  = m_separator;
    json["TimeFormat"] = m_time_format;

    nlohmann::json names;
        names["First"]   = m_ch_names.value(Logger::ChannelFirst);
        names["Second"]  = m_ch_names.value(Logger::ChannelSecond);
        names["Comment"] = m_ch_names.value(Logger::ChannelComment);
        names["Error"]   = m_ch_names.value(Logger::ChannelError);

    json["ChannelNames"] = names;
}

void LogFormatter::fromJson(const nlohmann::json& json)
{
    m_byte_format = json.value("ByteFormat", QString()) == "ASCII" ? AsciiFormat : HexFormat;
    m_separator   = json.value("Separator", ' ');
    m_time_format = json.value("TimeFormat", DefaultTimeFormat);

    nlohmann::json names = json.value("ChannelNames", nlohmann::json::object());
        m_ch_names[Logger::ChannelFirst]   = names.value("First"  , defaultFormat(Logger::ChannelFirst));
        m_ch_names[Logger::ChannelSecond]  = names.value("Second" , defaultFormat(Logger::ChannelSecond));
        m_ch_names[Logger::ChannelComment] = names.value("Comment", defaultFormat(Logger::ChannelComment));
        m_ch_names[Logger::ChannelError]   = names.value("Error"  , defaultFormat(Logger::ChannelError));
}

void LogFormatter::setChannelName(Logger::Channel ch, const QString &name)
{
    if (!name.isEmpty()) {
        m_ch_names[ch] = name;
    }
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

void LogFormatter::setByteFormat(ByteFormat format)
{
    m_byte_format = format;
}

LogFormatter::ByteFormat LogFormatter::byteFormat() const
{
    return m_byte_format;
}

QString LogFormatter::channelName(const Logger::Event &event) const
{
    return m_ch_names[event.channel];
}

QString LogFormatter::timestamp(const Logger::Event &event) const
{
    return event.timestamp.toString(m_time_format);
}

QString LogFormatter::data(const Logger::Event &event) const
{
    bool ascii = event.channel == Logger::ChannelComment || event.channel == Logger::ChannelError || m_byte_format == AsciiFormat;
    return ascii ? QString(event.message).trimmed() : event.message.toHex(m_separator);
}

QString LogFormatter::format(const Logger::Event &event, Flags flags) const
{
    QString ret;
        if ( (flags & TimeStampEnabled) != 0 ) {
            ret += timestamp(event);
            ret += " ";
        }

        if ( (flags & ChannelNameEnabled) != 0 ) {
            ret += channelName(event);
            ret += " ";
        }

        if ( (flags & DataEnabled) != 0 )
            ret += data(event);

        return ret;
}

QString LogFormatter::format(Logger::Channel ch) const
{
    return m_ch_names[ch];
}

QString LogFormatter::format(const QDateTime &dt) const
{
    return dt.toString(m_time_format);
}

QString LogFormatter::format(Logger::Channel ch, const QByteArray &msg) const
{
    bool ascii = ch == Logger::ChannelComment || ch == Logger::ChannelError || m_byte_format == AsciiFormat;
    return ascii ? QString(msg).trimmed() : msg.toHex(m_separator);
}

QMap<Logger::Channel, QString> LogFormatter::defaultChannelNames()
{
    static const QMap<Logger::Channel, QString> DefaultNames = {
        {Logger::ChannelFirst,  "RX"},
        {Logger::ChannelSecond, "TX"},
        {Logger::ChannelComment,"USR"},
        {Logger::ChannelError,  "ERR"}
    };

    return DefaultNames;
}

QString LogFormatter::defaultFormat(Logger::Channel ch)
{
    return DefaultTimeFormat[ch];
}

QString LogFormatter::defaultFormat(const QDateTime &dt)
{
    return dt.toString(DefaultTimeFormat);
}

QString LogFormatter::defaultFormat(Logger::Channel ch, const QByteArray &msg)
{
    bool ascii = ch == Logger::ChannelComment || ch == Logger::ChannelError;
    return ascii ? QString(msg).trimmed() : msg.toHex(DefaultSeparator);
}

QString LogFormatter::defaultFormat(const Logger::Event &event)
{
    QString ret;
        ret += defaultFormat(event.timestamp);
        ret += " ";
        ret += defaultFormat(event.channel);
        ret += " ";
        ret += defaultFormat(event.channel, event.message);

        return ret;
}

} // namespace protodb


