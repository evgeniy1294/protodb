#pragma once

#include <protodb/IOWidget.h>

class QComboBox;
class QValidator;
class QPushButton;

namespace protodb {

class SerialIOWidget final: public IOWidget
{
  Q_OBJECT

public:
    SerialIOWidget(QWidget *parent = nullptr);
    ~SerialIOWidget() = default;

    QString name() const override { return QString("Serial"); }
    virtual QString gcid() const override;
    QString deviceCID() const override;

    void setConfig(const nlohmann::json& json) override;
    void config(nlohmann::json& json) const override;
    void defaultConfig(nlohmann::json& json) const override;

private:
    void createGui();
    void connectSignals();
    void refreshPortList();
    bool event(QEvent* e) override;

private:
    QValidator* m_baudrate_validator;
    QComboBox* m_baudrate;
    QComboBox* m_flow_ctrl;
    QComboBox* m_data_bits;
    QComboBox* m_parity;
    QComboBox* m_stop_bits;
    QComboBox* m_open_mode;
    QComboBox* m_device;
    QPushButton* m_refresh_btn;
};

} // namespace protodb
