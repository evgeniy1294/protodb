#include <QtGui>
#include <QApplication>
#include "log_field_delegate.h"

LogFieldDelegate::LogFieldDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
{
}

void LogFieldDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    painter->save();

    auto text  = index.model()->data(index, Qt::DisplayRole).toString();

    // -------[TEXT COLOR]------- //
    auto color = index.model()->data(index, Qt::ForegroundRole).value<QColor>();
        painter->setPen(color);

    // -------[BACKGROUND COLOR]-------- //
    if (option.state & QStyle::State_Selected) {
        //painter->fillRect(option.rect, option.palette.highlight());
        painter->fillRect(option.rect, option.palette.midlight());
    }

    // -------[BACKGROUND COLOR]-------- //
    int flags = Qt::AlignBaseline;
    if (index.column() == 0) flags |= Qt::AlignRight;
    if (index.column() == 1) flags |= Qt::AlignHCenter;
    if (index.column() == 2) flags |= Qt::TextWordWrap;

    QRect rect = option.rect;
    //rect.setY(option.rect.y() + 4);

    QApplication::style()->drawItemText(painter, rect, flags,
                                        option.palette, true, text);

    painter->restore();
}


bool LogFieldDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    return false;
}

void LogFieldDelegate::updateEditorGeometry(QWidget* aEditor, const QStyleOptionViewItem& aOption, const QModelIndex& aIndex) const
{

}
