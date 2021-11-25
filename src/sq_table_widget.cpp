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

#include "CellButtonDelegate.h"
#include "CellSpinBoxDelegate.h"
#include "CellCheckBoxDelegate.h"
#include "SequenceModel.h"
#include "sq_table_widget.h"
#include "sq_table_dialog.h"

SqTableWidget::SqTableWidget(DisplayMode mode, QWidget* parent)
    : QWidget(parent)
    , m_mode(mode)
{
    createActions();
    createGui();
    createConnections();
}


void SqTableWidget::createActions()
{
    mRemoveAct = new QAction(QIcon(":/icons/close.svg"), tr("&Remove"), this);
        mRemoveAct->setStatusTip(tr("Remove selected macroses"));

    mCopyAct = new QAction(QIcon(":/icons/copy.svg"), tr("Copy"), this);
    mEditAct = new QAction(QIcon(":/icons/edit.svg"), tr("Edit"), this);

}

void SqTableWidget::createGui()
{
    // ---------[BUTTONS]---------- //
    mAddBtn = new QPushButton();
        mAddBtn->setIcon(QIcon(":/icons/add.svg"));
        mAddBtn->setToolTip("Add sequence");

    mRmBtn = new QPushButton();
        mRmBtn->setIcon(QIcon(":/icons/delete_minus.svg"));
        mRmBtn->setToolTip("Delete selected");

    mClrBtn = new QPushButton();
        mClrBtn->setIcon(QIcon(":/icons/delete_cross.svg"));
        mClrBtn->setToolTip("Delete all");

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
    mSqModel = new SequenceModel(this);

    auto cell_spinbox = new CellSpinBoxDelegate();
        cell_spinbox->setMinimum(0);
        cell_spinbox->setMaximum(10'000);
        cell_spinbox->setSingleStep(100);
        cell_spinbox->setSuffix(tr("ms"));

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
        mFilter->setFilterKeyColumn(SequenceModel::kColumnName);
        mFilter->setSourceModel(mSqModel);

    mTblView = new QTableView();
        cell_spinbox->setParent(mTblView);
        mTblView->setAlternatingRowColors(true);
        mTblView->setStyleSheet("alternate-background-color: #eff0f1");
        mTblView->setModel(mFilter);
        mTblView->setContextMenuPolicy(Qt::CustomContextMenu);
        mTblView->setItemDelegateForColumn(SequenceModel::kColumnPeriod, cell_spinbox);

        mTblView->setSelectionBehavior(QAbstractItemView::SelectRows);
        mTblView->setColumnWidth(SequenceModel::kColumnActiveFlag, 0);
        mTblView->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        mTblView->hideColumn(SequenceModel::kColumnDescription);
        mTblView->hideColumn(SequenceModel::kColumnDsl);

        auto headerView = mTblView->horizontalHeader();
            headerView->setSectionResizeMode(SequenceModel::kColumnName,       QHeaderView::Stretch);
            headerView->setSectionResizeMode(SequenceModel::kColumnBindedName, QHeaderView::Stretch);
            headerView->setSectionResizeMode(SequenceModel::kColumnPeriod,     QHeaderView::Fixed);
            headerView->setSectionResizeMode(SequenceModel::kColumnActiveFlag, QHeaderView::ResizeToContents);

    if (m_mode == kIncomingDisplayMode) {
        auto cell_checkbox = new CellCheckBoxDelegate();
            cell_checkbox->setText("");

        cell_spinbox->setSpecialValueText(tr("No Delay"));

        mSqModel->setIncomingMode();
        mTblView->setItemDelegateForColumn(SequenceModel::kColumnActiveFlag, cell_checkbox);
    }
    else
    {
        auto cell_button = new CellButtonDelegate();
            cell_button->setIcon(QIcon(":/icons/arrow.svg"));
            cell_button->setAlternateIcon(QIcon(":/icons/stop_rect.svg"));
            cell_button->setCheckable(true);
            cell_button->setFlat(true);

        cell_spinbox->setSpecialValueText(tr("No Repeat"));

        mSqModel->setOutgoingMode();
        mTblView->hideColumn(SequenceModel::kColumnBindedName);
        mTblView->setItemDelegateForColumn(SequenceModel::kColumnActiveFlag, cell_button);
    }

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

    connect(mClrBtn, &QPushButton::released, this, &SqTableWidget::onClickClear);

    connect(mRmBtn, &QPushButton::released, this, &SqTableWidget::onClickRemove);

    // ---------[FILTER]---------- //
    connect(mFindLe, &QLineEdit::textChanged, mFilter, &QSortFilterProxyModel::setFilterFixedString);


    // ---------[ACTIONS]---------- //
    connect(mRemoveAct, &QAction::triggered, this, &SqTableWidget::onClickRemove);

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

        if (index.column() == SequenceModel::kColumnName) {
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

        if (row_list.count() > 0) {
            auto index =mFilter->mapToSource(row_list.first());

            for (int i = 0; i < row_list.count(); i++) {
               mSqModel->removeRow( index.row() );
            }

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

