#include <QWidget>
#include <QTranslator>
#include <QToolButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QMenu>
#include "sequence.h"
#include "sequence_box.h"

SequenceBox::SequenceBox(const Sequence& aSequence)
{
  mNameWgt = new QLineEdit(aSequence.name());
    mNameWgt->setReadOnly(true);

  mTriggeredNameWgt = new QLineEdit(aSequence.triggerName());

  mRepeatTimeWgt = new QSpinBox();
    mRepeatTimeWgt->setMaximum(10000);
    mRepeatTimeWgt->setSingleStep(100);
    mRepeatTimeWgt->setValue(0);
    mRepeatTimeWgt->setSuffix("ms");
    mRepeatTimeWgt->setSpecialValueText(QObject::tr("No Repeat"));

  auto menu = new QMenu();
    menu->addAction(new QAction("Send"));
    menu->addAction(new QAction("Show in Editor..."));
    menu->addSeparator();
    menu->addAction(new QAction("Remove"));

  mToolButton  = new QToolButton();
    mToolButton->setIcon(QIcon(":/icons/enter.png"));
    mToolButton->setMenu(menu);
}

qsizetype SequenceBox::count() const
{
  return 4;
}

QWidget* SequenceBox::get(qsizetype aWgtId)
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

