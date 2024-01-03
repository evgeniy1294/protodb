#pragma once

#include <protodb/ConnectionConfigWidget.h>

class QComboBox;
class QLineEdit;
class QSpinBox;
class QPushButton;

namespace protodb {

class NetworkConfigWidget final: public ConnectionConfigWidget
{
  Q_OBJECT

public:
    NetworkConfigWidget(QWidget *parent = nullptr);
    ~NetworkConfigWidget() = default;

    QString name() const override { return QString("Network"); }
    QString gcid() const override;
    QString deviceCID() const override;

    void setConfig(const nlohmann::json& json) override;
    void config(nlohmann::json& json) const override;
    void defaultConfig(nlohmann::json& json) const override;

private:
    void createGui();
    void connectSignals();
    void refreshInterfaceList();
    void refreshIpList();

private:
    QComboBox* m_mode;
    QComboBox* m_protocol;
    QComboBox* m_interfaces;
    QComboBox* m_local_ip;
    QLineEdit* m_remote_ip;
    QSpinBox* m_remote_port;
    QSpinBox* m_local_port;
    QPushButton* m_refresh_btn;
};

} // namespace protodb
