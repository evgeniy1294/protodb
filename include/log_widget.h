#pragma once

#include <QWidget>

class QTableView;
class QPushButton;
class QLabel;
class QLineEdit;
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
  QTableView* mLogView;
  QPushButton* mRxBtn;
  QPushButton* mTxBtn;
  QLabel* mStatusLabel;
  QLabel* mConfigLabel;
  QLabel* mModeLabel;
  QLineEdit* mFindLe;
  LogConfigWidget* mDialog;
};

