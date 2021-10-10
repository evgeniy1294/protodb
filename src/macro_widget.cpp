#include "macro_widget.h"

#include <QLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include "macro_widget.h"

QLineEdit* mLineEdit;
QComboBox* mComboBox;

MacroWidget::MacroWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();
}


MacroWidget::~MacroWidget()
{
}


void MacroWidget::createGui()
{
  auto mNameLe = new QLineEdit("Sequence name");
    mNameLe->setReadOnly(true);

  auto mTrigLe = new QLineEdit("Trigger sequence");

  auto mRepeatSb = new QSpinBox();
    mRepeatSb->setMaximum(10000);
    mRepeatSb->setSingleStep(100);
    mRepeatSb->setValue(0);
    mRepeatSb->setSuffix("ms");
    mRepeatSb->setSpecialValueText(tr("No Repeat"));

  auto mSendBtn  = new QPushButton();
    mSendBtn->setIcon(QIcon(":/icons/enter.png"));

  QGridLayout* layout = new QGridLayout(this);
    layout->setAlignment(Qt::AlignTop);


    layout->addWidget(mNameLe, 0, 0);
    layout->addWidget(mTrigLe, 0, 1);
    layout->addWidget(mRepeatSb, 0, 2);
    layout->addWidget(mSendBtn, 0, 3);
}


