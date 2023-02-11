#include "LogItemDelegate.h"
#include "LogFormatter.h"
#include "LogDecorator.h"

#include <QtGui>
#include <QApplication>

using namespace protodb;

LogItemDelegate::LogItemDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
    , m_fmt(nullptr)
    , m_dec(nullptr)
{

}

void LogItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto m_option = option;

    auto model = index.model();
    int rowCount = model->rowCount();
    if (index.row() >= rowCount)
        return;

    auto channel = static_cast<Logger::Channel>(model->data( model->index(index.row(), Logger::ColumnChannel) ).toInt());
    int flags = static_cast<Qt::AlignmentFlag>(0x81);
    QColor color;
    QFont font;
    QString data;
    switch ( index.column() ) {
        case Logger::ColumnTimestamp: {
            color = m_dec != nullptr ? m_dec->attributeColor() : LogDecorator::defaultAttributeColor();
            font  = m_dec != nullptr ? m_dec->attributeFont () : LogDecorator::defaultAttributeFont();
            flags = Qt::AlignCenter;

            auto dt = model->data( index ).value<QDateTime>();
            data = m_fmt != nullptr ? m_fmt->format( dt ) : LogFormatter::defaultFormat( dt );
        } break;

        case Logger::ColumnChannel: {
            color = m_dec != nullptr ? m_dec->attributeColor() : LogDecorator::defaultAttributeColor();
            font  = m_dec != nullptr ? m_dec->attributeFont () : LogDecorator::defaultAttributeFont();

            data  = m_fmt != nullptr ? m_fmt->format(channel) : LogFormatter::defaultFormat(channel);
        } break;

        case Logger::ColumnMsg: {
            flags |=  Qt::TextWordWrap;
            color = m_dec != nullptr ? m_dec->channelColor(channel) : LogDecorator::defaultChannelColors().value(channel);
            font  = m_dec != nullptr ? m_dec->channelFont (channel) : LogDecorator::defaultChannelFonts ().value(channel);

            auto raw_data = index.model()->data(index).toByteArray();
            if (m_fmt != nullptr) {
                flags |= m_fmt->byteFormat() == LogFormatter::AsciiFormat ?
                            Qt::TextWrapAnywhere : Qt::TextWordWrap;
                data = m_fmt->format( channel, raw_data );
            }
            else {
                data = LogFormatter::defaultFormat(channel, raw_data);
                flags |= Qt::TextWordWrap;
            }

        } break;
    }

    painter->save();
        painter->setFont(font);
        painter->setPen(color);
        m_option.palette.setColor(QPalette::HighlightedText, color);

        QColor selection_color = QApplication::style()->standardPalette().color(QPalette::Inactive, QPalette::ToolTipBase);

        if (m_option.state & QStyle::State_Selected) {
            painter->fillRect(m_option.rect, selection_color);
        }

        QApplication::style()->drawItemText(painter, m_option.rect, flags, m_option.palette, true, data);

    painter->restore();
}


QSize LogItemDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    if (index.column() == Logger::ColumnMsg) {
        auto model = index.model();
        auto channel = static_cast<Logger::Channel>(model->data( model->index(index.row(), Logger::ColumnChannel) ).toInt());

        if (channel == Logger::ChannelFirst || channel == Logger::ChannelSecond) {
            QSize cell_size = option.rect.size();
            auto font  = m_dec != nullptr ? m_dec->channelFont (channel) :
                                            LogDecorator::defaultChannelFonts ().value(channel);

            auto raw_data = index.model()->data(index).toByteArray();
            QString data; int flags = 0;
            if (m_fmt != nullptr) {
                flags |= m_fmt->byteFormat() == LogFormatter::AsciiFormat ?
                            Qt::TextWrapAnywhere : Qt::TextWordWrap;
                data = m_fmt->format( channel, raw_data );
            }
            else {
                data = LogFormatter::defaultFormat(channel, raw_data);
                flags |= Qt::TextWordWrap;
            }

            auto fm = QFontMetrics(font);
                int width = fm.horizontalAdvance(data, flags);
                int height = fm.height()*width/cell_size.width() +
                                ((width % cell_size.width() != 0) ? fm.height() : 0);

            cell_size.setHeight(height);

            return cell_size;
        }
    }


    return QStyledItemDelegate::sizeHint(option, index);
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

void LogItemDelegate::setDecorator(LogDecorator* dec)
{
    if (dec != m_dec && dec != nullptr) {
        m_dec = dec;
    }
}

LogDecorator* LogItemDelegate::decorator() const
{
    return m_dec;
}

