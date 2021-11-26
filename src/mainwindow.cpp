#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QToolBar>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QInputDialog>
#include <QSettings>

#include <DockManager.h>

#include "mainwindow.h"
#include "settings_dialog.h"
#include "Worker.h"
#include "SequenceTableWidget.h"
#include "LogWidget.h"

#include "LogModel.h"

MainWindow::MainWindow(Worker* worker, QWidget *parent)
    : QMainWindow(parent)
    , m_worker(worker)
{
  createGui();
  connectSignals();
}


void MainWindow::createGui()
{
    createActions();
    createToolBar();
    createDock();

    setWindowTitle("Protocol Debugger");
    setWindowIcon(QIcon(":/icons/network.svg"));
    setIconSize({18,18});

    auto main_layout = new QHBoxLayout();
        main_layout->addWidget(m_toolbar);
        main_layout->addWidget(m_dock_man);

    auto central_widget = new QWidget();
        central_widget->setLayout(main_layout);

    setCentralWidget(central_widget);
}

#include "SequenceModel.h"
void MainWindow::createDock()
{
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHideDisabledButtons, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::AlwaysShowTabs, true);

    auto log_widget = new ads::CDockWidget("Log");
        log_widget->setWidget(new LogWidget());

    auto incoming_table_widget = new ads::CDockWidget("Incoming");
        incoming_table_widget->setWidget(new SequenceTableWidget(m_worker->incomingSequences()));

    auto outgoing_table_widget = new ads::CDockWidget("Outgoing");
        outgoing_table_widget->setWidget(new SequenceTableWidget(m_worker->outgoingSequences()));

    m_dock_man = new ads::CDockManager();
    m_dock_man->addDockWidget(ads::RightDockWidgetArea, log_widget);
    m_dock_man->addDockWidget(ads::RightDockWidgetArea, outgoing_table_widget);
    m_dock_man->addDockWidgetTab(ads::BottomDockWidgetArea, incoming_table_widget);
}


void MainWindow::createActions()
{
    m_new = new QAction(QIcon(":/icons/new_sessions.svg"), tr("&New..."), this);
    m_save = new QAction(QIcon(":/icons/save.svg"), tr("&Save"), this);
    m_save_as = new QAction(QIcon(":/icons/save_as.svg"), tr("&Save As..."), this);
    m_open = new QAction(QIcon(":/icons/open.svg"), tr("&Open..."), this);
    m_options = new QAction(QIcon(":/icons/options.svg"), tr("&Options..."), this);
    m_plugins = new QAction(tr("&Plugins..."), this);
    m_about = new QAction(tr("&About"), this);
    m_about_qt = new QAction(tr("&About Qt"), this);
    m_help_content = new QAction(tr("&Help"), this);
    m_exit = new QAction(QIcon(":/icons/close.svg"), tr("&Exit"), this);
}



void MainWindow::createToolBar() {
    m_toolbar = new isa_tool_bar( QBoxLayout::TopToBottom );
    m_toolbar->setButtonSize( QSize( 28, 28 ) );

    m_toolbar->addToolAction(m_new, false);
    m_toolbar->addToolAction(m_open);
    m_toolbar->addToolAction(m_save);
    m_toolbar->addToolAction(m_save_as, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_options, false);
    m_toolbar->addToolAction(m_plugins, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_help_content, false);
    m_toolbar->addToolAction(m_about_qt, false);
    m_toolbar->addToolAction(m_about, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_exit, false);
}

void MainWindow::connectSignals()
{
    connect(m_exit, &QAction::triggered, this, &MainWindow::exit);
    connect(m_about_qt, &QAction::triggered, &QApplication::aboutQt);

    connect(m_about, &QAction::triggered, this, [this]() {
        QMessageBox box;
        box.setText("Protocol debugger is powerful terminal software.\n"
                    "(c)2021 Evgenii Fedoseev (evgeniy1294@yandex.ru)");
        box.setIcon(QMessageBox::Icon::Information);
        box.exec();
    });

}

void MainWindow::saveState()
{
    QSettings settings;

    settings.setValue("MainWindow/Geometry", saveGeometry());
    settings.setValue("MainWindow/DockingState", m_dock_man->saveState());
}

void MainWindow::restoreState()
{
    QSettings settings;

    if (settings.contains("MainWindow/Geometry")) {
        auto geometry = settings.value("MainWindow/Geometry").toByteArray();
            restoreGeometry(geometry);
    }

    if (settings.contains("MainWindow/DockingState")) {
        auto state = settings.value("MainWindow/DockingState").toByteArray();
          m_dock_man->restoreState(state);
    }
}

void MainWindow::exit(){
  saveState();
  QApplication::exit(0);

  return;
}
