#include <QLineEdit>
#include <QComboBox>
#include <QLayout>
#include <QLabel>
#include <QIntValidator>
#include <QHostAddress>

#include "qnlohmann.h"
#include "NetConfigFrame.h"

NetConfigFrame::NetConfigFrame(QWidget* parent)
    : ConfigFrame(parent)
{
    createGui();
    connectSignals();
}

void NetConfigFrame::defaultConfig(nlohmann::json& json) const
{
    json["RemoteIp"] = "127.0.0.1";
    json["Port"]     = "1";
    json["Protocol"] = "TCP";
}

void NetConfigFrame::toJson(nlohmann::json& json) const
{
    json["RemoteIp"] = m_ip->text();
    json["Port"]     = m_port->text();
    json["Protocol"] = m_protocol->currentText();
}

void NetConfigFrame::fromJson(const nlohmann::json& json)
{
    m_ip->setText(json["RemoteIp"].get<QString>());
    m_port->setText(json["Port"].get<QString>());
    m_protocol->setCurrentText(json["Protocol"].get<QString>());
}

const QString& NetConfigFrame::jsonPrefix() const
{
    static const QString m_prefix("Network");
    return m_prefix;
}

void NetConfigFrame::createGui()
{
    m_ip = new QLineEdit();
        m_ip->setPlaceholderText(tr("IPv4 or IPv6"));

    m_port = new QLineEdit();
        auto portValidator = new QIntValidator(1, 65535, this);
        m_port->setValidator(portValidator);
        m_port->setPlaceholderText(tr("1 to 65535"));

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
    setFrameShape(QFrame::StyledPanel);
    setFrameShadow(QFrame::Raised);
}

void NetConfigFrame::connectSignals()
{
    connect(m_ip, &QLineEdit::editingFinished, this, [this]() {
        QHostAddress address =  QHostAddress(m_ip->text());
        if (address.isNull()) {
            m_ip->setPlaceholderText(tr("Invalid Address"));
            m_ip->clear();
        }
    });
}
