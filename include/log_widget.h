#pragma once

#include <QWidget>

class QTextBrowser;
class QPushButton;
class QLabel;
class LogConfigWidget;

class LogWidget: public QWidget
{
  Q_OBJECT

public:
  explicit LogWidget(QWidget* parent = nullptr);
  ~LogWidget();

private:
  void createGui();

private slots:
  //void colorDialog();
  void showDialog();

private:
  QTextBrowser* mLogBrowser;
  QPushButton* mRxBtn;
  QPushButton* mTxBtn;
  QLabel* mStatusLabel;
  QLabel* mConfigLabel;
  QLabel* mModeLabel;
  LogConfigWidget* mDialog;
};

