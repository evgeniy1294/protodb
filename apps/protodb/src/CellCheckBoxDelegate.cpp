#include "CellCheckBoxDelegate.h"

#include <QApplication>


CellCheckBoxDelegate::CellCheckBoxDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
    , m_active_row(-1)
{

}

void CellCheckBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    bool check = index.model()->data(index, Qt::DisplayRole).toBool();

    QStyleOptionButton box;
    box.rect        = option.rect;
    box.styleObject = option.styleObject;
    box.state       = option.state | QStyle::State_Enabled;
    box.text        = m_text;
    box.direction   = Qt::RightToLeft;

    box.state |= (check) ? QStyle::State_On : QStyle::State_Off;

    //if (option.state & QStyle::State_MouseOver) {
        box.state |= ((m_active_row == index.row()) ? QStyle::State_Sunken : QStyle::State_None);
    //}
    QApplication::style()->drawControl( QStyle::CE_CheckBox, &box, painter);
}

bool CellCheckBoxDelegate::editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index)
{
    switch(event->type()) {
        case QEvent::MouseButtonPress:
            m_active_row  = index.row();
            break;

        case QEvent::MouseButtonDblClick:
            [[fallthrough]];
        case QEvent::MouseButtonRelease:
            m_active_row = -1;
            model->setData(index, true, kClickRole);
            break;

        default:
            break;
    };

    return true;
}
