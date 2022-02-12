#pragma once

#include <QStyledItemDelegate>
#include <protodb/Logger.h>
#include <protodb/shared_types/LogTypes.h>

class LogItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LogItemDelegate(QObject* aParent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    QString message(const QModelIndex& index);

    ByteFormat byteFormat() const;
    void setByteFormat(ByteFormat format);

    void setAttributeColor(const QColor& color);
    QColor attributeColor() const;

    void setAttributeFont(const QFont& font);
    QFont attributeFont() const;

    void setChannelColor(Logger::Channel channel, const QColor& color);
    QColor channelColor (Logger::Channel channel) const;

    void setChannelFont(Logger::Channel channel, const QFont& font);
    QFont channelFont  (Logger::Channel channel) const;

    void setChannelName(Logger::Channel channel, const QString& name);
    QString channelName(Logger::Channel channel) const;

    void setTimestampFormat(const QString& format);
    const QString& timestampFormat() const;

    void setSeparator(char s);
    char separator() const;

private:
    ByteFormat m_byte_format;

    char m_separator;
    QColor m_attr_color;
    QFont  m_attr_font;
    QString m_time_format;
    QMap<Logger::Channel, QColor>  m_ch_colors;
    QMap<Logger::Channel, QFont>   m_ch_fonts;
    QMap<Logger::Channel, QString> m_ch_names;
};



