#include "LogWidget.h"
#include "LogTableView.h"
#include "SeanseConfigDialog.h"
#include "LogFormatter.h"
#include "LogProxyModel.h"
#include "Logger.h"
#include "MainClass.h"

#include <QApplication>
#include <QLayout>
#include <QTableView>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>

using namespace protodb;

LogWidget::LogWidget(QWidget* parent)
  : QWidget(parent)
{
    createGui();

    m_conn_dialog = new SeanceConfigDialog();
        m_conn_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        m_conn_dialog->resize(QSize(640, 540));

    auto logger = MainClass::instance().logger();
    m_log_proxy_model = new LogProxyModel(this);
        m_log_proxy_model->setSourceModel(logger);
        m_log_proxy_model->invalidate();

    m_view->setModel(m_log_proxy_model);
        m_view->reset();

    createConnections();
}

void LogWidget::createGui()
{
    // ---------[LOG VIEW]---------- //
    m_view = new LogTableView();

    // ---------[BUTTONS]---------- //
    m_clr_btn = new QPushButton();
        m_clr_btn->setIcon(QIcon(":/icons/delete_cross.svg"));
        m_clr_btn->setToolTip("Clear log window");

    m_mode_btn = new QPushButton();
        m_mode_btn->setIcon(QIcon(":/icons/hex.svg"));
        m_mode_btn->setIconSize(QSize(24,24));
        m_mode_btn->setFixedSize(32, 32);

    m_run = new QPushButton();
        m_run->setIcon(QIcon(":/icons/run.svg"));
        m_run->setIconSize(QSize(24,24));
        m_run->setFixedSize(32, 32);

    m_cfg_btn = new QPushButton();
        m_cfg_btn->setIcon(QIcon(":/icons/settings_gear.svg"));
        m_cfg_btn->setIconSize(QSize(18,18));
        m_cfg_btn->setFixedSize(32, 32);

    m_auto_scroll = new QPushButton();
        m_auto_scroll->setIcon(QIcon(":/icons/sort_down.svg"));
        m_auto_scroll->setIconSize(QSize(18,18));
        m_auto_scroll->setFixedSize(32, 32);
        m_auto_scroll->setCheckable(true);
        m_auto_scroll->setChecked(true);

    // ---------[LINE EDIT]---------- //
    m_find_le = new QLineEdit();
        m_find_le->setPlaceholderText(tr("Find sequence"));
        m_find_le->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);

    m_msg_le = new QLineEdit();
        m_msg_le->setPlaceholderText("Print your message");


    // ---------[COMBO BOX]---------- //
    m_data_format_cmb = new QComboBox();
        m_data_format_cmb->addItem("HEX",   Hex_Format);
        m_data_format_cmb->addItem("LF",    Ascii_LF_Format);
        m_data_format_cmb->addItem("CR",    Ascii_CR_Format);
        m_data_format_cmb->addItem("CR/LF", Ascii_CRLF_Format);
        m_data_format_cmb->addItem("None",  Ascii_None_Format);

    // ---------[LAYOUT]---------- //
    auto top_layout = new QHBoxLayout();
        top_layout->addWidget(m_clr_btn);
        top_layout->addWidget(m_find_le);
        top_layout->addWidget(m_run);
        top_layout->addWidget(m_mode_btn);
        top_layout->addWidget(m_auto_scroll);
        top_layout->addWidget(m_cfg_btn);

    auto bottom_layout = new QHBoxLayout();
        bottom_layout->addWidget(m_msg_le);
        bottom_layout->addWidget(m_data_format_cmb);

    QGridLayout* layout = new QGridLayout();
      layout->addLayout(top_layout, 0, 0);
      layout->addWidget(m_view, 1, 0);
      layout->addLayout(bottom_layout, 2, 0);

    setLayout(layout);
}

void LogWidget::createConnections()
{
    connect(m_log_proxy_model, &QAbstractProxyModel::rowsInserted, this, [this]() {
        if (m_auto_scroll->isChecked())
            m_view->scrollToBottom();
    });

    connect(m_data_format_cmb, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int idx) {
        // В этой функции можно менять валидатор, если используется hex-строка
    });

    connect(m_msg_le, &QLineEdit::returnPressed, this, [this]() {
        auto msg = m_msg_le->text().toLatin1();

        if (msg.isEmpty()) {
            return;
        }

        auto format = m_data_format_cmb->currentIndex();
        if ( format == Hex_Format) {
            msg = QByteArray::fromHex( msg );
        }
        else {
            if (format == Ascii_LF_Format) {
                msg.append('\n');
            }
            else if (format == Ascii_CR_Format) {
                msg.append('\r');
            }
            else if (format == Ascii_CRLF_Format) {
                msg.append('\n');
                msg.append('\r');
            }
        }

        MainClass::instance().sendBytes(msg);
    });

    connect(m_run, &QPushButton::released, this, [this]() {
        auto logger = MainClass::instance().logger();

        if (MainClass::instance().isStarted()) {
            MainClass::instance().stop();
        }
        else
        {
            MainClass::instance().start();
        }
    });

    connect(m_conn_dialog, &QDialog::accepted, this, [this]() {
        nlohmann::json seance_cfg;
            m_conn_dialog->state(seance_cfg);
            MainClass::instance().setSeanceConfigs(seance_cfg);

        auto log_configs = seance_cfg.value("LogConfigs", nlohmann::json::object());

        auto formatter = m_view->formatter();
            formatter->setTimeFormat(log_configs.value("TimestampFormat", LogFormatter::DefaultTimeFormat));
            formatter->setSeparator(log_configs.value<char>("CharSeparator", LogFormatter::DefaultSeparator));

        m_view->reset();
        m_view->resizeRowsToContents();
    });

    connect(m_clr_btn, &QPushButton::released, this, [this]() {
        auto model = m_log_proxy_model->sourceModel();

        if (dynamic_cast<QSortFilterProxyModel*>(model)) {
            model = static_cast<QSortFilterProxyModel*>(model)->sourceModel();
        }

        if (dynamic_cast<Logger*>(model)) {
            auto log_model = static_cast<Logger*>(model);
            log_model->clear();
        }
    });

    connect(m_mode_btn, &QPushButton::released, this, [this]() {
       static bool state = true;

       if (state) {
           m_view->formatter()->setByteFormat(LogFormatter::AsciiFormat);
           m_view->reset();
           m_view->resizeRowsToContents();
           m_mode_btn->setIcon(QIcon(":/icons/ascii.svg"));
       }
       else {
           m_view->formatter()->setByteFormat(LogFormatter::HexFormat);
           m_view->reset();
           m_view->resizeRowsToContents();
           m_mode_btn->setIcon(QIcon(":/icons/hex.svg"));
       }

       state = !state;
    });


    connect(m_cfg_btn, &QPushButton::released, this, [this]() {
        m_conn_dialog->show();
    });

    connect(m_find_le, &QLineEdit::editingFinished, this, [this]() {
        bool valid = m_log_proxy_model->setFilterExpression(m_find_le->text());

        auto palette = QApplication::palette();
        if (!valid) {
            palette.setColor(QPalette::Base, QColor(0xffcece));
        }

        m_find_le->setPalette(palette);
    });

    connect(&MainClass::instance(), &MainClass::sStarted, this, [this](QString str) {
        Q_UNUSED(str)
        m_run->setIcon(QIcon(":/icons/stop_rect.svg"));
    });

    connect(&MainClass::instance(), &MainClass::sStopted, this, [this]() {
        m_run->setIcon(QIcon(":/icons/run.svg"));
    });

    connect(&MainClass::instance(), &MainClass::sStartFailed, m_conn_dialog, &QWidget::show);
}




