#pragma once

#include "IOWidget.h"

class QLineEdit;
class QComboBox;

class NetIOWidget: public IOWidget
{
  Q_OBJECT

public:
    NetIOWidget(QWidget *parent = nullptr);
    ~NetIOWidget() = default;

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
    QLineEdit* m_ip;
    QLineEdit* m_port;
    QComboBox* m_protocol;
};

