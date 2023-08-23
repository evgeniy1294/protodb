#pragma once

#include <protodb/ScriptInterface.h>

#include <sol/sol.hpp>

namespace protodb {

class LuaInterfacePrivate;

class LuaInterface final: public ScriptInterface
{
    Q_OBJECT

public:
    LuaInterface(QObject* parent = nullptr);
   ~LuaInterface();

    virtual QString fileExtention() const override;

    bool setScriptFile(const QString& path) override;
    QString scriptFile() const override;
    bool isValid() const override;

    // Custom event handler
    bool handleDataEvent(Event event, QByteArray& bytes) override;

    QString syntaxId() const override;

private:
    Q_DECLARE_PRIVATE(LuaInterface)
    LuaInterfacePrivate* const d_ptr;

    bool m_valid = false;

    sol::state m_lua;
    QString m_script_file_path;

    sol::protected_function m_start;
    sol::protected_function m_stop;
    sol::protected_function m_transmit;
    sol::protected_function m_received;
    sol::environment m_env;
};

} // namespace protodb
