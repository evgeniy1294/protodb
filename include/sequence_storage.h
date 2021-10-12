#pragma once

#include <QObject>
#include <QList>
#include <QUuid>
#include "sequence.h"

class SequenceStorage: public QObject {
  Q_OBJECT

public:
  void append(const Sequence& aSequence);
  void clear();
  void remove(const QUuid& aUuid);
  void remove(int aPos);
  void remove(const QPointer<Sequence>& aPtr);

  qsizetype size();

  QUuid getUuid(int aPos) const;
  int find(const QPointer<Sequence>& aPtr);
  int find(const QUuid& aUuid);

  QPointer<Sequence> getSequence(QUuid aUuid) const;
  QPointer<Sequence> getSequence(int aPos) const;

signals:
  void sSeqAppended(const QUuid& aUuid, int aPos);
  void sSeqRemoved (const QUuid& aUuid, int aPos);
  void sCleared();

private:
  QList<Sequence> mList;
};
