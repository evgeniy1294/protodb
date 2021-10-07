#pragma once

#include <QWidget>

class QTextBrowser;

class WorkAreaWidget: public QWidget
{
  Q_OBJECT

public:
  explicit WorkAreaWidget(QWidget* parent = nullptr);
  ~WorkAreaWidget();

private:
  void createGui();

private:
  QTextBrowser* mLogBrowser;
  QTextBrowser* mScriptBrowser;
};

