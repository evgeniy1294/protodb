#include "protodb/LuaScriptInterface.h"
#include "protodb/LuaInterfaceDetail.h"

#include <protodb/utils/SolByteArrayWrapper.h>

#include <filesystem>

using namespace protodb;

LuaInterface::LuaInterface(QObject* parent)
    : ScriptInterface(parent)
    , d_ptr( new LuaInterfacePrivate() )
{
    Q_D(LuaInterface);
        d->q_ptr = this;

    m_lua.open_libraries(sol::lib::base, sol::lib::bit32, sol::lib::math, sol::lib::string);
    d->setExceptionHandler(m_lua);
    d->bindAll(m_lua);
}

LuaInterface::~LuaInterface()
{
    Q_D(LuaInterface);
        delete(d);
}

QString LuaInterface::fileExtention() const
{
    return ".lua";
}

QString LuaInterface::syntaxId() const
{
    return "LUA";
}

bool LuaInterface::setScriptFile(const QString& path)
{
    std::filesystem::path script_path = path.toLocal8Bit().toStdString();
    m_valid = false;

    if (std::filesystem::exists(script_path)) {
        if (script_path.extension() == fileExtention().toStdString()) {
            m_env = sol::environment(m_lua, sol::create, m_lua.globals());
            sol::protected_function_result pfr =
                    m_lua.safe_script_file(script_path.string(), m_env, &sol::script_pass_on_error);

            if (pfr.valid()) {
                m_start    = m_env["start"];
                m_stop     = m_env["stop"];
                m_transmit = m_env["beforeTransmit"];
                m_received = m_env["afterReceive"];

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

QString LuaInterface::scriptFile() const
{
    return m_script_file_path;
}

bool LuaInterface::isValid() const
{
    return m_valid;
}

QByteArray LuaInterface::compileCode(const QString& code) const
{
    const Q_D(LuaInterface);

    static const QString m_template = {
        "function compile(bytes) \n %1 \n end"
    };

    sol::state compiler;
    if (code.length() != 0) {
        compiler.open_libraries(sol::lib::base, sol::lib::bit32, sol::lib::math, sol::lib::string);
        d->setExceptionHandler(compiler);

        sol::protected_function_result pfr =
                compiler.safe_script(m_template.arg(code).toStdString(), &sol::script_pass_on_error);

        compiler["vec"] = std::vector<uint8_t>();

        if (pfr.valid()) {
            sol::safe_function compile = compiler["compile"];
                pfr = compile( compiler["vec"] );

            if (pfr.valid()) {
                std::vector<uint8_t>& bytes = compiler["vec"];
                return QByteArray(reinterpret_cast<const char*>(bytes.data()), static_cast<int>(bytes.size()));
            }
        }

        if (!pfr.valid()) {
            sol::error err = pfr;
            std::cout << err.what() << std::endl;
        }
    }

    return QByteArray();
}

bool LuaInterface::handleDataEvent(Event event, QByteArray& bytes)
{
    Q_D(LuaInterface);

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
            m_env = sol::nil;
            d->deleteUserObjects();

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
