#pragma once

#include <QObject>
#include <QPointer>

class QWidget;
class QLabel;
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
  ~SequenceBox();

  int count() const;
  QWidget* get(int aWgtId);

  void setIdx(int aIdx);

public slots:
  void onSendClicked();
  void onEditClicked();
  void onRemoveClicked();

signals:
  void sSend(const QPointer<Sequence>& mSqPtr);
  void sEdit(const QPointer<Sequence>& mSqPtr);
  void sRemoveMe(const QPointer<Sequence>& mSqPtr);

private:
  void createActions();
  void createMenu();
  void createGui();

private:
  QPointer<Sequence> mSq;

  // ---------[Actions]--------
  QAction* mSendAct;
  QAction* mEditAct;
  QAction* mRemoveAct;

  // ----------[Menu]----------
  QMenu* mToolMenu;

  // --------[Widgets]----------
  QPointer<QLabel>      mIdxLabel;
  QPointer<QToolButton> mToolButton;
  QPointer<QLineEdit>   mNameWgt;
  QPointer<QLineEdit>   mTriggeredNameWgt;
  QPointer<QSpinBox>    mRepeatTimeWgt;
};
