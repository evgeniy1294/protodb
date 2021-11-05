#include <QtGui>
#include <QApplication>
#include "LogItemDelegate.h"

LogItemDelegate::LogItemDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
{
}

void LogItemDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    auto m_option = option;

    painter->save();

    // -------[TEXT COLOR]------- //
    auto color = index.model()->data(index, Qt::ForegroundRole).value<QColor>();

        painter->setPen(color);
        m_option.palette.setColor(QPalette::HighlightedText, color);

    // -------[SELECTION STYLE]-------- //
    m_option.state &= ~(QStyle::State_MouseOver | QStyle::State_Item | QStyle::State_HasFocus);

    if (m_option.state & QStyle::State_Selected) {
        m_option.state = QStyle::State_Selected | QStyle::State_Enabled;
    }

    QStyledItemDelegate::paint(painter, m_option, index);

    painter->restore();
}
