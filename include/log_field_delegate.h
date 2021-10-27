#pragma once

#include <QStyledItemDelegate>

class LogFieldDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit LogFieldDelegate(QObject* aParent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;
    void updateEditorGeometry(QWidget* aEditor, const QStyleOptionViewItem& aOption,
                              const QModelIndex& aIndex) const override;
};



