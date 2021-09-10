#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QIcon>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mDockManager(new ads::CDockManager(this))
{
  setWindowTitle("Protocol Debugger");
  createActions();
  createMenus();
  createToolBar();

  // Create example content
  QLabel* l = new QLabel();
  l->setWordWrap(true);
  l->setAlignment(Qt::AlignTop | Qt::AlignLeft);
  l->setText("Some text on first label.");

  ads::CDockWidget* DockWidget = new ads::CDockWidget("Label 1");
  DockWidget->setWidget(l);

  mDockManager->addDockWidget(ads::TopDockWidgetArea, DockWidget);
}



void MainWindow::createActions()
{
  mExitAct = new QAction(tr("&Exit"), this);
  mExitAct->setStatusTip(tr("Close application"));
  connect(mExitAct, &QAction::triggered, this, &MainWindow::exit);

  mSessionsNewAct = new QAction(QIcon(":/icons/new_sessions.svg"), tr("&New session..."), this);
  mSessionsNewAct->setShortcuts(QKeySequence::New);
  mSessionsNewAct->setStatusTip(tr("Create a new file"));
  connect(mSessionsNewAct, &QAction::triggered, this, &MainWindow::sessionsNew);

  mSessionsManageAct = new QAction(tr("&Manage sessions..."), this);
  mSessionsManageAct->setStatusTip(tr("Manage active sessions"));
  connect(mSessionsManageAct, &QAction::triggered, this, &MainWindow::sessionsManage);

  mSessionsSaveAct = new QAction(tr("&Save"), this);
  mSessionsSaveAct->setShortcuts(QKeySequence::Save);
  mSessionsSaveAct->setStatusTip(tr("Save active session"));
  connect(mSessionsSaveAct, &QAction::triggered, this, &MainWindow::sessionsSave);

  mSessionsSaveAsAct = new QAction(tr("&Save as..."), this);
  mSessionsSaveAsAct->setShortcuts(QKeySequence::SaveAs);
  mSessionsSaveAsAct->setStatusTip(tr("Save active session with parameters"));
  connect(mSessionsSaveAsAct, &QAction::triggered, this, &MainWindow::sessionsSaveAs);

  mSessionsOpenAct = new QAction(tr("&Open..."), this);
  mSessionsOpenAct->setShortcut(QKeySequence::Open);
  mSessionsOpenAct->setStatusTip(tr("Open session"));
  connect(mSessionsOpenAct, &QAction::triggered, this, &MainWindow::sessionsOpen);

  mToolsOptionsAct = new QAction(tr("&Options..."), this);
  mToolsOptionsAct->setStatusTip(tr("Options"));
  connect(mToolsOptionsAct, &QAction::triggered, this, &MainWindow::toolsOptions);

  mToolsPluginsAct = new QAction(tr("&Plugins..."), this);
  mToolsPluginsAct->setStatusTip("Plugins");
  connect(mToolsPluginsAct, &QAction::triggered, this, &MainWindow::toolsPlugins);

  mHelpAboutAct = new QAction(tr("&About"), this);
  mHelpAboutAct->setStatusTip("About");
  connect(mHelpAboutAct, &QAction::triggered, this, &MainWindow::helpAbout);

  mHelpAboutQtAct = new QAction(tr("&About Qt"), this);
  mHelpAboutQtAct->setStatusTip("About Qt");
  connect(mHelpAboutQtAct, &QAction::triggered, this, &MainWindow::helpAboutQt);

  mHelpContentsAct = new QAction(tr("&Contents"), this);
  mHelpContentsAct->setStatusTip("Help contents");
  connect(mHelpContentsAct, &QAction::triggered, this, &MainWindow::helpContent);
}


void MainWindow::createMenus()
{
  mSessionsMenu = menuBar()->addMenu(tr("&Sessions"));
  mSessionsMenu->addAction(mSessionsNewAct);
  mSessionsMenu->addAction(mSessionsManageAct);
  mSessionsMenu->addAction(mSessionsOpenAct);
  mSessionsMenu->addAction(mSessionsSaveAct);
  mSessionsMenu->addAction(mSessionsSaveAsAct);
  mSessionsMenu->addSeparator();
  mSessionsMenu->addAction(mExitAct);

  mToolsMenu = menuBar()->addMenu(tr("&Tools"));
  mToolsMenu->addAction(mToolsPluginsAct);
  mToolsMenu->addSeparator();
  mToolsMenu->addAction(mToolsOptionsAct);

  mHelpMenu = menuBar()->addMenu(tr("&Help"));
  mHelpMenu->addAction(mHelpContentsAct);
  mHelpMenu->addSeparator();
  mHelpMenu->addAction(mHelpAboutAct);
  mHelpMenu->addAction(mHelpAboutQtAct);
}


void MainWindow::createToolBar() {
  mSessionsToolBar = addToolBar(tr("Sessions"));
  mSessionsToolBar->setFixedHeight(32);
  mSessionsToolBar->setIconSize({18,18});
  mSessionsToolBar->setMovable(false);
  mSessionsToolBar->addAction(mSessionsNewAct);
}

MainWindow::~MainWindow()
{
}

void MainWindow::sessionsNew() {
  return;
}

void MainWindow::sessionsManage() {
  return;
}

void MainWindow::sessionsSave() {
  return;
}

void MainWindow::sessionsSaveAs() {
  return;
}

void MainWindow::sessionsOpen() {
  return;
}

void MainWindow::toolsOptions() {
  return;
}

void MainWindow::toolsPlugins(){
  return;
}

void MainWindow::helpAbout(){
  return;
}

void MainWindow::helpAboutQt(){
  return;
}

void MainWindow::helpContent(){
  return;
}

void MainWindow::exit(){
  return;
}
