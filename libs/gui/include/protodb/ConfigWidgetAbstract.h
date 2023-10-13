#pragma once

#include <protodb/export/gui_cfg.hpp>

#include <nlohmann/json_fwd.hpp>

#include <QWidget>

namespace protodb
{

class PROTODB_GUI_EXPORT ConfigWidgetAbstract: public QWidget
{
    Q_OBJECT

public:
    ConfigWidgetAbstract(QWidget* parent = nullptr) : QWidget(parent) {};
    ~ConfigWidgetAbstract() = default;

    virtual void setConfig(const nlohmann::json& cfg) { return; }
    virtual void config(nlohmann::json& cfg) const { return; }

signals:
    void sConfigChanged();
};


} // namespace protodb
