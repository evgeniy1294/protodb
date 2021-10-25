#pragma once

#include "sequence.h"

class OutgoingSequence: public Sequence
{
public:
    OutgoingSequence()
        : Sequence()
        , mRepeatPeriod(0)
    {

    }

    inline int repeatPeriod() const { return mRepeatPeriod; }
    void setRepeatPeriod(int aRepeatPeriod) { mRepeatPeriod = aRepeatPeriod; }
private:
    int mRepeatPeriod;
};

