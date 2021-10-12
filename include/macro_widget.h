#pragma once

#include <QWidget>
#include <QList>

#include "sequence.h"

class QGridLayout;


class SequenceMultiWidget: public QWidget
{
  Q_OBJECT

public:
  explicit SequenceMultiWidget(QWidget* parent = nullptr);
  ~SequenceMultiWidget();

public slots:
  void addSequenceSlot(const QUuid& aUuid, int aPos);
  void removeSequenceSlot(const QUuid& aUuid, int aPos);
  void ClearSlot();

private:
  void createGui();

private:
  QGridLayout* mLayout;
};
