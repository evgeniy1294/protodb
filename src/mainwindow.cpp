#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QInputDialog>
#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mDockManager(new ads::CDockManager(this))
{
  setWindowTitle("Protocol Debugger");
  setWindowIcon(QIcon(":/icons/network.svg"));
  setIconSize({18,18});
  resize(640, 480);
  createActions();
  createMenus();
  createToolBar();

  SessionWidget* session_wdg = new SessionWidget();
  ads::CDockWidget* dock_wdg = new ads::CDockWidget("sessionName");
  dock_wdg->setWidget(session_wdg);

  mDockManager->addDockWidgetTab(ads::TopDockWidgetArea, dock_wdg);
}



void MainWindow::createActions()
{
  mExitAct = new QAction(QIcon(":/icons/close.svg"), tr("&Exit"), this);
  mExitAct->setStatusTip(tr("Close application"));
  connect(mExitAct, &QAction::triggered, this, &MainWindow::exit);

  mSessionsNewAct = new QAction(QIcon(":/icons/new_sessions.svg"), tr("&New..."), this);
  mSessionsNewAct->setShortcuts(QKeySequence::New);
  mSessionsNewAct->setStatusTip(tr("Create a new file"));
  connect(mSessionsNewAct, &QAction::triggered, this, &MainWindow::sessionsNew);

  mSessionsManageAct = new QAction(QIcon(":/icons/manage.svg"), tr("&Manage..."), this);
  mSessionsManageAct->setStatusTip(tr("Manage active sessions"));
  connect(mSessionsManageAct, &QAction::triggered, this, &MainWindow::sessionsManage);

  mSessionsSaveAct = new QAction(QIcon(":/icons/save.svg"), tr("&Save"), this);
  mSessionsSaveAct->setShortcuts(QKeySequence::Save);
  mSessionsSaveAct->setStatusTip(tr("Save active session"));
  connect(mSessionsSaveAct, &QAction::triggered, this, &MainWindow::sessionsSave);

  mSessionsSaveAsAct = new QAction(QIcon(":/icons/save_as.svg"), tr("&Save As..."), this);
  mSessionsSaveAsAct->setShortcuts(QKeySequence::SaveAs);
  mSessionsSaveAsAct->setStatusTip(tr("Save active session with parameters"));
  connect(mSessionsSaveAsAct, &QAction::triggered, this, &MainWindow::sessionsSaveAs);

  mSessionsOpenAct = new QAction(QIcon(":/icons/open.svg"), tr("&Open..."), this);
  mSessionsOpenAct->setShortcut(QKeySequence::Open);
  mSessionsOpenAct->setStatusTip(tr("Open session"));
  connect(mSessionsOpenAct, &QAction::triggered, this, &MainWindow::sessionsOpen);

  mToolsOptionsAct = new QAction(QIcon(":/icons/options.svg"), tr("&Options..."), this);
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
  connect(mHelpAboutQtAct, &QAction::triggered, &QApplication::aboutQt);

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
  mSessionsToolBar->addAction(mSessionsOpenAct);
  mSessionsToolBar->addAction(mSessionsSaveAct);
  mSessionsToolBar->addAction(mSessionsManageAct);
  mSessionsToolBar->addSeparator();
  mSessionsToolBar->addAction(mToolsOptionsAct);
}

MainWindow::~MainWindow()
{
}

void MainWindow::sessionsNew() {
  bool ok;

  QString sessionName = QInputDialog::getText(this, tr("Create new session"),
                                              tr("Enter new session name:"), QLineEdit::Normal,
                                              tr("New session"), &ok);
  if (ok && !sessionName.isEmpty())
  {
    SessionWidget* session_wdg = new SessionWidget();
    ads::CDockWidget* dock_wdg = new ads::CDockWidget(sessionName);
    dock_wdg->setWidget(session_wdg);

    mDockManager->addDockWidgetTab(ads::TopDockWidgetArea, dock_wdg);
  }

  return;
}

void MainWindow::sessionsManage() {
  return;
}

void MainWindow::sessionsSave() {
  return;
}

void MainWindow::sessionsSaveAs() {
  QFileDialog fileDialog;
  fileDialog.setAcceptMode(QFileDialog::AcceptSave);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setViewMode(QFileDialog::Detail);

  QStringList fileNames;
  if (fileDialog.exec()) {
    fileNames = fileDialog.selectedFiles();
  }

  // Do something

  return;
}

void MainWindow::sessionsOpen() {
  QFileDialog fileDialog;
  fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
  fileDialog.setFileMode(QFileDialog::AnyFile);
  fileDialog.setViewMode(QFileDialog::Detail);

  QStringList fileNames;
  if (fileDialog.exec()) {
    fileNames = fileDialog.selectedFiles();
  }

  // Do something

  return;
}

void MainWindow::toolsOptions() {
  return;
}

void MainWindow::toolsPlugins(){
  return;
}

void MainWindow::helpAbout(){
  QMessageBox messageBox;
  messageBox.setText("Protocol debugger is powerful terminal software.\n"
                     "(c)2021 Evgenii Fedoseev (evgeniy1294@yandex.ru)");
  messageBox.setStandardButtons(QMessageBox::Ok);
  messageBox.setDefaultButton(QMessageBox::Ok);
  messageBox.setIcon(QMessageBox::Icon::Information);
  messageBox.exec();

  return;
}



void MainWindow::helpContent(){
  return;
}

void MainWindow::exit(){
  // TODO: Save session
  QApplication::exit(0);

  return;
}
