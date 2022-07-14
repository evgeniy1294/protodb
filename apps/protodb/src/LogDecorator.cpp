#include "LogDecorator.h"

#include <protodb/utils/JsonUtils.h>

#include <QApplication>


LogDecorator::LogDecorator()
{
    setDefault();
}

LogDecorator::LogDecorator(const LogDecorator &other)
{
    m_attr_color = other.m_attr_color;
    m_attr_font  = other.m_attr_font;
    m_ch_colors  = other.m_ch_colors;
    m_ch_fonts   = other.m_ch_fonts;
}

void LogDecorator::setAttributeColor(const QColor& color)
{
    m_attr_color = color;
}

QColor LogDecorator::attributeColor() const
{
    return m_attr_color;
}

void LogDecorator::setAttributeFont(const QFont& font)
{
    m_attr_font = font;
}

QFont LogDecorator::attributeFont() const
{
    return m_attr_font;
}

void LogDecorator::setChannelColor(Logger::Channel channel, const QColor& color)
{
    m_ch_colors[channel] = color;
}

QColor LogDecorator::channelColor(Logger::Channel channel) const
{
    return m_ch_colors[channel];
}

QColor LogDecorator::channelColor(const Logger::Event &event) const
{
    return channelColor(event.channel);
}

void LogDecorator::setChannelFont(Logger::Channel channel, const QFont& font)
{
    m_ch_fonts[channel] = font;
}

QFont LogDecorator::channelFont(Logger::Channel channel) const
{
    return m_ch_fonts[channel];
}

QFont LogDecorator::channelFont(const Logger::Event &event) const
{
    return channelFont(event.channel);
}

void LogDecorator::setDefault()
{
    auto colors = defaultChannelColors();
    auto fonts  = defaultChannelFonts();

    m_attr_color = defaultAttributeColor();
    m_attr_font  = defaultAttributeFont();
    m_ch_colors[Logger::ChannelFirst]   = colors[Logger::ChannelFirst];
    m_ch_fonts [Logger::ChannelFirst]   = fonts [Logger::ChannelFirst];
    m_ch_colors[Logger::ChannelSecond]  = colors[Logger::ChannelSecond];
    m_ch_fonts [Logger::ChannelSecond]  = fonts [Logger::ChannelSecond];
    m_ch_colors[Logger::ChannelComment] = colors[Logger::ChannelComment];
    m_ch_fonts [Logger::ChannelComment] = fonts [Logger::ChannelComment];
    m_ch_colors[Logger::ChannelError]   = colors[Logger::ChannelError];
    m_ch_fonts [Logger::ChannelError]   = fonts [Logger::ChannelError];
}

QMap<Logger::Channel, QColor> LogDecorator::defaultChannelColors()
{
    static QMap<Logger::Channel, QColor> m_def_colors = {
        {Logger::ChannelFirst,   QColor(Qt::darkMagenta)},
        {Logger::ChannelSecond,  QColor(Qt::blue)},
        {Logger::ChannelComment, QColor(Qt::darkYellow)},
        {Logger::ChannelError,   QColor(Qt::red)}
    };

    return m_def_colors;
}

QMap<Logger::Channel, QFont> LogDecorator::defaultChannelFonts()
{
    static QMap<Logger::Channel, QFont> m_def_fonts = {
        {Logger::ChannelFirst,   QApplication::font()},
        {Logger::ChannelSecond,  QApplication::font()},
        {Logger::ChannelComment, QApplication::font()},
        {Logger::ChannelError,   QApplication::font()}
    };

    return m_def_fonts;
}

QColor LogDecorator::defaultAttributeColor()
{
    return QColor(Qt::darkGreen);
}

QFont LogDecorator::defaultAttributeFont()
{
    return QApplication::font();
}

QColor LogDecorator::defaultChannelColor(Logger::Channel channel)
{
    return defaultChannelColors()[channel];
}

QFont LogDecorator::defaultChannelFont(Logger::Channel channel)
{
    return defaultChannelFonts()[channel];
}
