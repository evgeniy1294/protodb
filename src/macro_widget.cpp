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

void SequenceMultiWidget::addSequenceSlot(const QUuid& aUuid, int aPos)
{
  QPointer<Sequence> ptr = Singleton::instance().mSequenceStorage.getSequence(aPos);

  if (!ptr.isNull()) {
    SequenceBox box(*ptr);

    auto row = mLayout->rowCount();

    for (int col = 0; col < box.count(); col++) {
      mLayout->addWidget(box.get(col), row, col);
    }
  }
  else
  {
    qDebug() << "Pointer on sequence is NULL";
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


