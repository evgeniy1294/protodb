#include "LogItemDelegate.h"

#include <protodb/Logger.h>

#include <QtGui>
#include <QApplication>


LogItemDelegate::LogItemDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)

{
    m_byte_format = ByteFormat::kHexFormat;
    m_separator   = ' ';

    m_time_format = "hh:mm:ss.zzz";
    m_attr_color = QColor(Qt::darkGreen);
    m_attr_font  = QApplication::font();

    m_ch_colors[Logger::kChannelFirst]   = QColor(Qt::darkMagenta);
    m_ch_fonts [Logger::kChannelFirst]   = QApplication::font();
    m_ch_names [Logger::kChannelFirst]   = "RX";

    m_ch_colors[Logger::kChannelSecond]  = QColor(Qt::blue);
    m_ch_fonts [Logger::kChannelSecond]  = QApplication::font();
    m_ch_names [Logger::kChannelSecond]  = "TX";

    m_ch_colors[Logger::kChannelComment] = QColor(Qt::darkYellow);
    m_ch_fonts [Logger::kChannelComment] = QApplication::font();
    m_ch_names [Logger::kChannelComment] = "LUA";

    m_ch_colors[Logger::kChannelError]   = QColor(Qt::red);
    m_ch_fonts [Logger::kChannelError]   = QApplication::font();
    m_ch_names [Logger::kChannelError]   = "ERR";
}

void LogItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto m_option = option;

    // ----------[DATA]---------- //
    auto model = index.model();
    int rowCount = model->rowCount();
    if (index.row() >= rowCount)
        return;

    auto channel = static_cast<Logger::Channel>(model->data( model->index(index.row(), Logger::kColumnChannel) ).toInt());
    Qt::AlignmentFlag align = static_cast<Qt::AlignmentFlag>(0x81);;
    QColor color;
    QFont font;
    QString data;
    switch ( index.column() ) {
        case Logger::kColumnTimestamp: {
            color = m_attr_color;
            font  = m_attr_font;
            align = Qt::AlignCenter;

            data = model->data( index ).value<QDateTime>().toString(m_time_format);
        } break;

        case Logger::kColumnChannel: {
            color = m_attr_color;
            font  = m_attr_font;

            data  = m_ch_names[channel];
        } break;

        case Logger::kColumnMsg: {
            color = m_ch_colors[channel];
            font  = m_ch_fonts [channel];

            if ( channel == Logger::kChannelFirst || channel == Logger::kChannelSecond ) {
                if ( m_byte_format == kHexFormat ) {
                    data = model->data(index).toByteArray().toHex(m_separator);
                    break;
                }
            }

            data = index.model()->data(index).toString();

        } break;
    }

    painter->save();
        painter->setFont(font);
        painter->setPen(color);
        m_option.palette.setColor(QPalette::HighlightedText, color);


        QColor selection_color = QApplication::style()->standardPalette().color(QPalette::Inactive, QPalette::Highlight);
        if (m_option.state & QStyle::State_Selected) {
            painter->fillRect(m_option.rect, selection_color);
        }

        QApplication::style()->drawItemText(painter, option.rect, align, m_option.palette, true, data);

        painter->restore();
}

QString LogItemDelegate::message(const QModelIndex& index)
{
    QString ret;
    if (index.isValid()) {
        auto model = index.model();
        auto channel = static_cast<Logger::Channel>(model->data( model->index(index.row(), Logger::kColumnChannel) ).toInt());
        auto timestamp = model->data( model->index(index.row(), Logger::kColumnTimestamp) ).value<QDateTime>().toString(m_time_format);

        QString data;
        if ( channel == Logger::kChannelFirst || channel == Logger::kChannelSecond ) {
            if ( m_byte_format == kHexFormat ) {
                data = model->data( model->index(index.row(), Logger::kColumnMsg) ).toByteArray().toHex(m_separator);
            }
        }

        if ( data.isEmpty() ) {
            data = model->data( model->index(index.row(), Logger::kColumnMsg) ).toString();
        }

        ret =  timestamp + ' ' + m_ch_names[channel] + ' ' + data;
    }

    return ret.simplified();
}

ByteFormat LogItemDelegate::byteFormat() const
{
    return m_byte_format;
}

void LogItemDelegate::setByteFormat(ByteFormat format)
{
    m_byte_format = format;
}

void LogItemDelegate::setAttributeColor(const QColor& color)
{
    m_attr_color = color;
}

QColor LogItemDelegate::attributeColor() const
{
    return m_attr_color;
}

void LogItemDelegate::setAttributeFont(const QFont& font)
{
    m_attr_font = font;
}

QFont LogItemDelegate::attributeFont() const
{
    return m_attr_font;
}

void LogItemDelegate::setChannelColor(Logger::Channel channel, const QColor& color)
{
    m_ch_colors[channel] = color;
}

QColor LogItemDelegate::channelColor(Logger::Channel channel) const
{
    return m_ch_colors[channel];
}

void LogItemDelegate::setChannelFont(Logger::Channel channel, const QFont& font)
{
    m_ch_fonts[channel] = font;
}

QFont LogItemDelegate::channelFont(Logger::Channel channel) const
{
    return m_ch_fonts[channel];
}

void LogItemDelegate::setChannelName(Logger::Channel ch, const QString &name)
{
    m_ch_names[ch] = name;
}

QString LogItemDelegate::channelName(Logger::Channel ch) const
{
    return m_ch_names[ch];
}

void LogItemDelegate::setTimestampFormat(const QString &format)
{
    m_time_format = format;
}

const QString& LogItemDelegate::timestampFormat() const
{
    return m_time_format;
}

void LogItemDelegate::setSeparator(char s)
{
    m_separator = s;
}

char LogItemDelegate::separator() const
{
    return m_separator;
}

