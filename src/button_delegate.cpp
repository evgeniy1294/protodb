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
     QRect r = option.rect;

     QStyleOptionButton btn;
     btn.rect     = QRect(r.left(), r.top(), r.width(), r.height());
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
             activeRect = option.rect;
             mState = QStyle::State_Sunken;
             break;

         case QEvent::MouseButtonDblClick:
         case QEvent::MouseButtonRelease: {
             if ( activeRow == index.row()) {
                 emit triggered(index);
             }
             mState = QStyle::State_Raised;
             activeRow = -1;

         } break;

         default:
              return false;
     };

     return true;
 }


