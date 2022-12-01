#pragma once

#include <QObject>

namespace protodb
{

class ScriptInterface: public QObject
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

    // Compile code to byte array
    virtual QByteArray compileCode(const QString& code) const = 0;

    // Custom event handler
    virtual bool handleDataEvent(Event event, QByteArray& bytes) = 0;

    bool handleEvent(Event event) {
        QByteArray dummy;
        return handleDataEvent(event, dummy);
    }

signals:
    void sErrorOccuared(QString detail);
    void sPrint(QString text);
};

} // protodb
