#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <DockManager.h>

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
  void createActions();
  void createMenus();
  void createToolBar();

private:
  ads::CDockManager* mDockManager;
  QMenu* mSessionsMenu;
  QMenu* mToolsMenu;
  QMenu* mHelpMenu;
  QToolBar* mSessionsToolBar;
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
  QLabel*  mLabel;
};


#endif // MAINWINDOW_H
