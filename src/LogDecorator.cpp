#include <QApplication>
#include "qnlohmann.h"
#include "LogDecorator.h"


LogDecorator::LogDecorator(QObject* parent)
    : QObject(parent)
{
    setDefaultConfig();
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

void LogDecorator::setChannelColor(LogChannel channel, const QColor& color)
{
    m_ch_colors[channel] = color;
    emit sConfigChanged();
}

QColor LogDecorator::channelColor(LogChannel channel) const
{
    return m_ch_colors[channel];
}

QColor LogDecorator::channelColor(const LogEvent &event) const
{
    return channelColor(event.channel);
}

void LogDecorator::setChannelFont(LogChannel channel, const QFont& font)
{
    m_ch_fonts[channel] = font;
    emit sConfigChanged();
}

QFont LogDecorator::channelFont(LogChannel channel) const
{
    return m_ch_fonts[channel];
}

QFont LogDecorator::channelFont(const LogEvent &event) const
{
    return channelFont(event.channel);
}

#include <iostream>

void LogDecorator::defaultConfig(nlohmann::json& json) const
{
    json["AttributeColor"]      = QColor(Qt::darkGreen);
    json["AttributeFont"]       = QApplication::font();
    json["FirstChannelColor"]   = QColor(Qt::red);
    json["FirstChannelFont"]    = QApplication::font();
    json["SecondChannelColor"]  = QColor(Qt::blue);
    json["SecondChannelFont"]   = QApplication::font();
    json["CommentChannelColor"] = QColor(Qt::darkYellow);
    json["CommentChannelFont"]  = QApplication::font();
}

void LogDecorator::fromJson(nlohmann::json& json)
{
    m_attr_color = json["AttributeColor"].get<QColor>();
    m_attr_font  = json["AttributeFont"].get<QFont>();
    m_ch_colors[kFirstLogChannel]   = json["FirstChannelColor"].get<QColor>();
    m_ch_colors[kSecondLogChannel]  = json["SecondChannelColor"].get<QColor>();
    m_ch_colors[kCommentLogChannel] = json["CommentChannelColor"].get<QColor>();
    m_ch_fonts [kFirstLogChannel]   = json["FirstChannelFont"].get<QFont>();
    m_ch_fonts [kSecondLogChannel]  = json["SecondChannelFont"].get<QFont>();
    m_ch_fonts [kCommentLogChannel] = json["CommentChannelFont"].get<QFont>();

    emit sConfigChanged();
}

void LogDecorator::toJson(nlohmann::json& json) const
{
    json["AttributeColor"]      = m_attr_color;
    json["AttributeFont"]       = m_attr_font;
    json["FirstChannelColor"]   = m_ch_colors[kFirstLogChannel];
    json["FirstChannelFont"]    = m_ch_fonts [kFirstLogChannel];
    json["SecondChannelColor"]  = m_ch_colors[kSecondLogChannel];
    json["SecondChannelFont"]   = m_ch_fonts [kSecondLogChannel];
    json["CommentChannelColor"] = m_ch_colors[kCommentLogChannel];
    json["CommentChannelFont"]  = m_ch_fonts [kCommentLogChannel];
}
