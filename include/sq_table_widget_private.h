#pragma once

#include <QItemDelegate>
#include <QStyledItemDelegate>

class ButtonDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    ButtonDelegate(QObject *parent = 0);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

signals:
    void triggered(const QModelIndex &index);

private:
    QStyle::State mState;
    int   activeRow;
};


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
