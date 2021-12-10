#pragma once

#include "ConfigFrame.h"

class QCheckBox;
class QLineEdit;

class LogFormatFrame: public ConfigFrame
{
    Q_OBJECT

public:
    LogFormatFrame(QWidget* aParent = nullptr);
    ~LogFormatFrame() = default;

    void defaultConfig(nlohmann::json& json) const override;
    void fromJson(const nlohmann::json& json) override;
    void toJson(nlohmann::json& json) const override;

private:
    void createGui();
    void connectSignals();

private:
    QCheckBox* m_append_en;
    QCheckBox* m_timestamp_en;
    QCheckBox* m_ch1_en;
    QCheckBox* m_ch2_en;
    QLineEdit* m_timestamp_format;
    QLineEdit* m_ch1_name;
    QLineEdit* m_ch2_name;
    QLineEdit* m_separator;
};

