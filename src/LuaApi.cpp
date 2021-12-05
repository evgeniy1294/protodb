#include "LuaApi.h"

#include <iostream>

static int exceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
    // L is the lua state, which you can wrap in a state_view if necessary
    // maybe_exception will contain exception, if it exists
    // description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
    std::cout << "An exception occurred in a function, here's what it says ";
    if (maybe_exception) {
        std::cout << "(straight from the exception): ";
        const std::exception& ex = *maybe_exception;
        std::cout << ex.what() << std::endl;
    }
    else {
        std::cout << "(from the description parameter): ";
        std::cout.write(description.data(), static_cast<std::streamsize>(description.size()));
        std::cout << std::endl;
    }

    // you must push 1 element onto the stack to be
    // transported through as the error object in Lua
    // note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
    // so we push a single string (in our case, the description of the error)
    return sol::stack::push(L, description);
}


/*
 * Каждый lua-скрипт может иметь следующие функции:
 *      start - вызывается при запуске сессии
 *      stop  - вызывается при остановке сессии
 *      beforeTransmit - вызывается перед отправкой каждого пакета
 *      afterReceive - вызывается после приёма каждого пакета
 *      buildMessage - вызывается для пакета, помеченного тегом LUA в DSL
 */

LuaApi::LuaApi(QObject* parent)
    : QObject(parent)
{
    m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
    m_lua.set_exception_handler(exceptionHandler);

    auto log = m_lua["log"].get_or_create<sol::table>();
    log.new_usertype<LuaApi>("log",
        "print", &LuaApi::logPrint,
        "clear", &LuaApi::logClear);

    m_lua["log"] = this;
}

void LuaApi::setScriptFile(const QString& path)
{
    m_file_path = path;
}

const QString& LuaApi::scriptFile() const
{
    return m_file_path;
}

bool LuaApi::loadScript()
{
    bool ret = false;

    if (QFile::exists(m_file_path)) {
        sol::protected_function_result pfr = m_lua.safe_script_file(m_file_path.toStdString(), &sol::script_pass_on_error);

        if (pfr.valid()) {
            m_start           = m_lua["start"];
            m_stop            = m_lua["stop"];
            m_before_transmit = m_lua["beforeTransmit"];
            m_after_receive   = m_lua["afterReceive"];
            m_build_message   = m_lua["buildMessage"];

            ret = true;
        }
        else {
            sol::error err = pfr;
            emit sLogError(err.what());
        }
    }

    if (!ret) {
        m_start           = sol::nil;
        m_stop            = sol::nil;
        m_before_transmit = sol::nil;
        m_after_receive   = sol::nil;
        m_build_message   = sol::nil;
    }

    return ret;
}

bool LuaApi::loadScript(const QString& path)
{
    setScriptFile(path);
    return loadScript();
}

void LuaApi::start()
{
    if (m_start.valid()) {
       sol::protected_function_result pfr = m_start();

       if (!pfr.valid()) {
           sol::error err = pfr;
           emit sLogError(err.what());
       }
    }
}


void LuaApi::stop()
{
    if (m_stop.valid()) {
        sol::protected_function_result pfr = m_stop();

        if (!pfr.valid()) {
            sol::error err = pfr;
            emit sLogError(err.what());
        }
     }
}

void LuaApi::beforeTransmit(QVector<uint8_t>& data)
{
    if (m_before_transmit.valid()) {
        sol::protected_function_result pfr = m_before_transmit(data);

        if (!pfr.valid()) {
            sol::error err = pfr;
            emit sLogError(err.what());
        }
     }
}

void LuaApi::afterReceive(QByteArray& data)
{
    if (m_after_receive.valid()) {
        sol::protected_function_result pfr = m_after_receive(data);

        if (!pfr.valid()) {
            sol::error err = pfr;
            emit sLogError(err.what());
        }
     }
}

void LuaApi::buildMessage(QByteArray& data)
{
    if (m_build_message.valid()) {
        sol::protected_function_result pfr = m_build_message(data);

        if (!pfr.valid()) {
            sol::error err = pfr;
            emit sLogError(err.what());
        }
     }
}

void LuaApi::logPrint(const char *c_str)
{
    emit sLogPrint(QByteArray(c_str));
}

void LuaApi::logClear()
{
    emit sLogClear();
}
