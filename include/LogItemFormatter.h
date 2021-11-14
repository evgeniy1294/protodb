#pragma once

#include <QObject>
#include <QColor>
#include <QFont>
#include <QMap>
#include "Logger.h"

class LogItemFormatter: public QObject {
    Q_OBJECT

public:
    enum MessageFormat {
        kHexFormat,
        kAsciiFormat,
    };
    Q_ENUM(MessageFormat)

public:
    LogItemFormatter(QObject* parent = nullptr);
    LogItemFormatter(const LogItemFormatter& other, QObject* parent = nullptr);

    void setDefault();

    void setAttributeColor(const QColor& color);
    QColor attributeColor() const;

    void setAttributeFont(const QFont& font);
    QFont attributeFont() const;

    void setChannelColor(Logger::Channel ch, const QColor& color);
    QColor channelColor(Logger::Channel ch) const;

    void setChannelFont(Logger::Channel ch, const QFont& font);
    QFont  channelFont(Logger::Channel ch) const;

    void setChannelName(Logger::Channel ch, const QString& name);
    QString channelName(Logger::Channel ch) const;

    void setDateTimeFormat(const QString& format);
    const QString& dateTimeFormat() const;

    MessageFormat messageFormat() const;
    MessageFormat nextMessageFormat();
    void setMessageFormat(MessageFormat format);

    QString timestamp(const Logger::LogItem& item) const;
    QString message(const Logger::LogItem& item) const;
    QString channel(const Logger::LogItem& item) const;
    QColor color(const Logger::LogItem& item) const;
    QFont font(const Logger::LogItem& item) const;

signals:
    void changed();

private:
    QMap<Logger::Channel, QColor> m_ch_colors;
    QMap<Logger::Channel, QFont> m_ch_fonts;
    QMap<Logger::Channel, QString> m_ch_names;
    QFont m_attr_font;
    QColor m_attr_color;
    QString m_date_time_format;
    MessageFormat m_msg_format;
};

const QString& toString(LogItemFormatter::MessageFormat format);
