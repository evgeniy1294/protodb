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
  void addSequenceSlot(const QUuid& aUuid, int aIndex);
  void removeSequenceSlot(const QUuid& aUuid, int aIndex);
  void ClearSlot();

private:
  void createGui();

private:
  QGridLayout* mLayout;
  QList< QSharedPointer<SequenceBox> > mSqBoxList;
};
