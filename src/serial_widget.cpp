#include <QWidget>
#include <QLayout>
#include <QLayoutItem>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include "serial_widget.h"


SerialWidget::SerialWidget(QWidget* parent)
  : QWidget(parent)
{
  createGui();

}


SerialWidget::~SerialWidget()
{

}


void SerialWidget::createGui()
{
  QGridLayout* layout = new QGridLayout(this);
    layout->setAlignment(Qt::AlignTop);

    mDeviceCb = new QComboBox();
      mDeviceCb->addItem("/dev/ttyS0");
      mDeviceCb->addItem("/dev/ttyACM0");
      mDeviceCb->addItem("/dev/ttyUSB0");
      mDeviceCb->addItem("/dev/ttyUSB1");

    mBaudrateCb = new QComboBox();
      mBaudrateCb->addItem("1200");
      mBaudrateCb->addItem("4800");
      mBaudrateCb->addItem("9600");
      mBaudrateCb->addItem("19200");
      mBaudrateCb->addItem("38400");
      mBaudrateCb->addItem("57600");
      mBaudrateCb->addItem("115200");
      mBaudrateCb->addItem("Custom");

    mDataBitsCb = new QComboBox();
      mDataBitsCb->addItem("5");
      mDataBitsCb->addItem("6");
      mDataBitsCb->addItem("7");
      mDataBitsCb->addItem("8");

    mFlowControlCb = new QComboBox();
      mFlowControlCb->addItem("None");
      mFlowControlCb->addItem("Hardware");
      mFlowControlCb->addItem("Software");

    mParityCb = new QComboBox();
      mParityCb->addItem("None");
      mParityCb->addItem("Even");
      mParityCb->addItem("Odd");
      mParityCb->addItem("Space");
      mParityCb->addItem("Mark");

    mStopBitsCb = new QComboBox();
      mStopBitsCb->addItem("1");
      mStopBitsCb->addItem("2");

    mOpenModeCb = new QComboBox();
      mOpenModeCb->addItem("Read/write");
      mOpenModeCb->addItem("Read");
      mOpenModeCb->addItem("Write");
      mOpenModeCb->addItem("Sniffer");

    mLogfileLe  = new QLineEdit(tr("/home/evgen/log.txt"));
    mConnectBtn = new QPushButton(tr("Open"));
    mLogfileBtn = new QPushButton(tr("..."));

    //layout->setSizeConstraint(QLayout::SetMinimumSize);
    layout->setColumnStretch(1, 0);
    layout->addWidget(new QLabel(tr("Device")));
    layout->addWidget(mDeviceCb);
    layout->addWidget(new QLabel(tr("Open mode")));
    layout->addWidget(mOpenModeCb);
    layout->addWidget(new QLabel(tr("Baudrate")));
    layout->addWidget(mBaudrateCb);
    layout->addWidget(new QLabel(tr("Data Bits")));
    layout->addWidget(mDataBitsCb);
    layout->addWidget(new QLabel(tr("Stop Bits")));
    layout->addWidget(mStopBitsCb);
    layout->addWidget(new QLabel(tr("Parity")));
    layout->addWidget(mParityCb);
    layout->addWidget(new QLabel(tr("Flow Control")));
    layout->addWidget(mFlowControlCb);
    layout->addWidget(new QLabel(tr("Log file:")));
    layout->addWidget(new QWidget());
    layout->addWidget(mLogfileBtn);
    layout->addWidget(mLogfileLe);
    layout->addWidget(new QWidget());
    layout->addWidget(mConnectBtn);
}

