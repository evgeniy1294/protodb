#pragma once

#include "Logger.h"

#include <QtCore>
#include <QColor>
#include <QFont>

class LogDecorator: public QObject, public Configurable
{
    Q_OBJECT

public:
    LogDecorator(QObject* parent = nullptr);
    LogDecorator(const LogDecorator& other, QObject* parent = nullptr);

    void setDefault();

    void setAttributeColor(const QColor& color);
    QColor attributeColor() const;

    void setAttributeFont(const QFont& font);
    QFont attributeFont() const;

    void setChannelColor(Logger::Channel channel, const QColor& color);
    QColor channelColor (Logger::Channel channel) const;
    QColor channelColor (const Logger::Event& event) const;

    void setChannelFont(Logger::Channel channel, const QFont& font);
    QFont channelFont  (Logger::Channel channel) const;
    QFont channelFont  (const Logger::Event& event) const;

    static QMap<Logger::Channel, QColor> defaultChannelColors();
    static QMap<Logger::Channel, QFont>  defaultChannelFonts();
    static QColor defaultAttributeColor();
    static QFont  defaultAttributeFont();

private:
    QColor m_attr_color;
    QFont  m_attr_font;
    QMap<Logger::Channel, QColor> m_ch_colors;
    QMap<Logger::Channel, QFont>  m_ch_fonts;
};
