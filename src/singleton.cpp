#include "singleton.h"
#include "settings_dialog.h"
#include "Core.h"

Singleton & Singleton::instance()
{
   static Singleton instance;
   return instance;
}

Singleton::Singleton()
{
    m_settings = new SettingsDialog();
    m_core = new Core();
}


