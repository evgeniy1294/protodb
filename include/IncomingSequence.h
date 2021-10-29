#pragma once

#include <QString>
#include "SequenceAbstract.h"

class IncomingSequence: public SequenceAbstract
{
public:
    IncomingSequence();

    const QString& triggedName() const { return m_trigger_name; }
    void setTriggedName(const QString& a_name) { m_trigger_name = a_name; }

    int delayPeriod() const { return m_delay_period; }
    void setDelayPeriod(int a_period) { m_delay_period = a_period; }

private:
    int m_delay_period;
    QString m_trigger_name;
};
