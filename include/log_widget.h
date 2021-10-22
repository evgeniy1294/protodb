#pragma once

#include <QWidget>

class QTextBrowser;
class QPushButton;

class LogWidget: public QWidget
{
  Q_OBJECT

public:
  explicit LogWidget(QWidget* parent = nullptr);
  ~LogWidget();

private:
  void createGui();

private slots:
  void colorDialog();

private:
  QTextBrowser* mLogBrowser;
  QPushButton* mRxBtn;
  QPushButton* mTxBtn;
};

