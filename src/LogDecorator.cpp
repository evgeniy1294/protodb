#include <QApplication>
#include "LogDecorator.h"

void to_json(nlohmann::json& j, const QString& str) {
    j = str.toStdString();
}

void from_json(const nlohmann::json& j, QString& str) {
    str = j.get<std::string>().c_str();
}

void to_json(nlohmann::json& j, const QColor& color) {
    j = color.name();
}

void from_json(const nlohmann::json& j, QColor& color) {
    color.setNamedColor(j.get<QString>());
}

void to_json(nlohmann::json& j, const QFont& font) {
    j = font.toString();
}

void from_json(const nlohmann::json& j, QFont& font) {
    font.fromString(j.get<QString>());
}


LogDecorator::LogDecorator(QObject* parent)
    : QObject(parent)
{
    setDefaultConfig();
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

void LogDecorator::setChannelColor(LogTableModel::Channel channel, const QColor& color)
{
    m_ch_colors[channel] = color;
    emit sConfigChanged();
}

QColor LogDecorator::channelColor(LogTableModel::Channel channel) const
{
    return m_ch_colors[channel];
}

void LogDecorator::setChannelFont(LogTableModel::Channel channel, const QFont& font)
{
    m_ch_fonts[channel] = font;
    emit sConfigChanged();
}

QFont LogDecorator::channelFont(LogTableModel::Channel channel) const
{
    return m_ch_fonts[channel];
}

#include <iostream>

void LogDecorator::defaultConfig(nlohmann::json& json) const
{
    json["AttributeColor"]      = QColor(Qt::green);
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
    m_ch_colors[LogTableModel::FirstChannel] = json["FirstChannelColor"].get<QColor>();
    m_ch_colors[LogTableModel::SecondChannel] = json["SecondChannelColor"].get<QColor>();
    m_ch_colors[LogTableModel::CommentChannel] = json["CommentChannelColor"].get<QColor>();
    m_ch_fonts[LogTableModel::FirstChannel] = json["FirstChannelFont"].get<QFont>();
    m_ch_fonts[LogTableModel::SecondChannel] = json["SecondChannelFont"].get<QFont>();
    m_ch_fonts[LogTableModel::CommentChannel] = json["CommentChannelFont"].get<QFont>();
}

void LogDecorator::toJson(nlohmann::json& json) const
{
    json["AttributeColor"]      = m_attr_color;
    json["AttributeFont"]       = m_attr_font;
    json["FirstChannelColor"]   = m_ch_colors[LogTableModel::FirstChannel];
    json["FirstChannelFont"]    = m_ch_fonts[LogTableModel::FirstChannel];
    json["SecondChannelColor"]  = m_ch_colors[LogTableModel::SecondChannel];
    json["SecondChannelFont"]   = m_ch_fonts[LogTableModel::SecondChannel];
    json["CommentChannelColor"] = m_ch_colors[LogTableModel::CommentChannel];
    json["CommentChannelFont"]  = m_ch_fonts[LogTableModel::CommentChannel];
}
