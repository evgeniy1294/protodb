#include <QLayout>
#include <QTableView>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>


#include "LogWidget.h"
#include "Logger.h"
#include "LogTableModel.h"
#include "LogItemDelegate.h"
#include "LogItemFormatter.h"
#include "ConnectionConfigDialog.h"
#include "event.h"

LogWidget::LogWidget(QWidget* parent)
  : QWidget(parent)
{
    createGui();
    connectSignals();
    m_conn_dialog = new ConnectionConfigDialog();
        m_conn_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
}


LogWidget::~LogWidget()
{

}
#include <iostream>

void LogWidget::createGui()
{
    // ---------[LOG VIEW]---------- //
    m_log = new Logger();
    m_view = new QTableView();
        m_model = new LogTableModel(m_view);
            m_model->setLogger(m_log);
        m_view->setModel(m_model);
        m_view->setItemDelegate(new LogItemDelegate());
        m_view->setWordWrap(true);
        m_view->setSelectionBehavior(QAbstractItemView::SelectRows);
        m_view->setShowGrid(false);
        m_view->verticalHeader()->hide();
        m_view->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
        auto hh = m_view->horizontalHeader();
            hh->setSectionResizeMode(LogTableModel::kColumnTimestamp, QHeaderView::ResizeToContents);
            hh->setSectionResizeMode(LogTableModel::kColumnChannel,   QHeaderView::ResizeToContents);
            hh->setSectionResizeMode(LogTableModel::kColumnMsg,     QHeaderView::Stretch);
            hh->hide();

    // ---------[BUTTONS]---------- //
    m_clear_btn = new QPushButton();
        m_clear_btn->setIcon(QIcon(":/icons/delete_cross.svg"));
        m_clear_btn->setToolTip("Clear log window");

    m_mode_btn = new QPushButton();
        m_mode_btn->setText(toString(m_model->formatter()->messageFormat()));
        m_mode_btn->setFixedSize(64, 32);

    m_run = new QPushButton();
        m_run->setIcon(QIcon(":/icons/run.svg"));
        m_run->setIconSize(QSize(24,24));
        m_run->setFixedSize(32, 32);

    m_config_btn = new QPushButton();
        m_config_btn->setText("115200, none, 8, 1");
        m_config_btn->setFixedSize(136, 32);

    // ---------[LINE EDIT]---------- //
    m_find_le = new QLineEdit();
        m_find_le->setPlaceholderText(tr("Find sequence"));
        m_find_le->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);

    m_msg_le = new QLineEdit();
        m_msg_le->setPlaceholderText("Print your message");


    // ---------[COMBO BOX]---------- //
    auto tool_mode_cmb = new QComboBox();
        tool_mode_cmb->addItem("HEX");
        tool_mode_cmb->addItem("ASCII");

    auto edit_mode_cmb = new QComboBox();
        edit_mode_cmb->addItem("HEX");
        edit_mode_cmb->addItem("LF");
        edit_mode_cmb->addItem("CR");
        edit_mode_cmb->addItem("CR/LF");
        edit_mode_cmb->addItem("None");
        edit_mode_cmb->addItem("DSL");

    // ---------[SPIN BOX]---------- //
    auto editor = new QSpinBox();
        editor->setMinimum(0);
        editor->setMaximum(120);
        editor->setSingleStep(1);
        editor->setSuffix("ms");
        editor->setSpecialValueText(QObject::tr("No delay"));

    // ---------[LAYOUT]---------- //
    auto top_layout = new QHBoxLayout();
        top_layout->addWidget(m_clear_btn);
        top_layout->addWidget(m_find_le);
        top_layout->addWidget(m_run);
        top_layout->addWidget(m_mode_btn);
        top_layout->addWidget(m_config_btn);

    auto bottom_layout = new QHBoxLayout();
        bottom_layout->addWidget(m_msg_le);
        bottom_layout->addWidget(edit_mode_cmb);
        bottom_layout->addWidget(editor);

    QGridLayout* layout = new QGridLayout();
      layout->addLayout(top_layout, 0, 0);
      layout->addWidget(m_view, 1, 0);
      layout->addLayout(bottom_layout, 2, 0);

    setLayout(layout);

    // --------[TEST]-------- //
        m_log->print(Logger::kFirstChannel, QByteArray(12, '1'));
        m_log->print(Logger::kCommentChannel, QByteArray("Test comment string:\n    byte[0] = 7\n    byte[1] = 7\n    byte[2] = 8"));
        m_log->print(Logger::kSecondChannel, QByteArray(90, '7'));
}

void LogWidget::connectSignals()
{
    connect(m_run, &QPushButton::released, this, [this]() {
        static bool state = true;
        if (state) {
            m_run->setIcon(QIcon(":/icons/stop_rect.svg"));;
            state = false;
        }
        else
        {
            m_run->setIcon(QIcon(":/icons/run.svg"));
            state = true;
        }
    });


    connect(m_clear_btn, &QPushButton::released, this, [this] {
        m_log->clear();
    });

    connect(m_mode_btn, &QPushButton::released, this, [this]() {
       m_mode_btn->setText(toString(m_model->formatter()->nextMessageFormat()));
    });


    connect(m_config_btn, &QPushButton::released, this, [this]() {
        m_conn_dialog->show();
    });
}




