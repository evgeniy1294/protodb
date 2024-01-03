#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/configurable/Configurable.h>

#include <QWidget>

namespace protodb
{

class PROTODB_DEV_EXPORT ConnectionConfigWidget: public QWidget, public Configurable
{
    Q_OBJECT

public:
    explicit ConnectionConfigWidget(QWidget* parent = nullptr)
        : QWidget(parent) {    }
    virtual ~ConnectionConfigWidget() = default;

    virtual QString name() const = 0;
    virtual QString deviceCID() const = 0;
};

} // namespace protodb
