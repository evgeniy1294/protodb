#include <QFont>
#include <QColor>
#include "LogItemFormatter.h"

LogItemFormatter::LogItemFormatter(QObject* parent)
    : QObject(parent)
{
    setDefault();
}

LogItemFormatter::LogItemFormatter(const LogItemFormatter& other, QObject* parent)
    : QObject(parent)
{
    m_ch_colors = other.m_ch_colors;
    m_ch_fonts  = other.m_ch_fonts;
}

void LogItemFormatter::setDefault()
{
    m_ch_colors[Logger::kFirstChannel]   = Qt::red;
    m_ch_colors[Logger::kSecondChannel]  = Qt::blue;
    m_ch_colors[Logger::kCommentChannel] = Qt::darkYellow;

    m_attr_color = Qt::darkGreen;

    m_ch_fonts[Logger::kFirstChannel]   = QFont();
    m_ch_fonts[Logger::kSecondChannel]  = QFont();
    m_ch_fonts[Logger::kCommentChannel] = QFont();

    m_attr_font = QFont();

    m_ch_names[Logger::kFirstChannel]   = ("[RX]");
    m_ch_names[Logger::kSecondChannel]  = ("[TX]");
    m_ch_names[Logger::kCommentChannel] = ("[USR]");

    m_date_time_format = "hh:mm:ss.zzz";

    m_msg_format = kHexFormat;

    emit changed();
}

void LogItemFormatter::setAttributeColor(const QColor& color)
{
    m_attr_color = color;
    emit changed();
}

QColor LogItemFormatter::attributeColor() const
{
    return m_attr_color;
}

void LogItemFormatter::setChannelColor(Logger::Channels ch, const QColor& color)
{
    if (ch < Logger::kChannelsNum) {
        m_ch_colors[ch] = color;
        emit changed();
    }
}

void LogItemFormatter::setChannelFont(Logger::Channels ch, const QFont& font)
{
    if (ch < Logger::kChannelsNum) {
        m_ch_fonts[ch] = font;
        emit changed();
    }
}

void LogItemFormatter::setDateTimeFormat(const QString& format)
{
    m_date_time_format = format;
    emit changed();
}

const QString& LogItemFormatter::dateTimeFormat() const
{
    return m_date_time_format;
}

LogItemFormatter::MessageFormat LogItemFormatter::nextMessageFormat()
{
    auto format = m_msg_format == kHexFormat ? kAsciiFormat : kHexFormat;
    setMessageFormat(format);

    return m_msg_format;
}

LogItemFormatter::MessageFormat LogItemFormatter::messageFormat() const
{
    return m_msg_format;
}

void LogItemFormatter::setMessageFormat(MessageFormat format)
{
    m_msg_format = format;
    emit changed();
}

QString LogItemFormatter::timestamp(const Logger::LogItem& item) const
{
    return item.timestamp.toString(m_date_time_format);
}

QString LogItemFormatter::message(const Logger::LogItem& item) const
{
    bool ascii = item.channel == Logger::kCommentChannel || m_msg_format == kAsciiFormat;
    return ascii ? item.message : item.message.toHex(' ');
}

QString LogItemFormatter::channel(const Logger::LogItem& item) const
{
    return channelName(item.channel);
}

QColor LogItemFormatter::color(const Logger::LogItem& item) const
{
    return channelColor(item.channel);
}

QFont LogItemFormatter::font(const Logger::LogItem& item) const
{
    return channelFont(item.channel);
}

QColor LogItemFormatter::channelColor(Logger::Channels ch) const
{
    if (ch < Logger::kChannelsNum) {
        return m_ch_colors[ch];
    }

    return {};
}

void LogItemFormatter::setAttributeFont(const QFont& font)
{
    m_attr_font = font;
    emit changed();
}

QFont LogItemFormatter::attributeFont() const
{
    return m_attr_font;
}

QFont LogItemFormatter::channelFont(Logger::Channels ch) const
{
    if (ch < Logger::kChannelsNum) {
        return m_ch_fonts[ch];
    }

    return {};
}

void LogItemFormatter::setChannelName(Logger::Channels channel, const QString& name)
{
    if (channel < Logger::kChannelsNum) {
        m_ch_names[channel] = name;
        emit changed();
    }
}

QString LogItemFormatter::channelName(Logger::Channels channel) const
{
    static const QString default_name = "[ERR]";

    if (channel < Logger::kChannelsNum) {
        return m_ch_names[channel];
    }

    return default_name;
}

const QString& toString(LogItemFormatter::MessageFormat format)
{
    static const QString hexFormatName("HEX");
    static const QString asciiFormatName("ASCII");
    static const QString unknownFormatName("ERR");

    switch(format) {
        case LogItemFormatter::kHexFormat:
            return hexFormatName;
        case LogItemFormatter::kAsciiFormat:
            return asciiFormatName;
        default:
            break;
    }

    return unknownFormatName;
}
