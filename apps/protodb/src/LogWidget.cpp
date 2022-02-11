#include "LogWidget.h"
#include "LogTableView.h"
#include "ConnectionConfigDialog.h"
#include "LogDecorationDialog.h"
#include "LogProxyModel.h"

#include <protodb/Logger.h>
#include <protodb/LuaApi.h>

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

LogWidget::LogWidget(QWidget* parent)
  : QWidget(parent)
{
    createGui();

    m_conn_dialog = new ConnectionConfigDialog();
        m_conn_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
        m_conn_dialog->resize(QSize(640, 540));

    m_log_proxy_model = new LogProxyModel(this);

    // ------- Test --------
    m_log_model = new Logger(this);
    m_lua_api = new LuaApi(this);
        m_lua_api->setScriptFile("/home/evgen/Workspace/protodb/script.lua");

    connect(m_lua_api, &LuaApi::sLogPrint, m_log_model, &Logger::comment);
    connect(m_lua_api, &LuaApi::sLogError, m_log_model, &Logger::error);
    connect(m_lua_api, &LuaApi::sLogClear, m_log_model, &Logger::clear);

    setModel(m_log_model);
    // ------- Test --------

    createConnections();
}

void LogWidget::setModel(Logger *model)
{
    m_log_proxy_model->setSourceModel(model);
        m_log_proxy_model->invalidate();

    m_view->setModel(m_log_proxy_model);
    m_view->setDecorator(model->decorator());
}

Logger LogWidget::model() const
{
    auto model = m_log_proxy_model->sourceModel();
    return dynamic_cast<Logger*>(model) ? static_cast<Logger*>(model) : nullptr;
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

    // ---------[LINE EDIT]---------- //
    m_find_le = new QLineEdit();
        m_find_le->setPlaceholderText(tr("Find sequence"));
        m_find_le->addAction(QIcon(":/icons/search.svg"), QLineEdit::TrailingPosition);

    m_msg_le = new QLineEdit();
        m_msg_le->setPlaceholderText("Print your message");


    // ---------[COMBO BOX]---------- //
    auto edit_mode_cmb = new QComboBox();
        edit_mode_cmb->addItem("HEX");
        edit_mode_cmb->addItem("LF");
        edit_mode_cmb->addItem("CR");
        edit_mode_cmb->addItem("CR/LF");
        edit_mode_cmb->addItem("None");
        edit_mode_cmb->addItem("DSL");

    // ---------[LAYOUT]---------- //
    auto top_layout = new QHBoxLayout();
        top_layout->addWidget(m_clr_btn);
        top_layout->addWidget(m_find_le);
        top_layout->addWidget(m_run);
        top_layout->addWidget(m_mode_btn);
        top_layout->addWidget(m_cfg_btn);

    auto bottom_layout = new QHBoxLayout();
        bottom_layout->addWidget(m_msg_le);
        bottom_layout->addWidget(edit_mode_cmb);

    QGridLayout* layout = new QGridLayout();
      layout->addLayout(top_layout, 0, 0);
      layout->addWidget(m_view, 1, 0);
      layout->addLayout(bottom_layout, 2, 0);

    setLayout(layout);
}

void LogWidget::createConnections()
{
    connect(m_run, &QPushButton::released, this, [this]() {
        static bool state = true;
        if (state) {
            m_run->setIcon(QIcon(":/icons/stop_rect.svg"));

            m_lua_api->loadScript();
            m_lua_api->start();

            // Test
            QByteArray array;
                array.push_back(0x31);
                array.push_back(0x32);
                array.push_back(0x33);
                array.push_back(0x34);

            m_lua_api->beforeTransmit(array);
            m_log_model->log(kSecondLogChannel, array);
            //std::cout << std::hex << (uint64_t)msg[0] << std::endl;

            array.clear();
            array.push_back(0x35);
            array.push_back(0x36);
            array.push_back(0x37);
            array.push_back(0x38);
            m_log_model->log(kFirstLogChannel, array);
            m_lua_api->afterReceive(array);

            //std::cout << std::hex << (uint64_t)array[0] << std::endl;
        }
        else
        {
            m_run->setIcon(QIcon(":/icons/run.svg"));
            m_lua_api->stop();
        }
        state = !state;
    });


    connect(m_clr_btn, &QPushButton::released, this, [this] {
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
           m_view->setByteFormat(ByteFormat::kAsciiFormat);
           m_view->reset();
           m_mode_btn->setIcon(QIcon(":/icons/ascii.svg"));
       }
       else {
           m_view->setByteFormat(ByteFormat::kHexFormat);
           m_view->reset();
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

}




