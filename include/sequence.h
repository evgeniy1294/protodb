#pragma once

#include <QString>
#include <QByteArray>
#include <QUuid>

class SequenceProcessor;

class Sequence {
  friend class SequenceProcessor;

public:
  Sequence()
   : mUuid(QUuid::createUuid())
   , mName()
   , mCharString()
   , mCached(false)
   , mRepeatPeriod(0)
   , mByteArray()
   , mTrigSequence()
   , mDescription()
   , mTriggerName() {  }

  void setName(const QString& newName);
  const QString& name() const;

  const QString& charString() const;
  void setCharString(const QString& newCharString);

  int repeatPeriod() const;
  void setRepeatPeriod(int newRepeatPeriod);

  Sequence* trigSequence() const;
  void setTrigSequence(Sequence* newTrigSequence);

  const QString& description() const;
  void setDescription(const QString& newDescription);

  const QString& triggerName() const;
  void setTriggerName(const QString& newTriggerName);

private:
  bool mCached;
  QByteArray mByteArray;

private:
  QUuid mUuid;

  QString mName;
  QString mCharString;
  Sequence* mTrigSequence;
  QString mDescription;
  QString mTriggerName; // ?
  int mRepeatPeriod;
};
