#include <QAction>
#include <QLayout>
#include <QHeaderView>
#include <QDataWidgetMapper>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>

#include "singleton.h"
#include "sq_model.h"
#include "sq_table_widget.h"
#include "sq_table_widget_private.h"
#include "sq_table_dialog.h"


SqTableWidget::SqTableWidget(QWidget* parent)
    : mMapper(new QDataWidgetMapper())
{
    createActions();
    createGui();
}


void SqTableWidget::onClickRemove()
{

}

void SqTableWidget::onClickClear()
{

}

void SqTableWidget::createActions()
{
    mRemoveAct = new QAction(QIcon(":/icons/close.svg"), tr("&Remove"), this);
        mRemoveAct->setStatusTip(tr("Remove selected macroses"));
        connect(mRemoveAct, &QAction::triggered, this, &SqTableWidget::onClickRemove);

    mClearAct = new QAction(QIcon(":/icons/trash.svg"), tr("&Remove All"), this);
        mClearAct->setStatusTip(tr("Clear Macro Table"));
        connect(mClearAct, &QAction::triggered, this, &SqTableWidget::onClickClear);
}

void SqTableWidget::createGui()
{
    auto h_layout = new QHBoxLayout();
        auto add_btn = new QPushButton();
            add_btn->setIcon(QIcon(":/icons/add.svg"));
            connect(add_btn, &QPushButton::released, this, [this](){mDialog->show();});


        auto rm_btn = new QToolButton();
            rm_btn->setIcon(QIcon(":/icons/close.svg"));
            rm_btn->addAction(mRemoveAct);
            rm_btn->addAction(mClearAct);

        auto find_le = new QLineEdit();
            find_le->setPlaceholderText(tr("Find sequence"));
            find_le->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);

        h_layout->addWidget(add_btn);
        h_layout->addWidget(rm_btn);
        h_layout->addWidget(find_le);

    mSqModel = new SqModel();
        mSqModel->setStorage(&Singleton::instance().mSequenceStorage);

    mMapper->setModel(mSqModel);
    mMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mMapper->setCurrentIndex(0);

    mDialog = new SqTableDialog();
        mDialog->setMapper(mMapper);

    auto btnDelegate = new ButtonDelegate();
        connect(btnDelegate, &ButtonDelegate::triggered, mSqModel, &SqModel::onSendSequence);

    auto mTblView = new QTableView();
        mSqModel->setParent(mTblView);
        mTblView->setModel(mSqModel);
        mTblView->setMouseTracking(true);

        connect(mMapper, &QDataWidgetMapper::currentIndexChanged, mTblView, &QTableView::selectRow);

        mTblView->setItemDelegateForColumn(SqModel::kColumnRepeatTime, new SpinBoxDelegate(mTblView));
        mTblView->setItemDelegateForColumn(SqModel::kColumnSendBtn, btnDelegate);
        mTblView->setColumnWidth(SqModel::kColumnSendBtn, 0);
        mTblView->hideColumn(SqModel::kColumnDescription);
        mTblView->hideColumn(SqModel::kColumnCharStr);
        auto headerView = mTblView->horizontalHeader();
            headerView->setSectionResizeMode(SqModel::kColumnSqName,     QHeaderView::Stretch);
            headerView->setSectionResizeMode(SqModel::kColumnTrigName,   QHeaderView::Stretch);
            headerView->setSectionResizeMode(SqModel::kColumnRepeatTime, QHeaderView::Fixed);
            headerView->setSectionResizeMode(SqModel::kColumnSendBtn,    QHeaderView::Fixed);

    auto main_layout = new QGridLayout();
        main_layout->addLayout(h_layout, 0, 0);
        main_layout->addWidget(mTblView, 1, 0);

    setLayout(main_layout);
}
