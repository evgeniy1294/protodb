#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/configurable/Configurable.h>

#include <QWidget>

namespace protodb
{

class PROTODB_DEV_EXPORT IOWidget: public QWidget, public Configurable
{
    Q_OBJECT

public:
    explicit IOWidget(QWidget* parent = nullptr)
        : QWidget(parent) {    }
    virtual ~IOWidget() = default;

    virtual QString name() const = 0;
};

} // namespace protodb
