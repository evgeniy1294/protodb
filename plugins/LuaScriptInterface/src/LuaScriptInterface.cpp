#include "LuaScriptInterface.h"

#include <protodb/utils/SolByteArrayWrapper.h>



#include <iostream>
#include <filesystem>

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


LuaScriptInterface::LuaScriptInterface(QObject* parent)
    : ScriptInterface(parent)
{
    m_lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
    m_lua.set_exception_handler(exceptionHandler);

    auto log = m_lua["log"].get_or_create<sol::table>();
    log.new_usertype<LuaScriptInterface>("log",
        "print", &LuaScriptInterface::sPrint);

    m_lua["log"] = this;
}

bool LuaScriptInterface::setScriptFile(const QString& path)
{
    std::filesystem::path script_path = path.toStdString();
    m_valid = false;

    if (std::filesystem::exists(script_path)) {
        if (script_path.extension() == ".lua") {
            sol::protected_function_result pfr = m_lua.safe_script_file(script_path, &sol::script_pass_on_error);

            if (pfr.valid()) {
                m_start    = m_lua["start"];
                m_stop     = m_lua["stop"];
                m_transmit = m_lua["beforeTransmit"];
                m_received = m_lua["afterReceive"];

                m_valid = true;
            }
            else {
                sol::error err = pfr;
                emit sErrorOccuared(err.what());
            }
        }
    }

    if (!m_valid) {
        m_start    = sol::nil;
        m_stop     = sol::nil;
        m_transmit = sol::nil;
        m_received = sol::nil;
    }

    return m_valid;
}

bool LuaScriptInterface::isValid() const
{
    return m_valid;
}

QByteArray LuaScriptInterface::compileCode(QString& code) const
{
    static const QString m_template = {
        "function compile(bytes) \n %1 \n end"
    };

    QByteArray ret;
    sol::state compiler;

    if (code.length() != 0) {
        compiler.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
        compiler.set_exception_handler(exceptionHandler);

        sol::protected_function_result pfr =
                compiler.safe_script(m_template.arg(code).toStdString(), &sol::script_pass_on_error);

        if (pfr.valid()) {
            pfr = compiler["compile"](ret);
        }
    }

    return ret;
}

bool LuaScriptInterface::handleEvent(Event event, QByteArray& bytes)
{
    switch(event) {
        case Start: {
            if (m_start.valid()) {
               sol::protected_function_result pfr = m_start();

               if (!pfr.valid()) {
                   sol::error err = pfr;
                   emit sErrorOccuared(err.what());
               }
            }
        } break;

        case Stop: {
            if (m_stop.valid()) {
                sol::protected_function_result pfr = m_stop();

                if (!pfr.valid()) {
                    sol::error err = pfr;
                    emit sErrorOccuared(err.what());
                }
             }
        } break;

        case Transmit: {
            if (m_transmit.valid()) {
                SolByteArrayWrapper wrapper(bytes);
                sol::protected_function_result pfr = m_transmit(wrapper);

                if (!pfr.valid()) {
                    sol::error err = pfr;
                    emit sErrorOccuared(err.what());
                }
             }
        } break;

        case Received: {
            if (m_received.valid()) {
                SolByteArrayWrapper wrapper(bytes);
                sol::protected_function_result pfr = m_received(wrapper);

                if (!pfr.valid()) {
                    sol::error err = pfr;
                    emit sErrorOccuared(err.what());
                }
             }
        } break;

        default:
            emit sErrorOccuared("Unknown event");
    }

    return false;
}
