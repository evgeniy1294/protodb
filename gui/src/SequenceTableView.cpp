#include <QHeaderView>
#include <protodb/SequenceModel.h>
#include <protodb/SequenceTableView.h>
#include <protodb/CellSpinBoxDelegate.h>
#include <protodb/CellCheckBoxDelegate.h>
#include <protodb/CellButtonDelegate.h>

SequenceTableView::SequenceTableView(QWidget *parent)
{
    m_spinbox_delegate = new CellSpinBoxDelegate(this);
        m_spinbox_delegate->setMinimum(0);
        m_spinbox_delegate->setMaximum(10'000);
        m_spinbox_delegate->setSingleStep(100);
        m_spinbox_delegate->setSuffix(tr("ms"));

    m_checkbox_delegate = new CellCheckBoxDelegate(this);
        m_checkbox_delegate->setText("");

    m_btn_delegate = new CellButtonDelegate(this);
        m_btn_delegate->setIcon(QIcon(":/icons/arrow.svg"));
        m_btn_delegate->setAlternateIcon(QIcon(":/icons/stop_rect.svg"));
        m_btn_delegate->setCheckable(true);
        m_btn_delegate->setFlat(true);

    setAlternatingRowColors(true);
    setStyleSheet("alternate-background-color: #eff0f1");
    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    auto vh = verticalHeader();
        vh->setSectionResizeMode(QHeaderView::Fixed);
}

void SequenceTableView::setSequenceModel(QAbstractItemModel *model, bool incomingMode)
{
    QTableView::setModel(model);
        setItemDelegateForColumn(SequenceModel::kColumnPeriod, m_spinbox_delegate);
        setColumnWidth(SequenceModel::kColumnActiveFlag, 0);
        hideColumn(SequenceModel::kColumnDescription);
        hideColumn(SequenceModel::kColumnDsl);

    QHeaderView* hh = horizontalHeader();
        hh->setSectionResizeMode(SequenceModel::kColumnName, QHeaderView::Stretch);
        hh->setSectionResizeMode(SequenceModel::kColumnBindedName, QHeaderView::Stretch);
        hh->setSectionResizeMode(SequenceModel::kColumnPeriod, QHeaderView::Fixed);
        hh->setSectionResizeMode(SequenceModel::kColumnActiveFlag, QHeaderView::ResizeToContents);

    if (incomingMode) {
        m_spinbox_delegate->setSpecialValueText(tr("No Delay"));
        showColumn(SequenceModel::kColumnBindedName);
        setItemDelegateForColumn(SequenceModel::kColumnActiveFlag, m_checkbox_delegate);
    }
    else
    {
        m_spinbox_delegate->setSpecialValueText(tr("No Repeat"));
        hideColumn(SequenceModel::kColumnBindedName);
        setItemDelegateForColumn(SequenceModel::kColumnActiveFlag, m_btn_delegate);
    }

    return;
}
