#pragma once

#include <QWidget>
#include <Configurable.h>

class IOWidget: public QWidget, Configurable
{
    Q_OBJECT

public:
    explicit IOWidget(QWidget* parent = nullptr)
        : QWidget(parent) {    }

    virtual ~IOWidget() = default;
};
