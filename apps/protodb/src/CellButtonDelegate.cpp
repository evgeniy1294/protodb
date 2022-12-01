#include "CellButtonDelegate.h"

#include <QApplication>
#include <QPushButton>
#include <QPainter>
#include <iostream>

using namespace protodb;

CellButtonDelegate::CellButtonDelegate(QObject *parent)
     : QStyledItemDelegate(parent)
     , m_active_row(-1)
     , m_flat(false)
     , m_checkable(false)
{
}



void CellButtonDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool check = index.model()->data(index, Qt::DisplayRole).toBool();

    QStyleOptionButton btn;
    btn.rect        = option.rect;
    btn.iconSize    = QSize(option.rect.width()*0.75, option.rect.height()*0.75);
    btn.styleObject = option.styleObject;
    btn.features   |= (m_flat) ? QStyleOptionButton::Flat : QStyleOptionButton::None;
    btn.state       = option.state | QStyle::State_Enabled;
    btn.text        = m_text;
    btn.icon = (check && m_checkable) ? m_alt_icon : m_icon;

    //if (option.state & QStyle::State_MouseOver) {
        btn.state |= ((m_active_row == index.row()) ? QStyle::State_Sunken : QStyle::State_None);
    //}

    QApplication::style()->drawControl( QStyle::CE_PushButton, &btn, painter);
}



bool CellButtonDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
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

void CellButtonDelegate::setIcon(const QIcon& icon)
{
    m_icon = icon;
}

void CellButtonDelegate::setAlternateIcon(const QIcon& icon)
{
    m_alt_icon = icon;
}

void CellButtonDelegate::setText(const QString& text)
{
    m_text = text;
}

void CellButtonDelegate::setCheckable(bool checkable)
{
    m_checkable = checkable;
}

bool CellButtonDelegate::isCheckable()
{
    return m_checkable;
}

void CellButtonDelegate::setFlat(bool flat)
{
    m_flat = flat;
}

bool CellButtonDelegate::isFlat()
{
    return m_flat;
}


