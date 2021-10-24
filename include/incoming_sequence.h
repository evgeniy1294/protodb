#pragma once

#include <QString>

class IncomingSequence//: public Sequence
{
public:
    IncomingSequence();

    inline bool active() { return  mActive; };
    const QString& triggedName() const { return mTriggedName; }
    void setTriggedName(const QString& aTriggedName) { mTriggedName = aTriggedName; }

private:
    bool mActive;
    QString mTriggedName;
};
