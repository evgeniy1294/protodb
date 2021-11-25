#include <QHeaderView>
#include "SequenceModel.h"
#include "SequenceTableView.h"
#include "CellSpinBoxDelegate.h"
#include "CellCheckBoxDelegate.h"
#include "CellButtonDelegate.h"

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

     //   setItemDelegateForColumn(SequenceModel::kColumnPeriod, cell_spinbox);

     //   mTblView->setColumnWidth(SequenceModel::kColumnActiveFlag, 0);
     //   mTblView->hideColumn(SequenceModel::kColumnDescription);
     //   mTblView->hideColumn(SequenceModel::kColumnDsl);

     //   auto headerView = mTblView->horizontalHeader();
     //       headerView->setSectionResizeMode(SequenceModel::kColumnName,       QHeaderView::Stretch);
     //       headerView->setSectionResizeMode(SequenceModel::kColumnBindedName, QHeaderView::Stretch);
     //       headerView->setSectionResizeMode(SequenceModel::kColumnPeriod,     QHeaderView::Fixed);
     //       headerView->setSectionResizeMode(SequenceModel::kColumnActiveFlag, QHeaderView::ResizeToContents);
}

void SequenceTableView::setModel(QAbstractItemModel *model)
{
    if (dynamic_cast<SequenceModel*>(model)) {
        QTableView::setModel(model);
        QHeaderView* hh = horizontalHeader();

    }
}
