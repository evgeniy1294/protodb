#include "LuaApi.h"

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
        m_lua.script_file(m_file_path.toStdString());

        m_start           = m_lua["start"];
        m_stop            = m_lua["stop"];
        m_before_transmit = m_lua["beforeTransmit"];
        m_after_receive   = m_lua["afterReceive"];
        m_build_message   = m_lua["buildMessage"];

        ret = true;
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
        m_start();
    }
}

void LuaApi::stop()
{
    if (m_stop.valid()) {
        m_stop();
    }
}

void LuaApi::beforeTransmit(QByteArray& data)
{
    if (m_before_transmit.valid()) {
        m_lua["__tx"] = data;
        m_before_transmit(m_lua["__tx"]);
    }
}

void LuaApi::afterReceive(QByteArray& data)
{
    if (m_after_receive.valid()) {
        m_lua["__rx"] = data;
        m_after_receive(m_lua["__rx"]);
    }
}

void LuaApi::buildMessage(QByteArray& data)
{
    if (m_build_message.valid()) {
        m_lua["__msg"] = data;
        m_build_message(m_lua["__msg"]);
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
