#pragma once

#include <QWidget>

class QTableView;
class QPushButton;
class QLabel;
class QLineEdit;
class LogConfigWidget;
class LogModel;

class LogWidget: public QWidget
{
  Q_OBJECT

public:
  explicit LogWidget(QWidget* parent = nullptr);
  ~LogWidget();

private:
  void createGui();
  void connectSignals();

private slots:
  //void colorDialog();
  void showDialog();

private:
  QTableView* mLogView;
  QPushButton* mModeBtn;
  QPushButton* mClrBtn;
  QPushButton* mChangeStateBtn;
  QPushButton* mConfigBtn;
  QLineEdit* mFindLe;
  QLineEdit* mMessageLe;
  LogConfigWidget* mDialog;
  LogModel* mLogModel;
};

