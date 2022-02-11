#include "LogItemDelegate.h"

#include <protodb/Logger.h>

#include <QtGui>
#include <QApplication>


LogItemDelegate::LogItemDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
    , m_byte_role(Logger::HexDisplayRole)
{
}

void LogItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto m_option = option;

    painter->save();

    // ----------[DATA]---------- //
    const QByteArray data = index.model()->data(index, m_byte_role).value<QByteArray>();

    // -------[TEXT COLOR]------- //
    auto color = index.model()->data(index, Qt::ForegroundRole).value<QColor>();

        painter->setPen(color);
        m_option.palette.setColor(QPalette::HighlightedText, color);

    // -------[TEXT FONT]------- //
    auto font = index.model()->data(index, Qt::FontRole).value<QFont>();
        painter->setFont(font);

    // -------[TEXT ALIGN]------- //
    auto align = index.model()->data(index, Qt::TextAlignmentRole).value<int>();

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
    return m_byte_role == Logger::AsciiDisplayRole ? ByteFormat::kAsciiFormat :
                                                       ByteFormat::kHexFormat;
}

void LogItemDelegate::setByteFormat(ByteFormat format)
{
    m_byte_role = (format == ByteFormat::kAsciiFormat) ? Logger::AsciiDisplayRole :
                                                         Logger::HexDisplayRole;
}

