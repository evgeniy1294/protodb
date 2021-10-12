#include <QPointer>
#include <QDebug>
#include "sequence_storage.h"


void SequenceStorage::append(const Sequence& aSequence)
{
  mList.append(aSequence);
  emit sSeqAppended(aSequence.uuid(), mList.size() - 1);
}

void SequenceStorage::clear()
{
  mList.clear();
  emit sCleared();
}

void SequenceStorage::remove(int aIndex)
{
  if (aIndex >= 0 && aIndex < mList.size()) {
    QUuid uuid = mList.at(aIndex).uuid();
    mList[aIndex];

    emit sSeqRemoved(uuid, aIndex);
  }
}

void SequenceStorage::remove(const QUuid& aUuid)
{
  auto index = find(aUuid);
  if (index != -1) {
    mList.removeAt(index);
    emit sSeqRemoved(aUuid, index);
  }
}

void SequenceStorage::remove(const QPointer<Sequence>& aPtr)
{
  QUuid uuid = aPtr->uuid();
  int index = find(aPtr);

  if (index != -1) {
    mList.removeAt(index);
    emit sSeqRemoved(uuid, index);
  }
}

qsizetype SequenceStorage::size() const
{
  return mList.size();
}

bool SequenceStorage::empty() const
{
  return mList.empty();
}

const QUuid SequenceStorage::getUuid(int aIndex) const
{
  QUuid uuid{};

  if (aIndex >= 0 && aIndex < mList.size()) {
    uuid = mList.at(aIndex).uuid();
  }

  return uuid;
}

int SequenceStorage::find(const QPointer<Sequence>& aPtr) const
{
  return mList.indexOf(*aPtr);
}

int SequenceStorage::find(const QUuid& aUuid) const
{
  int index = -1;

  for (int i = 0; i < mList.size(); i++) {
    if (mList[i].uuid() == aUuid) {
      index = i;
      break;
    }
  }

  return index;
}

QPointer<Sequence> SequenceStorage::getSequence(const QUuid& aUuid)
{
  QPointer<Sequence> ptr;
  int index = find(aUuid);

  if (index != -1) {
    ptr = QPointer<Sequence>(&mList[index]);
  }

  return ptr;
}

QPointer<Sequence> SequenceStorage::getSequence(int aIndex)
{
  qDebug() << aIndex;
  QPointer<Sequence> ptr;
  if (aIndex >= 0 && aIndex < mList.size()) {
    ptr = QPointer<Sequence>(&mList[aIndex]);
  }

  return ptr;
}


