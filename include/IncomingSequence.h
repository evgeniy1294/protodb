#pragma once

#include <QString>
#include "SequenceAbstract.h"

class IncomingSequence: public SequenceAbstract
{
public:
    IncomingSequence();

    inline bool active() { return  m_active; };
    void setActive(bool a_active) { m_active = a_active; }

    const QString& triggedName() const { return m_trigger_name; }
    void setTriggedName(const QString& a_name) { m_trigger_name = a_name; }

    int delayPeriod() const { return m_delay_period; }
    void setDelayPeriod(int a_period) { m_delay_period = a_period; }

private:
    bool m_active;
    int m_delay_period;
    QString m_trigger_name;
};
