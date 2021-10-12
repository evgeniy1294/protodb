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

  void addMacroSequence(Sequence* aSequence);

private:
  void createGui();

private:
  QGridLayout* mLayout;
  QList<Sequence> mMacroList;
};
