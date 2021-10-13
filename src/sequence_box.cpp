#include <QWidget>
#include <QTranslator>
#include <QToolButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QMenu>
#include "sequence.h"
#include "sequence_box.h"

SequenceBox::SequenceBox(const Sequence& aSq)
{
  createGui(aSq);
}


SequenceBox::SequenceBox(const QPointer<Sequence>& aSq)
{
  createGui(*aSq);
}

SequenceBox::~SequenceBox()
{
  if (mToolButton) {
    mToolButton->deleteLater();
  }

  if (mNameWgt) {
    mNameWgt->deleteLater();
  }

  if (mTriggeredNameWgt) {
    mTriggeredNameWgt->deleteLater();
  }

  if (mRepeatTimeWgt) {
    mRepeatTimeWgt->deleteLater();
  }
}

int SequenceBox::count() const
{
  return 4;
}

QWidget* SequenceBox::get(int aWgtId)
{
  QWidget* wgt = nullptr;

  switch(aWgtId) {
    case 0: wgt = mNameWgt;  break;
    case 1: wgt = mTriggeredNameWgt; break;
    case 2: wgt = mRepeatTimeWgt; break;
    case 3: wgt = mToolButton; break;
  }

  return wgt;
}

void SequenceBox::createActions()
{
  mSendAct   = new QAction(QIcon(":/icons/arrow.svg"), tr("&Send"), this);
  mEditAct   = new QAction(QIcon(":/icons/edit.svg") , tr("&Show in editor"), this);
  mRemoveAct = new QAction(QIcon(":/icons/close.svg"), tr("&Remove"), this);
}

void SequenceBox::createMenu()
{
  mToolMenu = new QMenu();
    mToolMenu->addAction(mSendAct);
    mToolMenu->addAction(mEditAct);
    mToolMenu->addSeparator();
    mToolMenu->addAction(mRemoveAct);
}

void SequenceBox::createGui(const Sequence& aSq)
{
  createActions();
  createMenu();

  mNameWgt = new QLineEdit(aSq.name());
    mNameWgt->setReadOnly(true);

  mTriggeredNameWgt = new QLineEdit(aSq.triggerName());

  mRepeatTimeWgt = new QSpinBox();
    mRepeatTimeWgt->setMaximum(10000);
    mRepeatTimeWgt->setSingleStep(100);
    mRepeatTimeWgt->setValue(0);
    mRepeatTimeWgt->setSuffix("ms");
    mRepeatTimeWgt->setSpecialValueText(QObject::tr("No Repeat"));

  mToolButton  = new QToolButton();
    mToolButton->setIcon(QIcon(":/icons/arrow.svg"));
    mToolButton->setMenu(mToolMenu);
}

