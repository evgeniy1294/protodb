#include <QApplication>
#include <QClipboard>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <protodb/LogTableView.h>
#include <protodb/LogDecorationDialog.h>
#include <protodb/LogItemDelegate.h>
#include <protodb/LogModel.h>

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
        hh->setSectionResizeMode(LogModel::kColumnTimestamp, QHeaderView::ResizeToContents);
        hh->setSectionResizeMode(LogModel::kColumnChannel,   QHeaderView::ResizeToContents);
        hh->setSectionResizeMode(LogModel::kColumnMsg,       QHeaderView::Stretch);
}

void LogTableView::setDecorator(LogDecorator* decorator)
{
    m_dec_dialog->setDecorator(decorator);
}

void LogTableView::setByteFormat(ByteFormat format)
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
        auto index = indexAt(pos);

        if (index.row() != -1) {
            auto channel = index.data(LogModel::ChannelRole).toInt();

            menu = (channel == kFirstLogChannel || channel == kSecondLogChannel) ?
                m_data_channel_menu : m_info_channel_menu;
        }

        menu->popup(viewport()->mapToGlobal(pos));
    });

    connect(m_copy, &QAction::triggered, this, [this]() {
        auto format = m_item_delegate->byteFormat();
        int role = (format == ByteFormat::kAsciiFormat) ? LogModel::EventAsciiDisplayRole :
                                                          LogModel::EventHexDisplayRole;

        auto msg = currentIndex().data(role).toString();
        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_copy_message, &QAction::triggered, this, [this]() {
        auto msg = currentIndex().data(Qt::DisplayRole).toString();

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_copy_as_bytes, &QAction::triggered, this, [this]() {
        auto msg = currentIndex().data(LogModel::HexDisplayRole).toString();

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_copy_as_string, &QAction::triggered, this, [this]() {
        auto msg = currentIndex().data(LogModel::AsciiDisplayRole).toString();

        QClipboard* pcb = QApplication::clipboard();
            pcb->setText(msg);
    });

    connect(m_add_to_analyzer, &QAction::triggered, this, [this]() {
        auto data = currentIndex().data(LogModel::AnalyzeRole);
        if (!data.isNull()) {
            emit sToAnalyzer(data.toByteArray());
        }
    });

    connect(m_options, &QAction::triggered, this, [this]() {
        m_dec_dialog->show();
    });
}

