#pragma once

#include <QFrame>
#include <Configurable.h>

class ConfigFrame: public QFrame, public Configurable
{
public:
    ConfigFrame(QWidget* parent = nullptr)
        : QFrame(parent) {  }
};

