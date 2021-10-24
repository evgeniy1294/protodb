#pragma once

class OutgoingSequence//: public Sequence
{
public:
    OutgoingSequence();

    inline int repeatPeriod() const { return mRepeatPeriod; }
    void setRepeatPeriod(int aRepeatPeriod) { mRepeatPeriod = aRepeatPeriod; }
private:
    int mRepeatPeriod;
};

