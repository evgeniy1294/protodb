#pragma once

#include <QtCore>

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "SequenceAbstract.h"

class LuaApi: public QObject {
    Q_OBJECT

public:
    LuaApi(QObject* parent = nullptr);

    void setScriptFile(const QString& path);
    const QString& scriptFile() const;

    bool loadScript();
    bool loadScript(const QString& path);

public:
    //--------- [LUA API] -----------//
    void start();
    void stop();
    void beforeTransmit(QByteArray& data);
    void afterReceive(QByteArray& data);
    void buildMessage(QByteArray& data);

    //---------- [C++ API] ----------//
    void print();

private:
    sol::state m_lua;
    QString m_file_path;

    sol::function m_start;
    sol::function m_stop;
    sol::function m_before_transmit;
    sol::function m_after_receive;
    sol::function m_build_message;
};
