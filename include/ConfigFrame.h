#pragma once

#include <QFrame>
#include <Configurable.h>

class ConfigFrame: QFrame, Configurable
{
public:
    ConfigFrame(QWidget* parent = nullptr)
        : QFrame(parent) {  }
};

