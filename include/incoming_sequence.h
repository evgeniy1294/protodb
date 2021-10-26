#pragma once

#include <QString>

class IncomingSequence//: public Sequence
{
public:
    IncomingSequence();

    inline bool active() { return  mActive; };
    void setActive(bool aFlag) { mActive = aFlag; }

    const QString& triggedName() const { return mTriggedName; }
    void setTriggedName(const QString& aTriggedName) { mTriggedName = aTriggedName; }

    int delayPeriod() const { return mDelayPeriod; }
    void setDelayPeriod(int aPeriod) { mDelayPeriod = aPeriod; }

private:
    bool mActive;
    int mDelayPeriod;
    QString mTriggedName;
};
