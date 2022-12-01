#pragma once

#include <protodb/IOWidget.h>

class QLineEdit;
class QComboBox;

namespace protodb {

class NetIOWidget final: public IOWidget
{
  Q_OBJECT

public:
    NetIOWidget(QWidget *parent = nullptr);
    ~NetIOWidget() = default;

    QString name() const override { return QString("Network"); }
    QString gcid() const override;

    void setConfig(const nlohmann::json& json) override;
    void config(nlohmann::json& json) const override;
    void defaultConfig(nlohmann::json& json) const override;

private:
    void createGui();
    void connectSignals();

private:
    QLineEdit* m_ip;
    QLineEdit* m_port;
    QComboBox* m_protocol;
};

} // namespace protodb
