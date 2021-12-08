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
#include "LogTableView.h"
#include "LogModel.h"
#include "ConnectionConfigDialog.h"
#include "LogDecorationDialog.h"

#include "LuaApi.h"






LogWidget::LogWidget(QWidget* parent)
  : QWidget(parent)
{
    createGui();
    createConnections();
    m_conn_dialog = new ConnectionConfigDialog();
    m_conn_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);

    m_decoration_dialog = new LogDecorationDialog();
    m_decoration_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);

    // ------- Test --------
    m_log_model = new LogModel(this);
    m_lua_api = new LuaApi(this);
        m_lua_api->setScriptFile("/home/evgen/workspace/protodb/script.lua");

    connect(m_lua_api, &LuaApi::sLogPrint, m_log_model, &LogModel::comment);
    connect(m_lua_api, &LuaApi::sLogError, m_log_model, &LogModel::error);
    connect(m_lua_api, &LuaApi::sLogClear, m_log_model, &LogModel::clear);

    setModel(m_log_model);
    // ------- Test --------
}

void LogWidget::setModel(LogModel *model)
{
    m_view->setModel(model);
    m_decoration_dialog->setDecorator(model->decorator());
}

LogModel LogWidget::model() const
{
    auto model = m_view->model();
    return dynamic_cast<LogModel*>(model) ? static_cast<LogModel*>(model) : nullptr;
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
        m_mode_btn->setText("HEX");
        m_mode_btn->setFixedSize(64, 32);

    m_run = new QPushButton();
        m_run->setIcon(QIcon(":/icons/run.svg"));
        m_run->setIconSize(QSize(24,24));
        m_run->setFixedSize(32, 32);

    m_cfg_btn = new QPushButton();
        m_cfg_btn->setText("115200, none, 8, 1");
        m_cfg_btn->setFixedSize(136, 32);

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
        top_layout->addWidget(m_clr_btn);
        top_layout->addWidget(m_find_le);
        top_layout->addWidget(m_run);
        top_layout->addWidget(m_mode_btn);
        top_layout->addWidget(m_cfg_btn);

    auto bottom_layout = new QHBoxLayout();
        bottom_layout->addWidget(m_msg_le);
        bottom_layout->addWidget(edit_mode_cmb);
        bottom_layout->addWidget(editor);

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
            QVector<uint8_t> msg{0x31, 0x32, 0x33, 0x34};

            m_lua_api->beforeTransmit(msg);
            std::cout << std::hex << (uint64_t)msg[0] << std::endl;

            QByteArray array(5, 0x32);

            m_lua_api->afterReceive(array);
            std::cout << std::hex << (uint64_t)array[0] << std::endl;
        }
        else
        {
            m_run->setIcon(QIcon(":/icons/run.svg"));
            m_lua_api->stop();
        }
        state = !state;
    });


    connect(m_clr_btn, &QPushButton::released, this, [this] {
        auto model = m_view->model();

        if (dynamic_cast<LogModel*>(model)) {
            auto log_model = static_cast<LogModel*>(model);
            log_model->clear();
        }
    });

    connect(m_mode_btn, &QPushButton::released, this, [this]() {
       static bool state = true;

       if (state) {
           m_mode_btn->setText("ASCII");
       }
       else {
           m_mode_btn->setText("HEX");
       }

       state = !state;
    });


    connect(m_cfg_btn, &QPushButton::released, this, [this]() {
        m_conn_dialog->show();
    });
}




