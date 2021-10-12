#include "macro_widget.h"

#include <QLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QToolButton>
#include <QMenu>
#include <QAction>
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

void SequenceMultiWidget::addMacroSequence(Sequence* aSequence)
{
  SequenceBox box(aSequence);

  auto row = mLayout->rowCount();

  for (int col = 0; col < box.count(); col++) {
    mLayout->addWidget(box.get(col), row, col);
  }
}


void SequenceMultiWidget::createGui()
{
  mLayout = new QGridLayout(this);
    mLayout->setAlignment(Qt::AlignTop);
}


