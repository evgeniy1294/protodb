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

#include "SequenceTableWidget.h"
#include "sq_table_dialog.h"
#include "SequenceTableView.h"
#include "SequenceModel.h"

SequenceTableWidget::SequenceTableWidget(QWidget* parent)
    : QWidget(parent)
    , m_model(nullptr)
    , m_mapper(new QDataWidgetMapper(this))
    , m_search_model(new QSortFilterProxyModel(this))
{
    createActions();
    createGui();
    createConnections();
}

SequenceTableWidget::SequenceTableWidget(SequenceModel *model, QWidget *parent)
    : QWidget(parent)
    , m_mapper(new QDataWidgetMapper(this))
    , m_search_model(new QSortFilterProxyModel(this))
{
    createActions();
    createGui();
    createConnections();

    setSequenceModel(model);
}

void SequenceTableWidget::setSequenceModel(SequenceModel *model)
{
    m_model = model;
    m_search_model->setSourceModel(model);
    m_mapper->setModel(model);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
        m_mapper->setCurrentIndex(0);
    m_view->setSequenceModel(m_search_model, model->isModeIncoming());
    m_edit_dialog->setMapper(m_mapper);
    m_edit_dialog->wipe();
}

SequenceModel* SequenceTableWidget::sequenceModel() const
{
    return m_model;
}


void SequenceTableWidget::createActions()
{
    m_rm_act = new QAction(QIcon(":/icons/close.svg"), tr("&Remove"), this);
        m_rm_act->setStatusTip(tr("Remove selected macroses"));

    m_cp_act = new QAction(QIcon(":/icons/copy.svg"), tr("Copy"), this);
    m_ed_act = new QAction(QIcon(":/icons/edit.svg"), tr("Edit"), this);

}

void SequenceTableWidget::createGui()
{
    // ---------[BUTTONS]---------- //
    m_add_btn = new QPushButton();
        m_add_btn->setIcon(QIcon(":/icons/add.svg"));
        m_add_btn->setToolTip("Add sequence");

    m_rm_btn = new QPushButton();
        m_rm_btn->setIcon(QIcon(":/icons/delete_minus.svg"));
        m_rm_btn->setToolTip("Delete selected");

    m_clr_btn = new QPushButton();
        m_clr_btn->setIcon(QIcon(":/icons/delete_cross.svg"));
        m_clr_btn->setToolTip("Delete all");

    // ---------[MENU]---------- //
    m_menu = new QMenu;
        m_menu->addAction(m_cp_act);
        m_menu->addAction(m_ed_act);
        m_menu->addSeparator();
        m_menu->addAction(m_rm_act);

    // ---------[FIND LINE EDIT]---------- //
    m_search_line = new QLineEdit();
        m_search_line->setPlaceholderText(tr("Find sequence"));
        m_search_line->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);


    // ---------[TABLE VIEW]---------- //
    m_view = new SequenceTableView();

    m_edit_dialog = new SqTableDialog();
        m_edit_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        m_edit_dialog->wipe();

    m_view = new SequenceTableView();

    // ---------[LAYOUT]---------- //
    auto h_layout = new QHBoxLayout();

        h_layout->addWidget(m_add_btn);
        h_layout->addWidget(m_rm_btn);
        h_layout->addWidget(m_clr_btn);
        h_layout->addWidget(m_search_line);

    auto main_layout = new QGridLayout();
        main_layout->addLayout(h_layout, 0, 0);
        main_layout->addWidget(m_view, 1, 0);

        setLayout(main_layout);
}

void SequenceTableWidget::createConnections()
{
    // ---------[BUTTONS]---------- //
    connect(m_add_btn, &QPushButton::released, this, [this](){m_mapper->toLast(); m_edit_dialog->show();});

    connect(m_clr_btn, &QPushButton::released, this, &SequenceTableWidget::onClickClear);

    connect(m_rm_btn, &QPushButton::released, this, &SequenceTableWidget::onClickRemove);

    // ---------[FILTER]---------- //
    connect(m_search_line, &QLineEdit::textChanged, m_search_model, &QSortFilterProxyModel::setFilterFixedString);


    // ---------[ACTIONS]---------- //
    connect(m_rm_act, &QAction::triggered, this, &SequenceTableWidget::onClickRemove);

    connect(m_ed_act, &QAction::triggered, this, [this]() {
        auto row_list = m_view->selectionModel()->selectedRows();

        if (row_list.size() > 0) {  
            auto index = m_search_model->mapToSource( row_list.last() );
            m_mapper->setCurrentIndex(index.row()); m_edit_dialog->show();
        }
    });

    // ---------[MAPPER]---------- //
    connect(m_mapper, &QDataWidgetMapper::currentIndexChanged, m_view, &QTableView::selectRow);


    // ---------[TABLE]---------- //
    connect(m_view, &QTableView::doubleClicked, this, [this](const QModelIndex &a_index) {
        auto index = m_search_model->mapToSource(a_index);

        if (index.column() == SequenceModel::kColumnName) {
            m_mapper->setCurrentIndex(index.row()); m_edit_dialog->show();
        }
    });
    connect(m_view, &QTableView::customContextMenuRequested, this, [this](const QPoint &pos) {
        auto index = m_search_model->mapToSource(m_view->indexAt(pos));

        if (index.row() >= 0) {
            m_menu->popup(m_view->viewport()->mapToGlobal(pos));
        }
    });
}



void SequenceTableWidget::onClickRemove()
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
        auto row_list = m_view->selectionModel()->selectedRows();

        if (row_list.count() > 0) {
            auto index =m_search_model->mapToSource(row_list.first());

            for (int i = 0; i < row_list.count(); i++) {
               m_model->removeRow( index.row() );
            }

            if (m_model->rowCount() == 0) {
                m_edit_dialog->wipe();
            }
            else
            {
                m_mapper->toLast();
            }
        }
    }
}

void SequenceTableWidget::onClickClear()
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
        m_model->removeRows( 0, m_model->rowCount() );
        m_edit_dialog->wipe();
    }
}

