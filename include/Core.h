#pragma once

#include <QObject>
#include <sol/sol.hpp>

class Logger;

class Core: public QObject
{
    Q_OBJECT

public:
    Core(QObject* parent = nullptr);
    void init();
    void start();
    void stop();
    void setScriptFile(const QString& file);
    Logger* logger();

private:
    QString m_script_file;
    Logger* m_logger;
    sol::state m_lua;
    sol::function m_lua_start;
    sol::function m_lua_stop;
    sol::function m_lua_input;
    sol::function m_lua_output;
};
