#pragma once

#include <QtCore>
#include <QColor>
#include <QFont>
#include <protodb/configurable/Configurable.h>
#include <protodb/shared_types/LogTypes.h>

class LogDecorator: public QObject, public Configurable
{
    Q_OBJECT

public:
    LogDecorator(QObject* parent = nullptr);
    LogDecorator(const LogDecorator& other, QObject* parent = nullptr);

    void setAttributeColor(const QColor& color);
    QColor attributeColor() const;

    void setAttributeFont(const QFont& font);
    QFont attributeFont() const;

    void setChannelColor(LogChannel channel, const QColor& color);
    QColor channelColor(LogChannel channel) const;
    QColor channelColor(const LogEvent& event) const;

    void setChannelFont(LogChannel channel, const QFont& font);
    QFont channelFont(LogChannel channel) const;
    QFont channelFont(const LogEvent& event) const;

signals:
    void sConfigChanged();

private:
    QColor m_attr_color;
    QFont  m_attr_font;
    QMap<LogChannel, QColor> m_ch_colors;
    QMap<LogChannel, QFont>  m_ch_fonts;
};
