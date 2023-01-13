#include "protodb/LuaScriptInterface.h"

#include <protodb/utils/SolByteArrayWrapper.h>

#include <QDebug>

#include <iostream>
#include <filesystem>
#include <tuple>

using namespace protodb;

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

    initStandartFunction();
}

QString LuaScriptInterface::fileExtention() const
{
    return ".lua";
}

QString LuaScriptInterface::syntaxId() const
{
    return "LUA";
}

void LuaScriptInterface::print(const char* str)
{
    emit sPrint(QString(str));
}

void LuaScriptInterface::log_clear()
{
    emit sLogClear();
}

void LuaScriptInterface::initStandartFunction()
{
    // log:print()
    auto log = m_lua["log"].get_or_create<sol::table>();
    log.new_usertype<LuaScriptInterface>("log",
        "print", &LuaScriptInterface::print,
        "clear", &LuaScriptInterface::log_clear
    );

    m_lua["log"] = this;

    // session
    auto session = m_lua["session"].get_or_create<sol::table>();

    // session: stop
    session.set_function("stop", [this]() {
        emit sStopSession();
    });

    // Utils
    auto utils = m_lua["utils"].get_or_create<sol::table>();

    // utils: tableToFloat
    utils.set_function("tableToFloat",
        [](const sol::table& parent, const sol::table& table) -> float
        {
            Q_UNUSED(parent)
            auto ret = std::numeric_limits<float>::quiet_NaN();

            if (table.get_type() == sol::type::table) {
                if (table.size() >= 4) {
                    uint32_t tmp = 0;

                    tmp |= table.raw_get_or<uint8_t>(1, 0);
                    tmp |= table.raw_get_or<uint8_t>(2, 0) << 8;
                    tmp |= table.raw_get_or<uint8_t>(3, 0) << 16;
                    tmp |= table.raw_get_or<uint8_t>(4, 0) << 24;

                    ret = *reinterpret_cast<float*>(&tmp);
                }
            }

            return ret;
        }
    );

    // utils: tableToDouble
    utils.set_function("tableToDouble",
        [](const sol::table& parent, const sol::table& table) -> double
        {
            Q_UNUSED(parent)
            auto ret = std::numeric_limits<float>::quiet_NaN();

            if (table.get_type() == sol::type::table) {
                if (table.size() >= 8) {
                    uint64_t tmp = 0;

                    tmp |=  table.raw_get_or<uint64_t>(1, 0) & 0xff;
                    tmp |= (table.raw_get_or<uint64_t>(2, 0) & 0xff) << 8;
                    tmp |= (table.raw_get_or<uint64_t>(3, 0) & 0xff) << 16;
                    tmp |= (table.raw_get_or<uint64_t>(4, 0) & 0xff) << 24;
                    tmp |= (table.raw_get_or<uint64_t>(5, 0) & 0xff) << 32;
                    tmp |= (table.raw_get_or<uint64_t>(6, 0) & 0xff) << 40;
                    tmp |= (table.raw_get_or<uint64_t>(7, 0) & 0xff) << 48;
                    tmp |= (table.raw_get_or<uint64_t>(8, 0) & 0xff) << 56;

                    ret = *reinterpret_cast<double*>(&tmp);
                }
            }

            return ret;
        }
    );

    // utils: floatToBytes
    utils.set_function("floatToBytes", [](float val) {
        uint32_t tmp = *reinterpret_cast<uint32_t*>(&val);
            uint8_t b1 = tmp & 0xff;
            uint8_t b2 = (tmp >> 8)  & 0xff;
            uint8_t b3 = (tmp >> 16) & 0xff;
            uint8_t b4 = (tmp >> 24) & 0xff;

        return std::make_tuple(b1, b2, b3, b4);
    });

    // utils: doubleToBytes
    utils.set_function("doubleToBytes", [](double val) {
        uint64_t tmp = *reinterpret_cast<uint64_t*>(&val);
            uint8_t b1 = tmp & 0xff;
            uint8_t b2 = (tmp >> 8)  & 0xff;
            uint8_t b3 = (tmp >> 16) & 0xff;
            uint8_t b4 = (tmp >> 24) & 0xff;
            uint8_t b5 = (tmp >> 32) & 0xff;
            uint8_t b6 = (tmp >> 40) & 0xff;
            uint8_t b7 = (tmp >> 48) & 0xff;
            uint8_t b8 = (tmp >> 56) & 0xff;

        return std::make_tuple(b1, b2, b3, b4, b5, b6, b7, b8);
    });

}

bool LuaScriptInterface::setScriptFile(const QString& path)
{
    std::filesystem::path script_path = path.toStdString();
    m_valid = false;

    if (std::filesystem::exists(script_path)) {
        if (script_path.extension() == fileExtention().toStdString()) {
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

QString LuaScriptInterface::scriptFile() const
{
    return m_script_file_path;
}

bool LuaScriptInterface::isValid() const
{
    return m_valid;
}

QByteArray LuaScriptInterface::compileCode(const QString& code) const
{
    static const QString m_template = {
        "function compile(bytes) \n %1 \n end"
    };

    sol::state compiler;

    if (code.length() != 0) {
        compiler.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);
        compiler.set_exception_handler(exceptionHandler);

        sol::protected_function_result pfr =
                compiler.safe_script(m_template.arg(code).toStdString(), &sol::script_pass_on_error);

        compiler["vec"] = std::vector<uint8_t>();

        if (pfr.valid()) {
            pfr = compiler["compile"]( compiler["vec"] );

            if (pfr.valid()) {
                std::vector<uint8_t>& bytes = compiler["vec"];
                return QByteArray(reinterpret_cast<const char*>(bytes.data()), bytes.size());
            }
        }

        if (!pfr.valid()) {
            sol::error err = pfr;
            std::cout << err.what() << std::endl;
        }
    }

    return QByteArray();
}

bool LuaScriptInterface::handleDataEvent(Event event, QByteArray& bytes)
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
