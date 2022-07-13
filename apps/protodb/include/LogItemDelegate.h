#pragma once

#include "Logger.h"
#include <QStyledItemDelegate>

class LogFormatter;

class LogItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LogItemDelegate(QObject* aParent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QString message(const QModelIndex& index);

    void setAttributeColor(const QColor& color);
    QColor attributeColor() const;

    void setAttributeFont(const QFont& font);
    QFont attributeFont() const;

    void setChannelColor(Logger::Channel channel, const QColor& color);
    QColor channelColor (Logger::Channel channel) const;

    void setChannelFont(Logger::Channel channel, const QFont& font);
    QFont channelFont  (Logger::Channel channel) const;

    void setFormatter(LogFormatter* fmt);
    LogFormatter* formatter() const;

private:
    LogFormatter* m_fmt;

    QColor m_attr_color;
    QFont  m_attr_font;
    QMap<Logger::Channel, QColor>  m_ch_colors;
    QMap<Logger::Channel, QFont>   m_ch_fonts;
};



