#pragma once

#include "ConfigFrame.h"

class QLineEdit;
class QComboBox;

class NetConfigFrame: public ConfigFrame
{
  Q_OBJECT

public:
    NetConfigFrame(QWidget *parent = nullptr);
    ~NetConfigFrame() = default;

    void defaultConfig(nlohmann::json &json) const override;
    void toJson(nlohmann::json &json) const override;
    void fromJson(const nlohmann::json &json) override;

    const QString& jsonPrefix() const override;

private:
    void createGui();
    void connectSignals();

private:
    QLineEdit* m_ip;
    QLineEdit* m_port;
    QComboBox* m_protocol;
};

