#pragma once

#include <nlohmann/json_fwd.hpp>

#include <QWidget>

class QCheckBox;
class QComboBox;
class QSpinBox;

namespace protodb {

class DelimetersConfigWidget final: public QWidget
{
    Q_OBJECT

    static constexpr int kOffValue    = -1;
    static constexpr int kCustomValue = -2;

public:
    explicit DelimetersConfigWidget(QWidget* parent = nullptr);
    virtual ~DelimetersConfigWidget() = default;

    void config(nlohmann::json& json) const;
    void defaultConfig(nlohmann::json& json) const;
    void setConfig(const nlohmann::json& json);

private:
    void createGui();
    void connectSignals();
    int getD1() const;
    int getD2() const;

private:
    QCheckBox* m_use_custom;
    QComboBox* m_del_1;
    QComboBox* m_del_2;
    QSpinBox*  m_guard_int;
};

} // namespace protodb
