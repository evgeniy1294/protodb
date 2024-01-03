#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/configurable/Configurable.h>

#include <QWidget>

namespace protodb
{

class Connection;

class PROTODB_DEV_EXPORT ConnectionAdvanceControlWidget: public QWidget
{
    Q_OBJECT

public:
    explicit ConnectionAdvanceControlWidget(QWidget* parent = nullptr)
        : QWidget(parent) {    }
    virtual ~ConnectionAdvanceControlWidget() = default;

    virtual QString deviceCID() const = 0;

    virtual void setConnection(QSharedPointer<Connection> conn) = 0;
};

} // namespace protodb
