#include "protodb/SerialIOWidget.h"
#include "protodb/SerialIOWidgetCreator.h"

#include <protodb/SerialConnectionCreator.h>

#include <protodb/utils/JsonUtils.h>
#include <nlohmann/json.hpp>

#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QSerialPortInfo>
#include <QIntValidator>
#include <QEvent>
#include <QPushButton>

using namespace protodb;

SerialConfigWidget::SerialConfigWidget(QWidget* parent)
    : ConnectionConfigWidget(parent)
{
    createGui();
    connectSignals();

    setDefaultConfig();
    refreshPortList();
}

QString SerialConfigWidget::gcid() const
{
    return SerialConnectionWidgetCreator::creatorId();
}

QString SerialConfigWidget::deviceCID() const
{
    return SerialConnectionCreator::creatorId();
}

void SerialConfigWidget::defaultConfig(nlohmann::json &json) const
{
    json["gcid"]        = SerialConnectionWidgetCreator::creatorId();
    json["PortName"]    = "";
    json["OpenMode"]    = "Communication";
    json["Baudrate"]    = "115200";
    json["DataBits"]    = "8";
    json["Parity"]      = "None";
    json["StopBits"]    = "1";
    json["FlowControl"] = "None";
}

void SerialConfigWidget::config(nlohmann::json &json) const
{
    json["gcid"]        = SerialConnectionWidgetCreator::creatorId();
    json["PortName"]    = m_device->currentData().toString();
    json["OpenMode"]    = m_open_mode->currentText();
    json["Baudrate"]    = m_baudrate->currentText();
    json["DataBits"]    = m_data_bits->currentText();
    json["Parity"]      = m_parity->currentText();
    json["StopBits"]    = m_stop_bits->currentText();
    json["FlowControl"] = m_flow_ctrl->currentText();
}

void SerialConfigWidget::setConfig(const nlohmann::json &json)
{
    QString baudrate = json.value("Baudrate", QString());
    if (m_baudrate->findText(baudrate) == -1) {
        m_baudrate->setItemText(m_baudrate->count() - 1, baudrate);
    }
    m_baudrate->setCurrentText(baudrate);

    auto idx = m_device->findData(json.value("PortName", QString()));
        idx = idx < 0 ? 0 : idx;
        m_device->setCurrentIndex(idx);

    m_open_mode->setCurrentText(json.value("OpenMode", QString()));
    m_data_bits->setCurrentText(json.value("DataBits", QString()));
    m_parity->setCurrentText(json.value("Parity", QString()));
    m_stop_bits->setCurrentText(json.value("StopBits", QString()));
    m_flow_ctrl->setCurrentText(json.value("FlowControl", QString()));
}

void SerialConfigWidget::createGui()
{
    static const QStringList baudrate_list   = { "1200", "4800", "9600", "19200", "38400", "57600", "115200", tr("Custom") };
    static const QStringList databits_list   = { "5", "6", "7", "8" };
    static const QStringList flow_ctrl_list  = { "None", "Software", "Hardware" };
    static const QStringList parity_list     = { "None", "Even", "Odd", "Space", "Mark" };
    static const QStringList stop_bits_list  = { "1", "2" };
    static const QStringList open_mode_list  = { "Communication", "Monitoring" };

    // --------[COMBO BOXES] --------- //
    m_baudrate_validator = new QIntValidator(0, 10000000, this);
    m_baudrate  = new QComboBox();
        m_baudrate->addItems(baudrate_list);
        m_baudrate->setInsertPolicy(QComboBox::InsertAtCurrent);

    m_data_bits = new QComboBox();
        m_data_bits->addItems(databits_list);
    m_flow_ctrl = new QComboBox();
        m_flow_ctrl->addItems(flow_ctrl_list);
    m_parity    = new QComboBox();
        m_parity->addItems(parity_list);
    m_stop_bits = new QComboBox();
        m_stop_bits->addItems(stop_bits_list);
    m_open_mode = new QComboBox();
        m_open_mode->addItems(open_mode_list);
    m_device    = new QComboBox();
        m_device->setDuplicatesEnabled(false);
        m_device->setMaxVisibleItems(10);
        m_device->setStyleSheet("combobox-popup: 0;");

    m_refresh_btn = new QPushButton();
        m_refresh_btn->setIcon(QIcon(":/icons/refresh.svg"));
        m_refresh_btn->setFixedSize(26, 26);

    // --------[LABELS]-------- //
    auto label_baudrate  = new QLabel(tr("Baudrate"));
    auto label_data_bits = new QLabel(tr("Data Bits"));
    auto label_flow_ctrl = new QLabel(tr("Flow Control"));
    auto label_parity    = new QLabel(tr("Parity"));
    auto label_stop_bits = new QLabel(tr("Stop Bits"));
    auto label_open_mode = new QLabel(tr("Open Mode"));
    auto label_device    = new QLabel(tr("Device"));


    auto cfg_layout = new QGridLayout();
        cfg_layout->setAlignment(Qt::AlignTop);
        cfg_layout->addWidget(label_device, 0, 0);
        cfg_layout->addWidget(m_device, 0, 1, 1, 3);
        cfg_layout->addWidget(m_refresh_btn, 0, 4);
        cfg_layout->addWidget(label_open_mode, 1, 0);
        cfg_layout->addWidget(m_open_mode, 1, 1);
        cfg_layout->addWidget(label_baudrate, 1, 2);
        cfg_layout->addWidget(m_baudrate, 1, 3);
        cfg_layout->addWidget(label_flow_ctrl, 2, 0);
        cfg_layout->addWidget(m_flow_ctrl, 2, 1);
        cfg_layout->addWidget(label_data_bits, 2, 2);
        cfg_layout->addWidget(m_data_bits, 2, 3);
        cfg_layout->addWidget(label_parity, 3, 0);
        cfg_layout->addWidget(m_parity, 3, 1);
        cfg_layout->addWidget(label_stop_bits, 3, 2);
        cfg_layout->addWidget(m_stop_bits, 3, 3);

        cfg_layout->setColumnStretch(0, 0);
        cfg_layout->setColumnStretch(1, 0);
        cfg_layout->setColumnStretch(2, 0);
        cfg_layout->setColumnStretch(3, 0);
        cfg_layout->setColumnStretch(4, 1);

    auto main_layout = new QVBoxLayout();
        main_layout->setAlignment(Qt::AlignTop);
        main_layout->addWidget(new QLabel(tr("Serial configurations:")));
        main_layout->addLayout(cfg_layout);

    setLayout(main_layout);
}

void SerialConfigWidget::connectSignals()
{
    connect(m_refresh_btn, &QPushButton::clicked, this, [this]() {
        refreshPortList();
    });

    connect(m_baudrate, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index){
        if (index == m_baudrate->count() - 1) {
            m_baudrate->setEditable(true);
            m_baudrate->setValidator(m_baudrate_validator);
            m_baudrate->lineEdit()->selectAll();
        }
        else {
            m_baudrate->setEditable(false);
        }
    });
}

void SerialConfigWidget::refreshPortList()
{
    const auto& ports = QSerialPortInfo::availablePorts();
    int idx = m_device->currentIndex();
        idx = idx >= ports.count() ? 0 : idx;

    m_device->clear();
        for (auto& p: ports) {
            #ifdef _WIN32
                auto str = p.portName();
            #else
                auto str = p.systemLocation();
            #endif

            auto desc = QSerialPortInfo(p).description();
                if (!desc.isEmpty()) str = str + QString(" (%1)").arg(desc);
            m_device->addItem(str, p.systemLocation());
        }
    m_device->setCurrentIndex(idx);
}

bool SerialConfigWidget::event(QEvent* e)
{
    if (e->type() == QEvent::Show) {
        refreshPortList();
    }

    return QWidget::event(e);
}
