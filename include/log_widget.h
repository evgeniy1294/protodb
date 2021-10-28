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

private slots:
  //void colorDialog();
  void showDialog();

private:
  QTableView* mLogView;
  QPushButton* mModeBtn;
  QLabel* mStatusLabel;
  QLabel* mConfigLabel;
  QLabel* mModeLabel;
  QLineEdit* mFindLe;
  LogConfigWidget* mDialog;
  LogModel* mLogModel;
};

