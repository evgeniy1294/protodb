#include <QAction>
#include <QLayout>
#include <QHeaderView>

#include "singleton.h"
#include "sq_model.h"
#include "sq_table_widget.h"
#include "spinbox_delegate.h"




SqTableWidget::SqTableWidget(QWidget* parent)
{
    createActions();
    createGui();
}

void SqTableWidget::onClickNew()
{

}

void SqTableWidget::onClickRemove()
{

}

void SqTableWidget::onClickClear()
{

}

void SqTableWidget::createActions()
{
    mNewAct = new QAction(QIcon(":/icons/add.svg"), tr("&Add"), this);
        mNewAct->setStatusTip(tr("Create new macros"));
        connect(mNewAct, &QAction::triggered, this, &SqTableWidget::onClickNew);

    mRemoveAct = new QAction(QIcon(":/icons/close.svg"), tr("&Remove"), this);
        mRemoveAct->setStatusTip(tr("Remove selected macroses"));
        connect(mRemoveAct, &QAction::triggered, this, &SqTableWidget::onClickRemove);

    mClearAct = new QAction(QIcon(":/icons/trash.svg"), tr("&Clear"), this);
        mClearAct->setStatusTip(tr("Clear Macro Table"));
        connect(mClearAct, &QAction::triggered, this, &SqTableWidget::onClickClear);
}

void SqTableWidget::createGui()
{
    mToolBar = new isa_tool_bar( QBoxLayout::LeftToRight );
        mToolBar->addToolAction(mNewAct);
        mToolBar->addToolAction(mRemoveAct);
        mToolBar->addMenuSeparator();
        mToolBar->addToolAction(mClearAct, false);

    auto mSqModel = new SqModel();
        mSqModel->setStorage(&Singleton::instance().mSequenceStorage);

    auto mTblView = new QTableView();
        mTblView->setModel(mSqModel);

        mTblView->setItemDelegateForColumn(SqModel::kColumnRepeatTime, new SpinBoxDelegate(mTblView));
        auto headerView = mTblView->horizontalHeader();
            headerView->setSectionResizeMode(SqModel::kColumnSqName,     QHeaderView::Stretch);
            headerView->setSectionResizeMode(SqModel::kColumnTrigName,   QHeaderView::Stretch);
            headerView->setSectionResizeMode(SqModel::kColumnRepeatTime, QHeaderView::Fixed);
            headerView->setSectionResizeMode(SqModel::kColumnSendBtn,    QHeaderView::Fixed);

    auto layout = new QVBoxLayout();
        layout->addWidget(mToolBar);
        layout->addWidget(mTblView);

    setLayout(layout);
}
