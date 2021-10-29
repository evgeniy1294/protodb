#pragma once

#include <QStyledItemDelegate>



class SpinBoxDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit SpinBoxDelegate(QObject* aParent = nullptr);

    QWidget* createEditor(QWidget* aParent, const QStyleOptionViewItem& aOption,
                          const QModelIndex& aIndex) const override;

    void setEditorData(QWidget* aEditor, const QModelIndex& aIndex) const override;
    void setModelData(QWidget* aEditor, QAbstractItemModel* aModel,
                      const QModelIndex& aIndex) const override;

    void updateEditorGeometry(QWidget* aEditor, const QStyleOptionViewItem& aOption,
                              const QModelIndex& aIndex) const override;

};
