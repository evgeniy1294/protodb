#include <QSpinBox>
#include "CellSpinBoxDelegate.h"

CellSpinBoxDelegate::CellSpinBoxDelegate(QObject* aParent)
    : QStyledItemDelegate(aParent)
    , m_min(0)
    , m_max(100)
    , m_step(1)
    , m_suffix()
    , m_special_text()
{

}


QWidget* CellSpinBoxDelegate::createEditor(QWidget* aParent, const QStyleOptionViewItem& aOption, const QModelIndex& aIndex) const
{
    auto editor = new QSpinBox(aParent);
    editor->setFrame(false);
    editor->setMinimum(m_min);
    editor->setMaximum(m_max);
    editor->setSingleStep(m_step);
    editor->setSuffix(m_suffix);
    editor->setSpecialValueText(m_special_text);

    return editor;
}

void CellSpinBoxDelegate::setEditorData(QWidget* aEditor, const QModelIndex& aIndex) const
{
    int value = aIndex.model()->data(aIndex, Qt::EditRole).toInt();
    auto spinBox = static_cast<QSpinBox*>(aEditor);
    spinBox->setValue(value);
}

void CellSpinBoxDelegate::setModelData(QWidget* aEditor, QAbstractItemModel* aModel, const QModelIndex& aIndex) const
{
    auto spinBox = static_cast<QSpinBox*>(aEditor);
    spinBox->interpretText();
    aModel->setData(aIndex, spinBox->value(), Qt::EditRole);
}

void CellSpinBoxDelegate::updateEditorGeometry(QWidget* aEditor, const QStyleOptionViewItem& aOption, const QModelIndex& aIndex) const
{
    aEditor->setGeometry(aOption.rect);
}
