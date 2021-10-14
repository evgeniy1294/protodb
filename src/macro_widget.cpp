#include <QLayout>
#include <QDebug>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QPointer>
#include "singleton.h"
#include "macro_widget.h"
#include "sequence_box.h"



QLineEdit* mLineEdit;
QComboBox* mComboBox;

SequenceMultiWidget::SequenceMultiWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();
}


SequenceMultiWidget::~SequenceMultiWidget()
{
}

void SequenceMultiWidget::addSequence(const QUuid& aUuid, int aIndex)
{
  QPointer<Sequence> ptr = Singleton::instance().mSequenceStorage.getSequence(aIndex);

  if (!ptr.isNull()) {
    auto box = QSharedPointer<SequenceBox>::create(ptr);
      box->setIdx(aIndex+1);

    mSqBoxList.insert(aIndex, box);
    connect(box.data(), &SequenceBox::sRemoveMe, this, &SequenceMultiWidget::removeBox);

    for (int col = 0; col < box->count(); col++) {
      mLayout->addWidget(box->get(col), aIndex, col);
    }
  }
}

void SequenceMultiWidget::removeSequence(const QUuid& aUuid, int aIndex)
{
  if ( aIndex >= 0 && aIndex < mSqBoxList.size() ) {
    auto box = mSqBoxList.takeAt(aIndex);

    for (int col = 0; col < box->count(); col++) {
      mLayout->removeWidget(box->get(col));
    }

    for (int row = aIndex; row < mSqBoxList.size(); row++) {
      mSqBoxList[row]->setIdx(row+1);

      for (int col = 0; col < box->count(); col++) {
        mLayout->addWidget(mSqBoxList[row]->get(col), row, col);
      }
    }
  }

  return;
}

void SequenceMultiWidget::clear()
{
  QLayoutItem* layoutItem;

  for (int row = 0; row < mLayout->rowCount(); row++) {
    layoutItem = mLayout->takeAt(row);
    layoutItem->widget()->hide();
      delete layoutItem;
  }

  mSqBoxList.clear();
}

void SequenceMultiWidget::removeBox(const QPointer<Sequence>& aSq)
{
  Singleton::instance().mSequenceStorage.remove(aSq);
}


void SequenceMultiWidget::createGui()
{
  mLayout = new QGridLayout(this);
    mLayout->setAlignment(Qt::AlignTop);
}


