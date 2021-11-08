#pragma once

#include <QWidget>

class QTableView;
class QPushButton;
class QLabel;
class QLineEdit;
class ConnectionConfigDialog;
class LogTableModel;
class Logger;

class LogWidget: public QWidget
{
  Q_OBJECT

public:
  explicit LogWidget(QWidget* parent = nullptr);
  ~LogWidget() = default;

private:
  void createGui();
  void connectSignals();

private:
  QTableView* m_view;
  QPushButton* m_mode_btn;
  QPushButton* m_clear_btn;
  QPushButton* m_run;
  QPushButton* m_config_btn;
  QLineEdit* m_find_le;
  QLineEdit* m_msg_le;
  LogTableModel* m_model;
  ConnectionConfigDialog* m_conn_dialog;
};

