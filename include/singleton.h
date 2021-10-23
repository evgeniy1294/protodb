#pragma once

#include "sequence_storage.h"
#include "settings_dialog.h"

class Singleton
{
public:
  SqStorage mSequenceStorage;
  SettingsDialog mSettingsDialog;

  static Singleton& instance();
private:
    Singleton() = default;
};
