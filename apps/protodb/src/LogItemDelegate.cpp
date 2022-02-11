#include "LogItemDelegate.h"

#include <protodb/Logger.h>

#include <QtGui>
#include <QApplication>


LogItemDelegate::LogItemDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
    , m_byte_format( ByteFormat::kHexFormat )
{
}

void LogItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto m_option = option;

    painter->save();

    // ----------[DATA]---------- //
    auto model = index.model();

    //auto channel = model->data( model->index(index.row(), Logger::kColumnChannel) ).toInt();
    auto color = Qt::green;
    auto font  = QFont();
    auto align = Qt::AlignCenter;
    QByteArray data;
    switch ( index.column() ) {
        case Logger::kColumnChannel: {
            data = "[CH]";
        }

        case Logger::kColumnTimestamp: {
            auto timestamp = model->data( index ).value<QDateTime>();
            data = timestamp.toString().toLatin1();
        }

        case Logger::kColumnMsg: {
            color = Qt::red;
            align = static_cast<Qt::AlignmentFlag>(0x81);
            data = index.model()->data(index).toByteArray();
        }
    }

    painter->setFont(font);

    painter->setPen(color);
    m_option.palette.setColor(QPalette::HighlightedText, color);

    // -------[DRAW SELECTION]-------- //
    QColor selection_color = QApplication::style()->standardPalette().color(QPalette::Inactive, QPalette::Highlight);
    if (m_option.state & QStyle::State_Selected) {
        painter->fillRect(m_option.rect, selection_color);
    }

    // -------[DRAW TEXT]-------- //
    QApplication::style()->drawItemText(painter, option.rect, align, m_option.palette, true, data);

    painter->restore();
}

ByteFormat LogItemDelegate::byteFormat() const
{
    return m_byte_format;
}

void LogItemDelegate::setByteFormat(ByteFormat format)
{
    m_byte_format = format;
}

