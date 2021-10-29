#pragma once

#include "SequenceAbstract.h"

class OutgoingSequence: public SequenceAbstract
{
public:
    OutgoingSequence()
        : SequenceAbstract()
        , m_repeat_period(0) { }

    inline int repeatPeriod() const { return m_repeat_period; }
    void setRepeatPeriod(int a_period) { m_repeat_period = a_period; }
private:
    int m_repeat_period;
};

