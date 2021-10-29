#pragma once

#include <QWidget>

class QTableView;
class QPushButton;
class QLabel;
class QLineEdit;
class ConnectionConfigDialog;
class LogTableModel;

class LogWidget: public QWidget
{
  Q_OBJECT

public:
  explicit LogWidget(QWidget* parent = nullptr);
  ~LogWidget();

private:
  void createGui();
  void connectSignals();

private:
  QTableView* mLogView;
  QPushButton* mModeBtn;
  QPushButton* mClrBtn;
  QPushButton* mChangeStateBtn;
  QPushButton* mConfigBtn;
  QLineEdit* mFindLe;
  QLineEdit* mMessageLe;
  ConnectionConfigDialog* m_conn_dialog;
  LogTableModel* mLogModel;
};

