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

    auto row = mLayout->rowCount();
    for (int col = 0; col < box->count(); col++) {
      mLayout->addWidget(box->get(col), row, col);
    }
  }
}

void SequenceMultiWidget::removeSequence(const QUuid& aUuid, int aIndex)
{
  mSqBoxList.removeAt(aIndex);

  QLayoutItem* row = mLayout->takeAt(aIndex);
    mLayout->removeItem(row);
    delete row;

  for (int i = aIndex; i < mSqBoxList.size(); i++) {
    mSqBoxList[i]->setIdx(i+1);
  }
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


