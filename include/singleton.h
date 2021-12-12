#pragma once


class Core;

class Singleton
{
public:
    // SettingsDialog* m_settings;
    Core* m_core;

    static Singleton& instance();
private:
    Singleton();
};
