#include <QComboBox>
#include <QLabel>
#include <QLayout>
#include <QSerialPortInfo>


#include "SerialConfigWidget.h"


SerialConfigWidget::SerialConfigWidget(QWidget* parent)
    : QFrame(parent)
{
    createGui();
}

void SerialConfigWidget::createGui()
{
    static const QStringList baudrate_list   = { "1200", "4800", "9600", "19200", "38400", "57600", "115200", "Custom" };
    static const QStringList databits_list   = { "5", "6", "7", "8" };
    static const QStringList flow_ctrl_list  = { "None", "Software", "Hardware" };
    static const QStringList parity_list     = { "None", "Even", "Odd", "Space", "Mark" };
    static const QStringList stop_bits_list  = { "1", "2" };
    static const QStringList open_mode_list  = { "Send/Receive", "Monitoring" };

    // --------[COMBO BOXES] --------- //
    m_baudrate  = new QComboBox();
        m_baudrate->addItems(baudrate_list);
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

    for (auto& port: QSerialPortInfo::availablePorts()) {
        m_device->addItem(port.portName());
    }

    // --------[LABELS]-------- //
    auto label_baudrate  = new QLabel(tr("Baudrate"));
    auto label_data_bits = new QLabel(tr("Data Bits"));
    auto label_flow_ctrl = new QLabel(tr("Flow Control"));
    auto label_parity    = new QLabel(tr("Parity"));
    auto label_stop_bits = new QLabel(tr("Stop Bits"));
    auto label_open_mode = new QLabel(tr("Open Mode"));
    auto label_device    = new QLabel(tr("Device"));

    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop);
        main_layout->addWidget(label_device, 0, 0);
        main_layout->addWidget(m_device, 0, 1);
        main_layout->addWidget(label_open_mode, 0, 2);
        main_layout->addWidget(m_open_mode, 0, 3);
        main_layout->addWidget(label_baudrate, 1, 0);
        main_layout->addWidget(m_baudrate, 1, 1);
        main_layout->addWidget(label_data_bits, 1, 2);
        main_layout->addWidget(m_data_bits, 1, 3);
        main_layout->addWidget(label_flow_ctrl, 2, 0);
        main_layout->addWidget(m_flow_ctrl, 2, 1);
        main_layout->addWidget(label_stop_bits, 2, 2);
        main_layout->addWidget(m_stop_bits, 2, 3);
        main_layout->addWidget(label_parity, 3, 0);
        main_layout->addWidget(m_parity, 3, 1);

        main_layout->setColumnStretch(0, 0);
        main_layout->setColumnStretch(1, 0);
        main_layout->setColumnStretch(2, 0);
        main_layout->setColumnStretch(3, 0);
        main_layout->setColumnStretch(4, 1);

    setLayout(main_layout);
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
}
