#include <QFile>
#include "Logger.h"
#include "Core.h"

static Logger* s_logger = nullptr;


Core::Core(QObject* parent)
    : QObject(parent)
    , m_logger(new Logger(this))
{
}

void Core::init()
{
    m_lua.open_libraries(sol::lib::base);

    auto log = m_lua["log"].get_or_create<sol::table>();
        log.new_usertype<Logger>("log",
            "print", static_cast<void (Logger::*)(const char *)>(&Logger::comment),
            "clear", &Logger::clear);

    m_lua["log"] = m_logger;
}

void Core::start()
{
    if (QFile::exists("my_script.lua")) {
        m_lua.script_file("my_script.lua");
        m_lua_start  = m_lua["start"];
        m_lua_stop   = m_lua["stop"];
        m_lua_input  = m_lua["input"];
        m_lua_output = m_lua["output"];

        if (m_lua_start.valid()) {
            m_lua_start();
        }
    }
}

void Core::stop()
{
    if (m_lua_stop.valid()) {
        m_lua_stop.call();
    }
}

Logger* Core::logger()
{
    return m_logger;
}
