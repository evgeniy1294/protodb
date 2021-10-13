#pragma once

#include <QObject>
#include <QPointer>

class QWidget;
class QToolButton;
class QLineEdit;
class QSpinBox;
class QMenu;
class QAction;
class Sequence;

class SequenceBox: public QObject {
  Q_OBJECT

public:
  SequenceBox(const QPointer<Sequence>& aSequence);
  SequenceBox(const Sequence& aSequence);
  ~SequenceBox();

  int count() const;
  QWidget* get(int aWgtId);

private:
  void createActions();
  void createMenu();
  void createGui(const Sequence& aSequence);

private:
  QPointer<Sequence> mSqPtr;

  // ---------[Actions]--------
  QAction* mSendAct;
  QAction* mEditAct;
  QAction* mRemoveAct;

  // ----------[Menu]----------
  QMenu* mToolMenu;

  // --------[Widgets]----------
  QPointer<QToolButton> mToolButton;
  QPointer<QLineEdit>   mNameWgt;
  QPointer<QLineEdit>   mTriggeredNameWgt;
  QPointer<QSpinBox>    mRepeatTimeWgt;
};
