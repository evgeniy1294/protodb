#pragma once

#include <protodb/ScriptInterface.h>

#include <sol/sol.hpp>

class LuaScriptInterface final: public ScriptInterface
{
    Q_OBJECT

public:
    LuaScriptInterface(QObject* parent = nullptr);
   ~LuaScriptInterface() = default;

    bool setScriptFile(const QString& path) override;
    bool isValid() const override;

    // Compile code to byte array
    QByteArray compileCode(QString& code) const override;

    // Custom event handler
    bool handleEvent(Event event, QByteArray& bytes) override;

    QString language() const override;

private:
    bool m_valid = false;

    sol::state m_lua;
    QString m_script_file_path;

    sol::protected_function m_start;
    sol::protected_function m_stop;
    sol::protected_function m_transmit;
    sol::protected_function m_received;
};
