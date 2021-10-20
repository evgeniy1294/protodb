#include <QPointer>
#include <QDebug>
#include "sequence_storage.h"


void SqStorage::append(const Sequence& aSequence)
{
  mList.append(aSequence);
  emit sSeqAppended(aSequence.uuid(), mList.size() - 1);
}

void SqStorage::insert(int i, const Sequence& aSequence)
{
  mList.insert(i, aSequence);
  emit sSeqAppended(aSequence.uuid(), i);
}

void SqStorage::clear()
{
  mList.clear();
  emit sCleared();
}

void SqStorage::remove(int aIndex)
{
  if (aIndex >= 0 && aIndex < mList.size()) {
    QUuid uuid = mList.at(aIndex).uuid();
    mList.removeAt(aIndex);

    emit sSeqRemoved(uuid, aIndex);
  }
}

void SqStorage::remove(const QUuid& aUuid)
{
  auto index = find(aUuid);
  if (index != -1) {
    mList.removeAt(index);
    emit sSeqRemoved(aUuid, index);
  }
}

void SqStorage::remove(const QPointer<Sequence>& aPtr)
{
  QUuid uuid = aPtr->uuid();
  int index = find(aPtr);

  if (index != -1) {
    mList.removeAt(index);
    emit sSeqRemoved(uuid, index);
  }
}

qsizetype SqStorage::size() const
{
  return mList.size();
}

bool SqStorage::empty() const
{
  return mList.empty();
}

const QUuid SqStorage::getUuid(int aIndex) const
{
  QUuid uuid{};

  if (aIndex >= 0 && aIndex < mList.size()) {
    uuid = mList.at(aIndex).uuid();
  }

  return uuid;
}

int SqStorage::find(const QPointer<Sequence>& aPtr) const
{
  return mList.indexOf(*aPtr);
}

int SqStorage::find(const QUuid& aUuid) const
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

QPointer<Sequence> SqStorage::getSequence(const QUuid& aUuid)
{
  QPointer<Sequence> ptr;
  int index = find(aUuid);

  if (index != -1) {
    ptr = QPointer<Sequence>(&mList[index]);
  }

  return ptr;
}

QPointer<Sequence> SqStorage::getSequence(int aIndex)
{
  //qDebug() << aIndex;
  QPointer<Sequence> ptr;
  if (aIndex >= 0 && aIndex < mList.size()) {
    ptr = QPointer<Sequence>(&mList[aIndex]);
  }

  return ptr;
}


