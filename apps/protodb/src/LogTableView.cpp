#include "Logger.h"
#include "LogFormatter.h"
#include "LogDecorator.h"
#include "LogTableView.h"
#include "LogItemDelegate.h"
#include "LogDecorationDialog.h"


#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QScrollBar>

using namespace protodb;

LogTableView::LogTableView(QWidget *parent)
    : QTableView(parent)
{
    m_decorator = new LogDecorator();
    m_formatter = new LogFormatter();

    m_item_delegate = new LogItemDelegate();
        m_item_delegate->setDecorator(m_decorator);
        m_item_delegate->setFormatter(m_formatter);

    setContextMenuPolicy(Qt::CustomContextMenu);
    setItemDelegate(m_item_delegate);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setShowGrid(false);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QHeaderView* vh = verticalHeader();
        vh->hide();

    QHeaderView* hh = horizontalHeader();
        hh->hide();

    m_dec_dialog = new LogDecorationDialog(this);

    createActions();
    createMenu();
    connectSignals();
}

LogTableView::~LogTableView()
{
    delete m_decorator;
    delete m_formatter;
}


void LogTableView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    QHeaderView* hh = horizontalHeader();
        hh->setSectionResizeMode(Logger::ColumnTimestamp, QHeaderView::Fixed);
        hh->setSectionResizeMode(Logger::ColumnChannel,   QHeaderView::Fixed);
        hh->setSectionResizeMode(Logger::ColumnMsg,       QHeaderView::Stretch);

    connect(model, &QAbstractItemModel::rowsInserted, this, [this](const QModelIndex &parent, int first, int last) {
        for (int i = first; i <= last; i++) {
            resizeRowToContents(i);
        }
    });
}

LogFormatter* LogTableView::formatter() const
{
    return m_formatter;
}

LogDecorator* LogTableView::decorator() const
{
    return m_decorator;
}

void LogTableView::setTimestampVisible(bool visible)
{
    if (visible)
        showColumn(Logger::ColumnTimestamp);
    else
        hideColumn(Logger::ColumnTimestamp);
}

void LogTableView::setChannelNameVisible(bool visible)
{
    if (visible)
        showColumn(Logger::ColumnChannel);
    else
        hideColumn(Logger::ColumnChannel);
}

void LogTableView::resizeVisibleRows()
{
    if (model()->rowCount() == 0) {
        return;
    }

    int page_cnt    = verticalScrollBar()->pageStep();

    int start_index = verticalScrollBar()->value() - page_cnt * 2;
        start_index = start_index < 0 ? 0 : start_index;

    int end_index   = verticalScrollBar()->value() + page_cnt * 2;
        end_index   = end_index >= model()->rowCount() ? model()->rowCount() - 1 : end_index;

    qDebug() << start_index << end_index;

    for (int i = start_index; i <= end_index; i++) {
        resizeRowToContents(i);
    }
}

void LogTableView::reset()
{
    int width, tmp;
    auto fm_1 = QFontMetrics(m_decorator->channelFont(Logger::ChannelFirst));
    auto fm_2 = QFontMetrics(m_decorator->channelFont(Logger::ChannelSecond));
    auto fm_c = QFontMetrics(m_decorator->channelFont(Logger::ChannelComment));
    auto fm_e = QFontMetrics(m_decorator->channelFont(Logger::ChannelError));

    // Timestamp column size
    auto dt = m_formatter->format(QDateTime::currentDateTime());

    width = fm_1.horizontalAdvance(dt)*1.25;

    tmp = fm_2.horizontalAdvance(dt)*1.25;
    width = tmp > width ? tmp : width;

    tmp = fm_c.horizontalAdvance(dt)*1.25;
    width = tmp > width ? tmp : width;

    tmp = fm_e.horizontalAdvance(dt)*1.25;
    width = tmp > width ? tmp : width;

    setColumnWidth(Logger::ColumnTimestamp, width);

    // ChannelName column size
    width = fm_1.horizontalAdvance(m_formatter->channelName(Logger::ChannelFirst))*1.25;

    tmp = fm_2.horizontalAdvance(m_formatter->channelName(Logger::ChannelSecond))*1.25;
    width = tmp > width ? tmp : width;

    tmp = fm_c.horizontalAdvance(m_formatter->channelName(Logger::ChannelComment))*1.25;
    width = tmp > width ? tmp : width;

    tmp = fm_e.horizontalAdvance(m_formatter->channelName(Logger::ChannelError))*1.25;
    width = tmp > width ? tmp : width;

    setColumnWidth(Logger::ColumnChannel, width);
    resizeRowsToContents();

    QTableView::reset();
}


void LogTableView::createMenu()
{
    m_base_menu = new QMenu();
        m_base_menu->addAction(m_options);

    m_data_channel_menu = new QMenu();
        // m_data_channel_menu->addAction(m_add_to_analyzer);
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

    m_group_menu = new QMenu();
        m_group_menu->addAction(m_copy);
        m_group_menu->addSeparator();
        m_group_menu->addAction(m_options);
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
            auto row_list = selectionModel()->selectedRows();
            if (row_list.size() > 1) {
                menu = m_group_menu;
            }
            else {
                auto channel = model()->data( model()->index( row, Logger::ColumnChannel ) ).toInt();

                menu = (channel == Logger::ChannelFirst || channel == Logger::ChannelSecond ) ?
                    m_data_channel_menu : m_info_channel_menu;
            }
        }

        menu->popup(viewport()->mapToGlobal(pos));
    });

    connect(m_copy, &QAction::triggered, this, [this]() {
        auto row_list = selectionModel()->selectedRows();

        QString text;
        for (int i = 0; i < row_list.count(); i++) {
            text.append(m_item_delegate->message( row_list.at(i) ));
            text.append('\n');
        }

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(text);
    });

    connect(m_copy_message, &QAction::triggered, this, [this]() {
        auto msg = model()->data(model()->index(currentIndex().row(), Logger::ColumnMsg)).toString();

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_copy_as_bytes, &QAction::triggered, this, [this]() {
        char separator = m_item_delegate->formatter()->separator();
        auto msg = model()->data(model()->index(currentIndex().row(), Logger::ColumnMsg))
                .toByteArray().toHex(separator);

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_copy_as_string, &QAction::triggered, this, [this]() {
        auto msg = model()->data(model()->index(currentIndex().row(), Logger::ColumnMsg)).toString();

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_add_to_analyzer, &QAction::triggered, this, [this]() {
        auto channel = model()->data( model()->index( currentIndex().row(), Logger::ColumnChannel ) ).toInt();

        if (channel == Logger::ChannelFirst || channel == Logger::ChannelSecond ) {

            auto data = currentIndex().data();
            if (!data.isNull()) {

            }
        }
    });

    connect(m_options, &QAction::triggered, this, [this]() {
        m_dec_dialog->readOrigin(m_decorator);
        m_dec_dialog->show();
    });

    connect(m_dec_dialog, &LogDecorationDialog::sConfigChanged, this, [this]() {
        m_dec_dialog->apply(m_decorator);
        reset();
        resizeRowsToContents();
    });
}


