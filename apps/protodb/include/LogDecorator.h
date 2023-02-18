#pragma once

#include "Logger.h"

#include <QtCore>
#include <QColor>
#include <QFont>

namespace protodb {

class LogDecorator
{
public:
    LogDecorator();
    LogDecorator(const LogDecorator& other);

    void setDefault();

    void toJson(nlohmann::json& json) const;
    void fromJson(const nlohmann::json& json);

    void setAttributeColor(const QColor& color);
    QColor attributeColor() const;

    void setChannelColors(const QMap<Logger::Channel, QColor>& colors);
    void setChannelColor(Logger::Channel channel, const QColor& color);
    QColor channelColor (Logger::Channel channel) const;
    QColor channelColor (const Logger::Event& event) const;

    void setChannelFonts(const QMap<Logger::Channel, QFont>& fonst);
    void setChannelFont(Logger::Channel channel, const QFont& font);
    QFont channelFont  (Logger::Channel channel) const;
    QFont channelFont  (const Logger::Event& event) const;

    QMap<Logger::Channel, QColor> channelColors() const;
    QMap<Logger::Channel, QFont>  channelFonts()  const;

    static QMap<Logger::Channel, QColor> defaultChannelColors();
    static QMap<Logger::Channel, QFont>  defaultChannelFonts();
    static QColor defaultAttributeColor();
    static QColor defaultChannelColor(Logger::Channel channel);
    static QFont  defaultChannelFont(Logger::Channel channel);

private:
    QColor m_attr_color;
    QMap<Logger::Channel, QColor> m_ch_colors;
    QMap<Logger::Channel, QFont>  m_ch_fonts;
};

} // namespace protodb
