#include <QSpinBox>
#include "spinbox_delegate.h"

SpinBoxDelegate::SpinBoxDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
{

}


QWidget* SpinBoxDelegate::createEditor(QWidget* aParent, const QStyleOptionViewItem& aOption, const QModelIndex& aIndex) const
{
    auto editor = new QSpinBox(aParent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(10000);
    editor->setSingleStep(100);
    editor->setSuffix("ms");
    editor->setSpecialValueText(QObject::tr("No Repeat"));

    return editor;
}

void SpinBoxDelegate::setEditorData(QWidget* aEditor, const QModelIndex& aIndex) const
{
    int value = aIndex.model()->data(aIndex, Qt::EditRole).toInt();
    auto spinBox = static_cast<QSpinBox*>(aEditor);
    spinBox->setValue(value);
}

void SpinBoxDelegate::setModelData(QWidget* aEditor, QAbstractItemModel* aModel, const QModelIndex& aIndex) const
{
    auto spinBox = static_cast<QSpinBox*>(aEditor);
    spinBox->interpretText();
    aModel->setData(aIndex, spinBox->value(), Qt::EditRole);
}

void SpinBoxDelegate::updateEditorGeometry(QWidget* aEditor, const QStyleOptionViewItem& aOption, const QModelIndex& aIndex) const
{
    aEditor->setGeometry(aOption.rect);
}
