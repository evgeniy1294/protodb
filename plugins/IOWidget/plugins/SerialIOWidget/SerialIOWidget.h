#pragma once

#include "IOWidget.h"

class QComboBox;
class QValidator;

class SerialIOWIdget: public IOWidget
{
  Q_OBJECT

public:
    SerialIOWIdget(QWidget *parent = nullptr);
    ~SerialIOWIdget() = default;

    const QString& name() const override;
    const QString& description() const override;

    void defaultConfig(nlohmann::json &json) const override;
    void toJson(nlohmann::json &json) const override;
    void fromJson(const nlohmann::json &json) override;

private:
    void createGui();
    void connectSignals();

private:
    QString m_name;
    QString m_description;

    QValidator* m_baudrate_validator;
    QComboBox* m_baudrate;
    QComboBox* m_flow_ctrl;
    QComboBox* m_data_bits;
    QComboBox* m_parity;
    QComboBox* m_stop_bits;
    QComboBox* m_open_mode;
    QComboBox* m_device;
};
