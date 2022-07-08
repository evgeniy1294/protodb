#pragma once

#include "Sequence.h"

#include <QObject>

#include <system_error>

class SequenceProcessor: public QObject
{
    Q_OBJECT

public:
    enum Event {
        StartEvent,
        StopEvent,
        BeforeTransmitEvent,
        AfterReceiveEvent,
    };

public:
    virtual void setHandlersCode(const QString& code, std::error_code& ec) = 0;
    virtual void setFiltersCode (const QString& code, std::error_code& ec) = 0;
    virtual void makeSequence(Sequence& seq, std::error_code& ec) = 0;
    virtual bool acceptSequence(const Sequence& seq) = 0;
    virtual void handleEvent (Event event, const Sequence& seq = Sequence()) = 0;

signals:
    void print(QString text);
};
