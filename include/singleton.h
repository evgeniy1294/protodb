#pragma once

#include "settings_dialog.h"
#include "Core.h"

class Singleton
{
public:
    SettingsDialog mSettingsDialog;
    Core m_core;

    static Singleton& instance();
private:
    Singleton() = default;
};
