#include "LogTableView.h"
#include "LogDecorationDialog.h"
#include "LogItemDelegate.h"

#include "Logger.h"

#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMenu>
#include <QAction>

LogTableView::LogTableView(QWidget *parent)
    : QTableView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    m_item_delegate = new LogItemDelegate();
    setItemDelegate(m_item_delegate);
    setWordWrap(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(false);

    QHeaderView* vh = verticalHeader();
        vh->setSectionResizeMode(QHeaderView::ResizeToContents);
        vh->hide();

    QHeaderView* hh = horizontalHeader();
        hh->hide();

    m_dec_dialog = new LogDecorationDialog(this);

    createActions();
    createMenu();
    connectSignals();
}


void LogTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    QHeaderView* hh = horizontalHeader();
        hh->setSectionResizeMode(Logger::kColumnTimestamp, QHeaderView::ResizeToContents);
        hh->setSectionResizeMode(Logger::kColumnChannel,   QHeaderView::ResizeToContents);
        hh->setSectionResizeMode(Logger::kColumnMsg,       QHeaderView::Stretch);
}

void LogTableView::setByteFormat(Logger::ByteFormat format)
{
    m_item_delegate->setByteFormat(format);
}

void LogTableView::createMenu()
{
    m_base_menu = new QMenu();
        m_base_menu->addAction(m_options);

    m_data_channel_menu = new QMenu();
        m_data_channel_menu->addAction(m_add_to_analyzer);
        m_data_channel_menu->addAction(m_copy);
        m_data_channel_menu->addAction(m_copy_as_bytes);
        m_data_channel_menu->addAction(m_copy_as_string);
        m_data_channel_menu->addSeparator();
        m_data_channel_menu->addAction(m_options);

    m_info_channel_menu = new QMenu();
        m_info_channel_menu->addAction(m_copy);
        m_info_channel_menu->addAction(m_copy_message);
        m_info_channel_menu->addSeparator();
        m_info_channel_menu->addAction(m_options);
}

void LogTableView::createActions()
{
    m_copy = new QAction(tr("Copy"));
    m_copy_message = new QAction(tr("Copy Message"));
    m_copy_as_bytes = new QAction(tr("Copy as Bytes"));
    m_copy_as_string = new QAction(tr("Copy as String"));
    m_add_to_analyzer = new QAction(tr("Add to Analyzer"));
    m_options = new QAction(tr("Options"));
}

void LogTableView::connectSignals()
{
    connect(this, &QTableView::customContextMenuRequested, this, [this](QPoint pos) {
        auto menu = m_base_menu;
        auto row  = indexAt(pos).row();

        if (row != -1) {
            auto channel = model()->data( model()->index( row, Logger::kColumnChannel ) ).toInt();

            menu = (channel == Logger::kChannelFirst || channel == Logger::kChannelSecond) ?
                m_data_channel_menu : m_info_channel_menu;
        }

        menu->popup(viewport()->mapToGlobal(pos));
    });

    connect(m_copy, &QAction::triggered, this, [this]() {
        QClipboard* pcb = QApplication::clipboard();
            pcb->setText( m_item_delegate->message( currentIndex() ) );
    });

    connect(m_copy_message, &QAction::triggered, this, [this]() {
        auto msg = currentIndex().data().toString();

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_copy_as_bytes, &QAction::triggered, this, [this]() {
        auto msg = currentIndex().data().toByteArray().toHex(m_item_delegate->separator());

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_copy_as_string, &QAction::triggered, this, [this]() {
        auto msg = currentIndex().data(Qt::DisplayRole).toString();

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_add_to_analyzer, &QAction::triggered, this, [this]() {
        auto channel = model()->data( model()->index( currentIndex().row(), Logger::kColumnChannel ) ).toInt();

        if (channel == Logger::kChannelFirst || channel == Logger::kChannelSecond) {

            auto data = currentIndex().data();
            if (!data.isNull()) {
                emit sToAnalyzer(data.toByteArray());
            }
        }
    });

    connect(m_options, &QAction::triggered, this, [this]() {
        m_dec_dialog->show();
    });
}

