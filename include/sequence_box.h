#pragma once

#include <QObject>
#include <QPointer>

class QWidget;
class QToolButton;
class QLineEdit;
class QSpinBox;
class Sequence;

class SequenceBox: public QObject {
  Q_OBJECT

public:
  SequenceBox();
  SequenceBox(const Sequence& aSequence);
  // SequenceBox(QPointer<Sequence> aSequence);
  qsizetype count() const;
  QWidget* get(qsizetype aWgtId);

private:
  //QPointer<Sequence> mSqPtr;

  QToolButton* mToolButton;
  QLineEdit*   mNameWgt;
  QLineEdit*   mTriggeredNameWgt;
  QSpinBox*    mRepeatTimeWgt;
};
