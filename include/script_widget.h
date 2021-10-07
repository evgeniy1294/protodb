#pragma once

#include <QWidget>

class QTextBrowser;

class ScriptWidget: public QWidget
{
  Q_OBJECT

public:
  explicit ScriptWidget(QWidget* parent = nullptr);
  ~ScriptWidget();

private:
  void createGui();

private:
  QTextBrowser* mScriptBrowser;
};


