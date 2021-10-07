#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tool_bar.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void sessionsNew();
  void sessionsManage();
  void sessionsSave();
  void sessionsSaveAs();
  void sessionsOpen();
  void toolsOptions();
  void toolsPlugins();
  void helpAbout();
  void helpContent();
  void exit();

private:
  void createGui();
  void createActions();
  void createToolBar();

private:
  QAction* mSessionsNewAct;
  QAction* mSessionsManageAct;
  QAction* mSessionsSaveAct;
  QAction* mSessionsSaveAsAct;
  QAction* mSessionsOpenAct;
  QAction* mToolsOptionsAct;
  QAction* mToolsPluginsAct;
  QAction* mHelpAboutAct;
  QAction* mHelpAboutQtAct;
  QAction* mHelpContentsAct;
  QAction* mExitAct;
  isa_tool_bar* mIsaToolBar;
};


#endif // MAINWINDOW_H
