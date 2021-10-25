#pragma once

#include "settings_dialog.h"

class Singleton
{
public:
  SettingsDialog mSettingsDialog;

  static Singleton& instance();
private:
    Singleton() = default;
};
