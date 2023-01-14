#pragma once

#include <QWidget>
#include <protodb/configurable/Configurable.h>

class QCheckBox;
class QLineEdit;

namespace protodb {

class LogFormatWidget: public QWidget, public Configurable
{
public:
    explicit LogFormatWidget(QWidget* parent = nullptr);
    virtual ~LogFormatWidget() = default;

    void config(nlohmann::json& json) const override;
    void defaultConfig(nlohmann::json& json) const override;
    void setConfig(const nlohmann::json& json) override;

private:
    void createGui();
    void connectSignals();

private:
    QCheckBox* m_append_en;
    QCheckBox* m_timestamp_en;
    QCheckBox* m_ch1_en;
    QCheckBox* m_ch2_en;
    QCheckBox* m_cmt_en;
    QCheckBox* m_ch_name_en;
    QLineEdit* m_timestamp_format;
    QLineEdit* m_ch1_name;
    QLineEdit* m_ch2_name;
    QLineEdit* m_cmt_name;
    QLineEdit* m_separator;
};

} // namespace protodb
