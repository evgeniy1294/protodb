#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QUuid>

class SequenceProcessor;

class Sequence: public QObject {
  Q_OBJECT

public:
  friend class SequenceProcessor;

  enum Type {
      Incoming,
      Outgoing
  };

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

  Sequence(const Sequence& aSeq)
    : mUuid(aSeq.mUuid)
    , mName(aSeq.mName)
    , mCharString(aSeq.mCharString)
    , mCached(aSeq.mCached)
    , mRepeatPeriod(aSeq.mRepeatPeriod)
    , mByteArray(aSeq.mByteArray)
    , mTrigSequence(aSeq.mTrigSequence)
    , mDescription(aSeq.mDescription)
    , mTriggerName(aSeq.mTriggerName) {  }

  Sequence& operator=(const Sequence&);
  bool operator==(const Sequence&);

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

  const QUuid& uuid() const;
  inline Type type() const { return static_cast<Type>(mType); }

private:
  bool mCached;
  QByteArray mByteArray;

private:
  QUuid mUuid;
  ushort mType;

  int mRepeatPeriod;
  QString mName;
  QString mCharString;
  QString mDescription;
  QString mTriggerName;
  Sequence* mTrigSequence;
};
