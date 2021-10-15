#pragma once

#include <QObject>
#include <QList>
#include <QUuid>
#include "sequence.h"

class SqStorage: public QObject {
  Q_OBJECT

public:
  void append(const Sequence& aSequence);
  void clear();
  void remove(int aPos);
  void remove(const QUuid& aUuid);
  void remove(const QPointer<Sequence>& aPtr);

  qsizetype size() const;
  bool empty() const;

  const QUuid getUuid(int aPos) const;

  int find(const QPointer<Sequence>& aPtr) const;
  int find(const QUuid& aUuid) const;

  QPointer<Sequence> getSequence(const QUuid& aUuid);
  QPointer<Sequence> getSequence(int aPos);

signals:
  void sSeqAppended(const QUuid& aUuid, int aPos);
  void sSeqRemoved (const QUuid& aUuid, int aPos);
  void sCleared();

private:
  QList<Sequence> mList;
};
