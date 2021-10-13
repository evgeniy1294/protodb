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

void SequenceMultiWidget::addSequenceSlot(const QUuid& aUuid, int aIndex)
{
  QPointer<Sequence> ptr = Singleton::instance().mSequenceStorage.getSequence(aIndex);

  if (!ptr.isNull()) {
    auto box = QSharedPointer<SequenceBox>::create(*ptr);
      mSqBoxList.insert(aIndex, box);

    auto row = mLayout->rowCount();
    mLayout->addWidget(new QLabel(QString::number(row)), row, 0);

    for (int col = 1; col < box->count()+1; col++) {
      mLayout->addWidget(box->get(col-1), row, col);
    }
  }
}

void SequenceMultiWidget::removeSequenceSlot(const QUuid& aUuid, int aPos)
{

}

void SequenceMultiWidget::ClearSlot()
{

}




void SequenceMultiWidget::createGui()
{
  mLayout = new QGridLayout(this);
    mLayout->setAlignment(Qt::AlignTop);
}


