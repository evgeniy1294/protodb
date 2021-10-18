#include <iostream>

#include <QItemDelegate>
#include <QtGui>
#include <QApplication>
#include <QDialog>

#include "sq_table_widget_private.h"


ButtonDelegate::ButtonDelegate(QObject *parent)
     : QItemDelegate(parent)
     , mState(QStyle::State_Enabled)
     , activeRow(-1)
{
}



void ButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionButton btn;
    btn.rect     = option.rect;
    btn.icon     = QIcon(":/icons/arrow.svg");
    btn.features = QStyleOptionButton::Flat;

    if (option.state & QStyle::State_MouseOver) {
        btn.state  = (activeRow == index.row()) ? mState : QStyle::State_Raised;
    }

    btn.state |= QStyle::State_Enabled;

    QApplication::style()->drawControl( QStyle::CE_PushButton, &btn, painter);
}



bool ButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    switch(event->type()) {
        case QEvent::MouseButtonPress:
            activeRow  = index.row();
            mState = QStyle::State_Sunken;
            break;

        case QEvent::MouseButtonDblClick:
        case QEvent::MouseButtonRelease: {
            activeRow = -1;
            mState = QStyle::State_Raised;

            emit triggered(index);

        } break;

        default:
             return false;
    };

    return true;
}


