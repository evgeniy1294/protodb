#pragma once

#include <QWidget>
#include <QList>
#include <QSharedPointer>

#include "sequence_box.h"

class QGridLayout;

class SequenceMultiWidget: public QWidget
{
  Q_OBJECT

public:
  explicit SequenceMultiWidget(QWidget* parent = nullptr);
  ~SequenceMultiWidget();

public slots:
  void addSequence(const QUuid& aUuid, int aIndex);
  void removeSequence(const QUuid& aUuid, int aIndex);
  void clear();

  void removeBox(const QPointer<Sequence>& aSq);

private:
  void createGui();

private:
  QGridLayout* mLayout;
  QList< QSharedPointer<SequenceBox> > mSqBoxList;
};
