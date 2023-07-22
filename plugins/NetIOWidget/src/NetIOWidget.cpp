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
    json["RemotePort"] = "0";
    json["LocalPort"]  = "0";
}

void NetIOWidget::config(nlohmann::json& json) const
{
    json["gcid"] = NetIOWIdgetCreator::creatorId();
    json["Mode"] = m_mode->currentText();
    json["Protocol"] = m_protocol->currentText();
    json["RemoteIP"]   = m_remote_ip->text();
    json["RemotePort"] = m_remote_port->text();
    json["LocalPort"] = m_local_port->text();
}

void NetIOWidget::setConfig(const nlohmann::json& json)
{
    m_mode->setCurrentText(json.value<QString>("Mode", QString("Client")));
    m_protocol->setCurrentText(json.value<QString>("Protocol", QString("TCP")));
    m_remote_ip->setText(json.value<QString>("RemoteIP", QString("127.0.0.1")));
    m_remote_port->setText(json.value<QString>("RemotePort", QString("0")));
    m_local_port->setText(json.value<QString>("LocalPort", QString("0")));
}

void NetIOWidget::createGui()
{
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

    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop);
        main_layout->addWidget(new QLabel(tr("Network configurations:")), 0, 0, 1, 3);
        main_layout->addWidget(new QLabel(tr("Mode")), 1, 0, 1, 1);
        main_layout->addWidget(m_mode, 1, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Protocol")), 2, 0, 1, 1);
        main_layout->addWidget(m_protocol, 2, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Remote IP")), 3, 0, 1, 1);
        main_layout->addWidget(m_remote_ip, 3, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Remote port")), 4, 0, 1, 1);
        main_layout->addWidget(m_remote_port, 4, 1, 1, 1);
        main_layout->addWidget(new QLabel(tr("Local port")), 5, 0, 1, 1);
        main_layout->addWidget(m_local_port, 5, 1, 1, 1);

    setLayout(main_layout);
}

void NetIOWidget::connectSignals()
{
    connect(m_remote_ip, &QLineEdit::editingFinished, this, [this]() {
        QHostAddress address =  QHostAddress(m_remote_ip->text());
        if (address.isNull()) {
            m_remote_ip->setPlaceholderText(tr("Invalid Address"));
            m_remote_ip->clear();
        }
    });
}
