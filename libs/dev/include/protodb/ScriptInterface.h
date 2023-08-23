#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <QObject>

namespace protodb
{

class PROTODB_DEV_EXPORT ScriptInterface: public QObject
{
    Q_OBJECT

public:
    enum Event { Start = 0, Stop, Received, Transmit };

public:
    ScriptInterface(QObject* parent = nullptr)
        : QObject(parent) {}

   ~ScriptInterface() = default;

    virtual QString fileExtention() const = 0;

    virtual bool setScriptFile(const QString& path) = 0;
    virtual QString scriptFile() const = 0;
    virtual bool isValid() const = 0;

    virtual QString syntaxId() const = 0;

    // Custom event handler
    virtual bool handleDataEvent(Event event, QByteArray& bytes) = 0;

    bool handleEvent(Event event) {
        QByteArray dummy;
        return handleDataEvent(event, dummy);
    }

signals:
    void sErrorOccuared(QString detail);
    void sPrint(QString text);
    void sLogClear();
    void sStopSession();
    void sSendBytes(QByteArray bytes);
};

} // protodb
