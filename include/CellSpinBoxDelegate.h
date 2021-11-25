#pragma once

#include <QStyledItemDelegate>

class CellSpinBoxDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit CellSpinBoxDelegate(QObject* aParent = nullptr);

    QWidget* createEditor(QWidget* aParent, const QStyleOptionViewItem& aOption,
                          const QModelIndex& aIndex) const override;

    void setEditorData(QWidget* aEditor, const QModelIndex& aIndex) const override;
    void setModelData(QWidget* aEditor, QAbstractItemModel* aModel,
                      const QModelIndex& aIndex) const override;

    void updateEditorGeometry(QWidget* aEditor, const QStyleOptionViewItem& aOption,
                              const QModelIndex& aIndex) const override;


    int minimum() const {return  m_min;}
    void setMinimum(int min) { m_min = min; }

    int maximum() const { return m_max; }
    void setMaximum(int max) { m_max = max; }

    int singleStep() { return  m_step; }
    void setSingleStep(int step) { m_step = step; }

    const QString& suffix() const { return  m_suffix; }
    void setSuffix(const QString& suffix) { m_suffix = suffix; }

    const QString &specialValueText() const { return m_special_text; }
    void setSpecialValueText(const QString& text) { m_special_text = text; }

private:
    int m_min;
    int m_max;
    int m_step;
    QString m_suffix;
    QString m_special_text;
};
