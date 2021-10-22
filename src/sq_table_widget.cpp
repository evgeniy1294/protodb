#include <QAction>
#include <QLayout>
#include <QHeaderView>
#include <QDataWidgetMapper>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QTableView>
#include <QMenu>
#include <QSortFilterProxyModel>

#include <iostream>

#include "singleton.h"
#include "sq_model.h"
#include "sq_table_widget.h"
#include "sq_table_widget_private.h"
#include "sq_table_dialog.h"

SqTableWidget::SqTableWidget(QWidget* parent)
{
    createActions();
    createGui();
    createConnections();
}


void SqTableWidget::createActions()
{
    mRemoveAct = new QAction(QIcon(":/icons/close.svg"), tr("&Remove"), this);
        mRemoveAct->setStatusTip(tr("Remove selected macroses"));

    mClearAct = new QAction(QIcon(":/icons/trash.svg"), tr("&Remove All"), this);
        mClearAct->setStatusTip(tr("Clear Macro Table"));

    mCopyAct = new QAction(tr("Copy"));
    mEditAct = new QAction(tr("Edit"));

}

void SqTableWidget::createGui()
{
    // ---------[BUTTONS]---------- //
    mAddBtn = new QPushButton();
        mAddBtn->setIcon(QIcon(":/icons/add.svg"));

    mRmBtn = new QPushButton();
        mRmBtn->setIcon(QIcon(":/icons/delete_minus.svg"));

    mClrBtn = new QPushButton();
        mClrBtn->setIcon(QIcon(":/icons/delete_cross.svg"));

    mBtnDelegate = new ButtonDelegate();

    // ---------[MENU]---------- //
    mMenu = new QMenu;
        mMenu->addAction(mCopyAct);
        mMenu->addAction(mEditAct);
        mMenu->addSeparator();
        mMenu->addAction(mRemoveAct);

    // ---------[FIND LINE EDIT]---------- //
    mFindLe = new QLineEdit();
        mFindLe->setPlaceholderText(tr("Find sequence"));
        mFindLe->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);


    // ---------[TABLE VIEW]---------- //
    mSqModel = new SqModel(this);
        mSqModel->setStorage(&Singleton::instance().mSequenceStorage);

    mMapper = new QDataWidgetMapper(this);
        mMapper->setModel(mSqModel);
        mMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        mMapper->setCurrentIndex(0);

    mDialog = new SqTableDialog();
        mDialog->setMapper(mMapper);
        mDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        mDialog->wipe();

    mFilter = new QSortFilterProxyModel(this);
        mFilter->setFilterCaseSensitivity(Qt::CaseInsensitive);
        mFilter->setDynamicSortFilter(true);
        mFilter->setFilterKeyColumn(SqModel::kColumnSqName);
        mFilter->setSourceModel(mSqModel);

    mTblView = new QTableView();
        mTblView->setModel(mFilter);
        mTblView->setContextMenuPolicy(Qt::CustomContextMenu);
        mTblView->setItemDelegateForColumn(SqModel::kColumnRepeatTime, new SpinBoxDelegate(mTblView));
        mTblView->setItemDelegateForColumn(SqModel::kColumnSendBtn, mBtnDelegate);
        mTblView->setColumnWidth(SqModel::kColumnSendBtn, 0);
        mTblView->hideColumn(SqModel::kColumnDescription);
        mTblView->hideColumn(SqModel::kColumnCharStr);
        mTblView->setSelectionBehavior(QAbstractItemView::SelectRows);
        auto headerView = mTblView->horizontalHeader();
            headerView->setSectionResizeMode(SqModel::kColumnSqName,     QHeaderView::Stretch);
            headerView->setSectionResizeMode(SqModel::kColumnTrigName,   QHeaderView::Stretch);
            headerView->setSectionResizeMode(SqModel::kColumnRepeatTime, QHeaderView::Fixed);
            headerView->setSectionResizeMode(SqModel::kColumnSendBtn,    QHeaderView::Fixed);

    // ---------[LAYOUT]---------- //
    auto h_layout = new QHBoxLayout();

        h_layout->addWidget(mAddBtn);
        h_layout->addWidget(mRmBtn);
        h_layout->addWidget(mClrBtn);
        h_layout->addWidget(mFindLe);

    auto main_layout = new QGridLayout();
        main_layout->addLayout(h_layout, 0, 0);
        main_layout->addWidget(mTblView, 1, 0);

        setLayout(main_layout);
}

void SqTableWidget::createConnections()
{
    // ---------[BUTTONS]---------- //
    connect(mAddBtn, &QPushButton::released, this, [this](){mMapper->toLast(); mDialog->show();});
    connect(mRmBtn, &QPushButton::released, this, &SqTableWidget::onClickRemove);
    connect(mClrBtn, &QPushButton::released, this, &SqTableWidget::onClickClear);
    connect(mBtnDelegate, &ButtonDelegate::triggered, this, [this](const QModelIndex& a_index) {
        auto index = mFilter->mapToSource(a_index);
        mSqModel->onSendSequence(index);
    });


    // ---------[FILTER]---------- //
    connect(mFindLe, &QLineEdit::textChanged, mFilter, &QSortFilterProxyModel::setFilterFixedString);


    // ---------[ACTIONS]---------- //
    connect(mRemoveAct, &QAction::triggered, this, &SqTableWidget::onClickRemove);

    connect(mClearAct, &QAction::triggered, this, &SqTableWidget::onClickClear);

    connect(mEditAct, &QAction::triggered, this, [this]() {
        auto row_list = mTblView->selectionModel()->selectedRows();

        if (row_list.size() > 0) {  
            auto index = mFilter->mapToSource( row_list.last() );
            mMapper->setCurrentIndex(index.row()); mDialog->show();
        }
    });

    // ---------[MAPPER]---------- //
    connect(mMapper, &QDataWidgetMapper::currentIndexChanged, mTblView, &QTableView::selectRow);


    // ---------[TABLE]---------- //
    connect(mTblView, &QTableView::doubleClicked, this, [this](const QModelIndex &a_index) {
        auto index = mFilter->mapToSource(a_index);

        if (index.column() == SqModel::kColumnSqName) {
            mMapper->setCurrentIndex(index.row()); mDialog->show();
        }
    });
    connect(mTblView, &QTableView::customContextMenuRequested, this, [this](const QPoint &pos) {
        auto index = mFilter->mapToSource(mTblView->indexAt(pos));

        if (index.row() >= 0) {
            mMenu->popup(mTblView->viewport()->mapToGlobal(pos));
        }
    });
}



void SqTableWidget::onClickRemove()
{
    QMessageBox msgbox;
    {
        msgbox.setText( tr("Delete selected?") );
        msgbox.setInformativeText( tr("Selected sequence will be removed") );
        msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgbox.setDefaultButton(QMessageBox::No);
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.setFixedSize( QSize(480, 240) );

            msgbox.setButtonText(QMessageBox::Yes, QObject::tr("Yes") );
            msgbox.setButtonText(QMessageBox::No, QObject::tr("No") );
    }

    if (msgbox.exec() == QMessageBox::Yes) {
        auto row_list = mTblView->selectionModel()->selectedRows();

        if (!row_list.empty()) {
            mSqModel->removeRows( row_list.constFirst().row(), row_list.count() );

            if (mSqModel->rowCount() == 0) {
                mDialog->wipe();
            }
            else
            {
                mMapper->toLast();
            }
        }
    }
}

void SqTableWidget::onClickClear()
{
    QMessageBox msgbox;
    {
        msgbox.setText( tr("Clear Table?") );
        msgbox.setInformativeText( tr("All sequence will be removed") );
        msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgbox.setDefaultButton(QMessageBox::No);
        msgbox.setIcon(QMessageBox::Icon::Warning);
        msgbox.setFixedSize( QSize(680, 240) );

            msgbox.setButtonText(QMessageBox::Yes, QObject::tr("Yes") );
            msgbox.setButtonText(QMessageBox::No, QObject::tr("No") );
    }

    if (msgbox.exec() == QMessageBox::Yes) {
        mSqModel->removeRows( 0, mSqModel->rowCount() );
        mDialog->wipe();
    }
}

