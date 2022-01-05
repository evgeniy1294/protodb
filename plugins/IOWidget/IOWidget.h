#pragma once

#include <QWidget>
#include <protodb/configurable/Configurable.h>

class IOWidget: public QWidget, public Configurable
{
    Q_OBJECT

public:
    explicit IOWidget(QWidget* parent = nullptr)
        : QWidget(parent) {    }

    virtual ~IOWidget() = default;
};
