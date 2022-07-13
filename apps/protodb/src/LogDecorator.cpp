#include "LogDecorator.h"

#include <protodb/utils/JsonUtils.h>

#include <QApplication>


LogDecorator::LogDecorator(QObject* parent)
    : QObject(parent)
{
    setDefault();
}

LogDecorator::LogDecorator(const LogDecorator &other, QObject *parent)
    : QObject(parent)
{
    m_attr_color = other.m_attr_color;
    m_attr_font  = other.m_attr_font;
    m_ch_colors  = other.m_ch_colors;
    m_ch_fonts   = other.m_ch_fonts;
}

void LogDecorator::setAttributeColor(const QColor& color)
{
    m_attr_color = color;
    emit sConfigChanged();
}

QColor LogDecorator::attributeColor() const
{
    return m_attr_color;
}

void LogDecorator::setAttributeFont(const QFont& font)
{
    m_attr_font = font;
    emit sConfigChanged();
}

QFont LogDecorator::attributeFont() const
{
    return m_attr_font;
}

void LogDecorator::setChannelColor(Logger::Channel channel, const QColor& color)
{
    m_ch_colors[channel] = color;
    emit sConfigChanged();
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
    emit sConfigChanged();
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

}

void LogDecorator::fromJson(const nlohmann::json& json)
{
    m_attr_color = json["AttributeColor"].get<QColor>();
    m_attr_font  = json["AttributeFont"].get<QFont>();
    m_ch_colors[Logger::ChannelFirst]   = json["FirstChannelColor"].get<QColor>();
    m_ch_fonts [Logger::ChannelFirst]   = json["FirstChannelFont"].get<QFont>();
    m_ch_fonts [Logger::ChannelSecond]  = json["SecondChannelFont"].get<QFont>();
    m_ch_colors[Logger::ChannelSecond]  = json["SecondChannelColor"].get<QColor>();
    m_ch_colors[Logger::ChannelComment] = json["CommentChannelColor"].get<QColor>();
    m_ch_fonts [Logger::ChannelComment] = json["CommentChannelFont"].get<QFont>();
    m_ch_colors[Logger::ChannelError]   = json["ErrorChannelColor"].get<QColor>();
    m_ch_fonts [Logger::ChannelError]   = json["ErrorChannelFont"].get<QFont>();

    emit sConfigChanged();
}

void LogDecorator::toJson(nlohmann::json& json) const
{
    json["AttributeColor"]      = m_attr_color;
    json["AttributeFont"]       = m_attr_font;
    json["FirstChannelColor"]   = m_ch_colors[Logger::ChannelFirst];
    json["FirstChannelFont"]    = m_ch_fonts [Logger::ChannelFirst];
    json["SecondChannelColor"]  = m_ch_colors[Logger::ChannelSecond];
    json["SecondChannelFont"]   = m_ch_fonts [Logger::ChannelSecond];
    json["CommentChannelColor"] = m_ch_colors[Logger::ChannelComment];
    json["CommentChannelFont"]  = m_ch_fonts [Logger::ChannelComment];
    json["ErrorChannelColor"]   = m_ch_colors[Logger::ChannelError];
    json["ErrorChannelFont"]    = m_ch_fonts [Logger::ChannelError];
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

}

QColor LogDecorator::defaultAttributeColor()
{
    return QColor(Qt::darkGreen);
}

QFont LogDecorator::defaultAttributeFont()
{
    return QApplication::font();
}
