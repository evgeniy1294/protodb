#include "protodb/NetIOWidget.h"
#include "protodb/NetIOWidgetCreator.h"

#include <protodb/utils/JsonUtils.h>

#include <protodb/NetIODeviceCreator.h>

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

void NetIOWidget::defaultConfig(nlohmann::json& json) const
{
    json["CID"] = NetIODeviceCreator::creatorId();

    nlohmann::json attr;
        attr["RemoteIp"] = "localhost";
        attr["Port"]     = "0";
        attr["Protocol"] = "TCP";
    json["Attributes"] = attr;
}

void NetIOWidget::config(nlohmann::json& json) const
{
    json["CID"] = "NetIODeviceCreator";

    nlohmann::json attr;
        attr["RemoteIp"] = m_ip->text();
        attr["Port"]     = m_port->text();
        attr["Protocol"] = m_protocol->currentText();
    json["Attributes"] = attr;
}

void NetIOWidget::setConfig(const nlohmann::json& json)
{
    nlohmann::json attr = json.value<nlohmann::json>("Attributes", nlohmann::json());

    m_ip->setText(json.value<QString>("RemoteIp", QString()));
    m_port->setText(json.value<QString>("Port", QString()));
    m_protocol->setCurrentText(json.value<QString>("Protocol", QString()));
}

void NetIOWidget::createGui()
{
    m_ip = new QLineEdit();
        m_ip->setPlaceholderText(tr("IPv4 or IPv6"));
        m_ip->setText("localhost");

    m_port = new QLineEdit();
        auto portValidator = new QIntValidator(0, 65535, this);
        m_port->setValidator(portValidator);
        m_port->setPlaceholderText(tr("0 to 65535"));
        m_port->setText("0");

    m_protocol = new QComboBox();
        m_protocol->addItem("TCP");
        m_protocol->addItem("UDP");

    auto main_layout = new QGridLayout();
        main_layout->setAlignment(Qt::AlignTop);
        main_layout->addWidget(new QLabel(tr("Network configurations:")), 0, 0, 1, 3);
        main_layout->addWidget(new QLabel(tr("Address")), 1, 0);
        main_layout->addWidget(m_ip, 1, 1);
        main_layout->addWidget(m_protocol, 1, 2);
        main_layout->addWidget(new QLabel(tr("Port")), 2, 0);
        main_layout->addWidget(m_port, 2, 1);

    setLayout(main_layout);
}

void NetIOWidget::connectSignals()
{
    connect(m_ip, &QLineEdit::editingFinished, this, [this]() {
        QHostAddress address =  QHostAddress(m_ip->text());
        if (address.isNull()) {
            m_ip->setPlaceholderText(tr("Invalid Address"));
            m_ip->clear();
        }
    });
}
