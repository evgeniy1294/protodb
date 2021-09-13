#pragma once

#include <QWidget>

class QComboBox;
class QPushButton;
class QLineEdit;

class SerialWidget: public QWidget
{
  Q_OBJECT

public:
  explicit SerialWidget(QWidget* parent = nullptr);
  ~SerialWidget();

private:
  void createGui();

private:
  QComboBox*   mBaudrateCb;
  QComboBox*   mFlowControlCb;
  QComboBox*   mDataBitsCb;
  QComboBox*   mParityCb;
  QComboBox*   mStopBitsCb;
  QComboBox*   mOpenModeCb;
  QComboBox*   mDeviceCb;
  QPushButton* mConnectBtn;
  QPushButton* mLogfileBtn;
  QLineEdit*   mLogfileLe;
};


