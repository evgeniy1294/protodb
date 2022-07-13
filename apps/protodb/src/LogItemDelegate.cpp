#include "LogItemDelegate.h"
#include "LogFormatter.h"

#include <QtGui>
#include <QApplication>

LogItemDelegate::LogItemDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
    , m_fmt(nullptr)

{
    m_attr_color = QColor(Qt::darkGreen);
    m_attr_font  = QApplication::font();

    m_ch_colors[Logger::ChannelFirst]   = QColor(Qt::darkMagenta);
    m_ch_fonts [Logger::ChannelFirst]   = QApplication::font();

    m_ch_colors[Logger::ChannelSecond]  = QColor(Qt::blue);
    m_ch_fonts [Logger::ChannelSecond]  = QApplication::font();

    m_ch_colors[Logger::ChannelComment] = QColor(Qt::darkYellow);
    m_ch_fonts [Logger::ChannelComment] = QApplication::font();

    m_ch_colors[Logger::ChannelError]   = QColor(Qt::red);
    m_ch_fonts [Logger::ChannelError]   = QApplication::font();
}

void LogItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto m_option = option;

    // ----------[DATA]---------- //
    auto model = index.model();
    int rowCount = model->rowCount();
    if (index.row() >= rowCount)
        return;

    auto channel = static_cast<Logger::Channel>(model->data( model->index(index.row(), Logger::ColumnChannel) ).toInt());
    Qt::AlignmentFlag align = static_cast<Qt::AlignmentFlag>(0x81);;
    QColor color;
    QFont font;
    QString data;
    switch ( index.column() ) {
        case Logger::ColumnTimestamp: {
            color = m_attr_color;
            font  = m_attr_font;
            align = Qt::AlignCenter;

            auto dt = model->data( index ).value<QDateTime>();
            data = m_fmt != nullptr ? m_fmt->format( dt ) : LogFormatter::defaultFormat(dt);
        } break;

        case Logger::ColumnChannel: {
            color = m_attr_color;
            font  = m_attr_font;

            data  = m_fmt != nullptr ? m_fmt->format(channel) : LogFormatter::defaultFormat(channel);
        } break;

        case Logger::ColumnMsg: {
            color = m_ch_colors[channel];
            font  = m_ch_fonts [channel];

            auto raw_data = index.model()->data(index).toByteArray();
            data = m_fmt != nullptr ? m_fmt->format( channel, raw_data ) :
                                      LogFormatter::defaultFormat(channel, raw_data);

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
        auto event = index.data(Logger::EventRole).value<Logger::Event>();
        ret = m_fmt != nullptr ? m_fmt->format( event ) : LogFormatter::defaultFormat(event);
    }

    return ret.simplified();
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

void LogItemDelegate::setFormatter(LogFormatter* fmt)
{
    if (fmt != m_fmt && fmt != nullptr) {
        m_fmt = fmt;
    }

    return;
}

LogFormatter* LogItemDelegate::formatter() const
{
    return m_fmt;
}

