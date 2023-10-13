#include "protodb/NetIOWidget.h"
#include "protodb/NetIOWidgetCreator.h"

#include <protodb/utils/JsonUtils.h>
#include <nlohmann/json.hpp>

#include <protodb/NetworkConnectionCreator.h>

#include <QLineEdit>
#include <QComboBox>
#include <QLayout>
#include <QLabel>
#include <QIntValidator>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QPushButton>

using namespace protodb;

NetIOWidget::NetIOWidget(QWidget* parent)
    : IOWidget(parent)
{
    createGui();
    connectSignals();

    setDefaultConfig();
}

QString NetIOWidget::gcid() const
{
    return NetIOWIdgetCreator::creatorId();
}

QString NetIOWidget::deviceCID() const
{
    return NetworkConnectionCreator::creatorId();
}

void NetIOWidget::defaultConfig(nlohmann::json& json) const
{
    json["gcid"] = NetIOWIdgetCreator::creatorId();
    json["Mode"] = "Client";
    json["Protocol"] = "TCP";
    json["RemoteIP"] = "127.0.0.1";
    json["RemotePort"] = 0;
    json["LocalPort"] = 0;
    json["Interface"] = QString();
    json["LocalIP"] = "127.0.0.1";
}

void NetIOWidget::config(nlohmann::json& json) const
{
    json["gcid"] = NetIOWIdgetCreator::creatorId();
    json["Mode"] = m_mode->currentText();
    json["Protocol"] = m_protocol->currentText();
    json["RemoteIP"] = m_remote_ip->text();
    json["RemotePort"] = m_remote_port->text().toInt();
    json["LocalPort"] = m_local_port->text().toInt();
    json["Interface"] = m_interfaces->currentData().toString();
    json["LocalIP"] = m_local_ip->currentText();
}

void NetIOWidget::setConfig(const nlohmann::json& json)
{
    m_mode->setCurrentText(json.value<QString>("Mode", QString("Client")));
    m_protocol->setCurrentText(json.value<QString>("Protocol", QString("TCP")));
    m_remote_ip->setText(json.value<QString>("RemoteIP", QString("127.0.0.1")));
    m_remote_port->setText(QString::number(json.value<int>("RemotePort", int(0))));
    m_local_port->setText(QString::number(json.value<int>("LocalPort", int(0))));

    m_interfaces->blockSignals(true);
        auto interface = json.value<QString>("Interface", QString());
        for (int i = 0; i < m_interfaces->count(); i++) {
            if (m_interfaces->itemData(i).toString() == interface) {
                m_interfaces->setCurrentIndex(i); break;
            }
        }
        refreshIpList();
    m_interfaces->blockSignals(false);

    auto ip = json.value<QString>("LocalIP", QString());
    for (int i = 0; i < m_local_ip->count(); i++) {
        if (m_local_ip->itemText(i) == ip) {
            m_local_ip->setCurrentIndex(i);
        }
    }
}

void NetIOWidget::refreshInterfaceList()
{
    auto currentName = m_interfaces->currentData().toString();
    m_interfaces->clear();

    int idx = 0;
    const auto& interfaces = QNetworkInterface::allInterfaces();
    for (int i = 0; i < interfaces.count(); i++) {
        auto name = interfaces[i].name();
        m_interfaces->addItem(QString("%1 (%2)").arg(interfaces[i].humanReadableName(), interfaces[i].hardwareAddress()), name);
        if (name == currentName) {
            idx = i;
        }
    }

    m_interfaces->setCurrentIndex(idx);
}

void NetIOWidget::refreshIpList()
{
    auto name = m_interfaces->currentData().toString();
    auto ip_list = QNetworkInterface::interfaceFromName(name).addressEntries();

    m_local_ip->clear();
    for (int i = 0; i < ip_list.count(); i++) {
        auto str = ip_list[i].ip().toString();
        m_local_ip->addItem(str.mid(0, str.indexOf('%')));
    }
}

void NetIOWidget::createGui()
{
    m_local_ip = new QComboBox();
    m_local_ip->setMaxVisibleItems(10);
    m_local_ip->setStyleSheet("combobox-popup: 0;");

    m_interfaces = new QComboBox();
        m_interfaces->setMaxVisibleItems(10);
        m_interfaces->setStyleSheet("combobox-popup: 0;");
        refreshInterfaceList();

    m_refresh_btn = new QPushButton();
        m_refresh_btn->setIcon(QIcon(":/icons/refresh.svg"));
        m_refresh_btn->setFixedSize(26, 26);

    m_remote_ip = new QLineEdit();
        m_remote_ip->setPlaceholderText(tr("IPv4 or IPv6"));
        m_remote_ip->setText("127.0.0.1");

    auto portValidator = new QIntValidator(0, 65535, this);
    m_remote_port = new QLineEdit();
        m_remote_port->setValidator(portValidator);
        m_remote_port->setPlaceholderText(tr("0 to 65535"));
        m_remote_port->setText("0");

    m_local_port = new QLineEdit();
        m_local_port->setValidator(portValidator);
        m_local_port->setPlaceholderText(tr("0 to 65535"));
        m_local_port->setText("0");

    m_protocol = new QComboBox();
        m_protocol->addItem("TCP");
        m_protocol->addItem("UDP");

    m_mode = new QComboBox();
        m_mode->addItem("Client");
        m_mode->addItem("Server");

    if (m_protocol->currentText() == "UDP") {
            m_mode->setDisabled(true);
    }

    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop);
        main_layout->addWidget(new QLabel(tr("Network configurations:")), 0, 0, 1, 3);
        main_layout->addWidget(new QLabel(tr("Interface")), 1, 0, 1, 1);
        main_layout->addWidget(m_interfaces, 1, 1, 1, 1);
        main_layout->addWidget(m_refresh_btn, 1, 2, 1, 1);
        main_layout->addWidget(new QLabel(tr("Local IP")), 2, 0, 1, 1);
        main_layout->addWidget(m_local_ip, 2, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Local port")), 3, 0, 1, 1);
        main_layout->addWidget(m_local_port, 3, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Protocol")), 4, 0, 1, 1);
        main_layout->addWidget(m_protocol, 4, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Mode")), 5, 0, 1, 1);
        main_layout->addWidget(m_mode, 5, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Remote IP")), 6, 0, 1, 1);
        main_layout->addWidget(m_remote_ip, 6, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Remote port")), 7, 0, 1, 1);
        main_layout->addWidget(m_remote_port, 7, 1, 1, 1);

    setLayout(main_layout);
}

void NetIOWidget::connectSignals()
{
    connect(m_interfaces, &QComboBox::currentTextChanged, this, [this](){
        refreshIpList();
    });

    connect(m_protocol, &QComboBox::currentTextChanged, this, [this](const QString& text) {
        if (m_protocol->currentText() == "UDP") {
            m_mode->setDisabled(true);
        }
        else {
            m_mode->setEnabled(true);
        }
    });

    connect(m_remote_ip, &QLineEdit::editingFinished, this, [this]() {
        QHostAddress address =  QHostAddress(m_remote_ip->text());

        if (address.isNull()) {
            m_remote_ip->setStyleSheet("QLineEdit { background: rgb(255, 170, 170); }");
        }
        else {
            m_remote_ip->setStyleSheet("QLineEdit { background: rgb(255, 255, 255); }");
        }
    });

    connect(m_refresh_btn, &QPushButton::clicked, this, &NetIOWidget::refreshInterfaceList);
}
