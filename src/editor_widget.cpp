#include <QLayout>
#include <QComboBox>
#include <QTextBrowser>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include "editor_widget.h"


EditorWidget::EditorWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();
}


EditorWidget::~EditorWidget()
{
}


void EditorWidget::createGui()
{
  mNameLe    = new QLineEdit("Sequence name");

  mStatusLabel = new QLabel(tr("HEX, pos 4/7"));

  mHelpEditor = new QTextBrowser();
    mHelpEditor->setText(tr("Description..."));

  mDataEditor = new QTextBrowser();
    mDataEditor->setText(tr("x50 | x30 | x34 | xDE | xAD | xBE | xEF"));

  mCrcCb = new QComboBox();
    mCrcCb->addItem("No CRC");
    mCrcCb->addItem("CRC16 Modbus");
    mCrcCb->addItem("CRC32");

  mRepeatSb = new QSpinBox();
    mRepeatSb->setMaximum(10000);
    mRepeatSb->setSingleStep(100);
    mRepeatSb->setValue(0);
    mRepeatSb->setSuffix("ms");
    mRepeatSb->setSpecialValueText(tr("No Repeat"));

  auto mSaveBtn  = new QPushButton();
    mSaveBtn->setIcon(QIcon(":/icons/save.svg"));

  QGridLayout* layout = new QGridLayout(this);
    layout->setAlignment(Qt::AlignTop);


    layout->addWidget(mNameLe, 0, 0);
    layout->addWidget(mSaveBtn, 0, 1);

    layout->addWidget(mHelpEditor, 1, 0);
    layout->addWidget(mStatusLabel, 2, 0);
    layout->addWidget(mDataEditor, 3, 0);
}

