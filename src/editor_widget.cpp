#include <QWidget>
#include <QLayout>
#include <QComboBox>
#include <QTextBrowser>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "editor_widget.h"

QTextBrowser* mTextBrowser;
QLineEdit* mLineEdit;
QComboBox* mComboBox;

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

  auto mSaveBtn  = new QPushButton("Save");
  auto mSendBtn  = new QPushButton("Send");

  QGridLayout* layout = new QGridLayout(this);
    layout->setAlignment(Qt::AlignTop);


    layout->addWidget(mNameLe, 0, 0);
    layout->addWidget(mStatusLabel, 1, 0);
    layout->addWidget(mDataEditor, 2, 0, 3, 1);
    layout->addWidget(mSaveBtn, 0, 1);
    layout->addWidget(mCrcCb, 2, 1);
    layout->addWidget(mRepeatSb, 3, 1, Qt::AlignTop);
    layout->addWidget(mSendBtn, 4, 1);
}

