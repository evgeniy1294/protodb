#pragma once

#include <QWidget>

class QTextBrowser;

class LogWidget: public QWidget
{
  Q_OBJECT

public:
  explicit LogWidget(QWidget* parent = nullptr);
  ~LogWidget();

private:
  void createGui();

private:
  QTextBrowser* mLogBrowser;
  QTextBrowser* mScriptBrowser;
};
