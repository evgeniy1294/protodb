#pragma once

#include <QStyledItemDelegate>
#include "LogTableModel.h"

class LogItemDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LogItemDelegate(QObject* aParent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};



