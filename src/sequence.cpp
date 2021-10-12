#include "sequence.h"

Sequence& Sequence::operator=(const Sequence& aSq)
{
  mUuid = aSq.mUuid;
  mName = aSq.mName;
  mCharString = aSq.mCharString;
  mTrigSequence = aSq.mTrigSequence;
  mDescription = aSq.mDescription;
  mTriggerName = aSq.mTriggerName;
  mRepeatPeriod = aSq.mRepeatPeriod;

  return *this;
}

bool Sequence::operator==(const Sequence& aSq)
{
  return mUuid == aSq.mUuid;
}

void Sequence::setName(const QString& newName)
{
  mName = newName;
}

const QString& Sequence::name() const
{
  return mName;
}

const QString& Sequence::charString() const
{
  return mCharString;
}

void Sequence::setCharString(const QString& newCharString)
{
  mCharString = newCharString;
}

int Sequence::repeatPeriod() const
{
  return mRepeatPeriod;
}

void Sequence::setRepeatPeriod(int newRepeatPeriod)
{
  mRepeatPeriod = newRepeatPeriod;
}

Sequence* Sequence::trigSequence() const
{
  return mTrigSequence;
}

void Sequence::setTrigSequence(Sequence* newTrigSequence)
{
  mTrigSequence = newTrigSequence;
}

const QString& Sequence::description() const
{
  return mDescription;
}

void Sequence::setDescription(const QString& newDescription)
{
  mDescription = newDescription;
}

const QString& Sequence::triggerName() const
{
  return mTriggerName;
}

void Sequence::setTriggerName(const QString& newTriggerName)
{
  mTriggerName = newTriggerName;
}

const QUuid& Sequence::uuid() const
{
  return mUuid;
}
