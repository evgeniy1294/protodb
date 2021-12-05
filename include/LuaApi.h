#pragma once

#include <QtCore>
#include <QVector>

#include <sol/sol.hpp>

#include "SequenceAbstract.h"

class LuaApi final: public QObject {
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
    void beforeTransmit(QVector<uint8_t>& data);
    void afterReceive(QByteArray& data);
    void buildMessage(QByteArray& data);

signals:
    void sLogPrint(const QByteArray&);
    void sLogClear();

private:
    //--------- [C API] -----------//
    void logPrint(const char* c_str);
    void logClear();

private:
    sol::state m_lua;
    QString m_file_path;

    sol::protected_function m_start;
    sol::protected_function m_stop;
    sol::protected_function m_before_transmit;
    sol::protected_function m_after_receive;
    sol::protected_function m_build_message;
};
