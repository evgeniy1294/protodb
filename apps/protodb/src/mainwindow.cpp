#include "mainwindow.h"
#include "SequenceModel.h"
#include "SequenceTableWidget.h"
#include "LogWidget.h"
#include "ProtodbSessionManager.h"
#include "ProtodbConfigDialog.h"
#include "Assistant.h"
#include "MainClass.h"
#include "CrcCalculator.h"

#include "ProtodbVersion.h"

#include <protodb/plugins/PluginManagerDialog.h>
#include <protodb/sessions/SessionManagerGui.h>
#include <protodb/utils/JsonUtils.h>

#include <DockManager.h>

#include <nlohmann/json.hpp>

#include <QApplication>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QIcon>
#include <QMessageBox>
#include <QFileDialog>
#include <QStringList>
#include <QInputDialog>

using namespace protodb;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
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
    setIconSize({18,18});

    auto main_layout = new QHBoxLayout();
        main_layout->addWidget(m_toolbar);
        main_layout->addWidget(m_dock_man);

    auto central_widget = new QWidget();
        central_widget->setLayout(main_layout);

    setCentralWidget(central_widget);

    m_config_dialog = new ProtodbConfigDialog(this);
    m_plugin_manager_dialog = new PluginManagerDialog(this);
    m_session_manager_dialog = new SessionManagerGui(this);
        m_session_manager_dialog->setSessionManager(&ProtodbSessionManager::instance());
}

void MainWindow::createDock()
{
    ads::CDockManager::setConfigFlag(ads::CDockManager::DockAreaHideDisabledButtons, true);
    ads::CDockManager::setConfigFlag(ads::CDockManager::AlwaysShowTabs, true);

    m_log_widget = new LogWidget();

    auto seance_widget = new ads::CDockWidget("Seance");
        seance_widget->setObjectName("SeanceWidget");
        seance_widget->setWidget(m_log_widget);

        m_wgt_menu->addAction(seance_widget->toggleViewAction());

    auto& main_class = MainClass::instance();
    auto incoming_table_widget = new ads::CDockWidget("Incoming sequences");
        incoming_table_widget->setObjectName("IncomingSequences");
        incoming_table_widget->setWidget(new SequenceTableWidget(main_class.incomingSequences()));

        m_wgt_menu->addAction(incoming_table_widget->toggleViewAction());

    auto connection_control_widget = new ads::CDockWidget("Connection control");
        connection_control_widget->setObjectName("ConnectionControl");
        m_wgt_menu->addAction(connection_control_widget->toggleViewAction());

    auto outgoing_table_widget = new ads::CDockWidget("Outgoing sequences");
        outgoing_table_widget->setObjectName("OutgoingSequences");
        outgoing_table_widget->setWidget(new SequenceTableWidget(main_class.outgoingSequences()));

        m_wgt_menu->addAction(outgoing_table_widget->toggleViewAction());

    m_crc_calc = new ChecksumCalculator();
    auto checksum_dock = new ads::CDockWidget("Checksum calculator");
        checksum_dock->setObjectName("ChecksumCalculator");
        checksum_dock->setWidget(m_crc_calc);

        m_wgt_menu->addAction(checksum_dock->toggleViewAction());

    ads::CDockAreaWidget* area;
    m_dock_man = new ads::CDockManager();
    m_dock_man->addDockWidget(ads::RightDockWidgetArea, seance_widget);

    area = m_dock_man->addDockWidget(ads::RightDockWidgetArea, connection_control_widget);
    area = m_dock_man->addDockWidget(ads::BottomDockWidgetArea, outgoing_table_widget, area);
    area = m_dock_man->addDockWidget(ads::BottomDockWidgetArea, checksum_dock, area, 1);
    m_dock_man->addDockWidget(ads::CenterDockWidgetArea, incoming_table_widget, area, 0);
}


void MainWindow::createActions()
{
    m_show_wgt_menu = new QAction(QIcon(), tr("&Wigets"), this);
    m_sessions = new QAction(QIcon(), tr("&Sessions..."), this);
    m_options = new QAction(QIcon(), tr("&Options..."), this);
    m_plugins = new QAction(QIcon(":/icons/plugin.svg"), tr("&Plugins..."), this);
    m_import_tables = new QAction(tr("&Import tables"), this);
    m_export_tables = new QAction(tr("&Export tables"), this);
    m_import_log    = new QAction(tr("&Import log"), this);
    m_export_log    = new QAction(tr("&Export log"), this);
    m_plugins = new QAction(tr("&Plugins..."), this);
    m_about = new QAction(tr("&About"), this);
    m_about_qt = new QAction(tr("&About Qt"), this);
    m_help_content = new QAction(tr("&Help"), this);
    m_exit = new QAction(tr("&Exit"), this);
}



void MainWindow::createToolBar() {
    m_toolbar = new isa_tool_bar( QBoxLayout::TopToBottom );
    m_toolbar->setButtonSize( QSize( 28, 28 ) );

    m_toolbar->addToolAction(m_show_wgt_menu, false);
    m_toolbar->addToolAction(m_export_tables, false);
    m_toolbar->addToolAction(m_import_tables, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_export_log, false);
    m_toolbar->addToolAction(m_import_log, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_options, false);
    m_toolbar->addToolAction(m_plugins, false);
    m_toolbar->addToolAction(m_sessions, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_help_content, false);
    m_toolbar->addToolAction(m_about, false);
    m_toolbar->addToolAction(m_about_qt, false);
    m_toolbar->addMenuSeparator();
    m_toolbar->addToolAction(m_exit, false);

    m_wgt_menu = new QMenu();
        m_show_wgt_menu->setMenu(m_wgt_menu);
}

void MainWindow::connectSignals()
{
    connect(m_exit, &QAction::triggered, QApplication::instance(), &QApplication::quit);
    connect(m_about_qt, &QAction::triggered, &QApplication::aboutQt);

    connect(m_plugins, &QAction::triggered, this, [this]() {
        m_plugin_manager_dialog->show();
    });

    connect(m_help_content, &QAction::triggered, this, []() {
        Assistant::instance().showDocumentation("content/index.html");
    });

    connect(m_options, &QAction::triggered, m_config_dialog, &QDialog::show);

    connect(m_about, &QAction::triggered, this, []() {
        static auto msg = QString("Protocol debugger v%1.%2.%3\n\nProtocol Debugger is powerful terminal software.\n"
                                  "(c)2021 Evgenii Fedoseev (evgeniy1294@yandex.ru)");

        QMessageBox box;
        box.setText(msg.arg(QString::number(PROTODB_VERSION_MAJOR),
                            QString::number(PROTODB_VERSION_MINOR),
                            QString::number(PROTODB_VERSION_PATCH)));
        box.setIcon(QMessageBox::Icon::Information);
        box.exec();
    });

    connect(m_export_log, &QAction::triggered, this, [this]() {
        QString exportPath = QDir::homePath() + "/" + "log.json";
        exportPath = QFileDialog::getSaveFileName(this,
            tr("Select session"), exportPath, tr("Log files (*.json)"));

        nlohmann::json log;
            MainClass::instance().logger()->toJson(log);
            writeToFile(exportPath, log);
    });

    connect(m_import_log, &QAction::triggered, this, [this]() {
        QString importPath = QDir::homePath();
        importPath = QFileDialog::getOpenFileName(this,
            tr("Select session"), importPath, tr("Log files (*.json)"));

        nlohmann::json log;
            readFromFile(importPath, log);
            MainClass::instance().logger()->fromJson(log);

    });

    connect(m_export_tables, &QAction::triggered, this, []() {
        QFileDialog fileDialog;
        fileDialog.setAcceptMode(QFileDialog::AcceptSave);
        fileDialog.setFileMode(QFileDialog::AnyFile);
        fileDialog.setViewMode(QFileDialog::List);

        if (fileDialog.exec()) {
            auto& main_class = MainClass::instance();
            auto fileNames = fileDialog.selectedFiles();

            if (fileNames.size() != 0) {
                auto path = fileNames.back();
                nlohmann::json j;

                nlohmann::json outgoing;
                    main_class.outgoingSequences()->toJson(outgoing);
                    j["outgoing"] = outgoing;

                nlohmann::json incoming;
                    main_class.incomingSequences()->toJson(incoming);
                     j["incoming"] = incoming;

                writeToFile(path, j);
            }
        }
    });

    connect(m_import_tables, &QAction::triggered, this, []() {
        QFileDialog fileDialog;
        fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
        fileDialog.setFileMode(QFileDialog::AnyFile);
        fileDialog.setViewMode(QFileDialog::List);

        if (fileDialog.exec()) {
            auto& main_class = MainClass::instance();
            auto fileNames = fileDialog.selectedFiles();

            if (fileNames.size() != 0) {
                nlohmann::json json;
                bool ok = readFromFile(fileNames.back(), json);

                if (ok) {
                    if (json["incoming"].is_array()) {
                        main_class.incomingSequences()->fromJson(json["incoming"]);
                    }

                    if (json["outgoing"].is_array()) {
                        main_class.outgoingSequences()->fromJson(json["outgoing"]);
                    }
                }
            }
        }
    });

    connect(m_sessions, &QAction::triggered, this, [this]() {
        m_session_manager_dialog->show();
    });

    connect(&MainClass::instance(), &MainClass::sStarted, this, [this](QString str, QWidget* control_wgt) {
        auto seance_widget = m_dock_man->findDockWidget("SeanceWidget");
            seance_widget->setWindowTitle(QString("Seance: %1").arg(str));

        if (control_wgt) {
            auto dock = m_dock_man->findDockWidget("ConnectionControl");
                dock->setWidget(control_wgt);
                dock->update();
                dock->closeDockWidget();
                dock->toggleView(true);
        }
    });

    connect(&MainClass::instance(), &MainClass::sStopted, this, [this]() {
        auto seance_widget = m_dock_man->findDockWidget("SeanceWidget");
        seance_widget->setWindowTitle(QString("Seance"));

        auto dock = m_dock_man->findDockWidget("ConnectionControl");
            dock->toggleView(false);

        auto wgt = dock->takeWidget();
            if (wgt) wgt->deleteLater();
    });

    connect(m_log_widget, &LogWidget::sCalculateCrc, this, [this](const QByteArray& bytes) {
        m_crc_calc->setData(bytes, true);
    });
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QApplication::quit();
}

void MainWindow::getWidgetsState(nlohmann::json& json) const
{
    auto log = static_cast<LogWidget*>(m_dock_man->findDockWidget("SeanceWidget")->widget());

    json["MainWindowGeometry"] = saveGeometry();
    json["DockingState"] = m_dock_man->saveState();

    nlohmann::json log_style;
        log->getLogStyle(log_style);
    json["LogStyle"] = log_style;

}

void MainWindow::setWidgetsState(const nlohmann::json& json)
{
    if (json.contains("MainWindowGeometry")) {
        auto geometry = json.value("MainWindowGeometry", QByteArray());
        if (!geometry.isEmpty())
            restoreGeometry(geometry);
    }

    if (json.contains("DockingState")) {
        auto state = json.value("DockingState", QByteArray());
        if (!state.isEmpty())
            m_dock_man->restoreState(state);
    }

    if (json.contains("LogStyle")) {
        auto log = static_cast<LogWidget*>(m_dock_man->findDockWidget("SeanceWidget")->widget());
        log->setLogStyle(json.value("LogStyle", nlohmann::json::object()));
    }
}

void MainWindow::getToolsState(nlohmann::json& json) const
{
    json["ChecksumCalculator"] = m_crc_calc->currentModel();
}

void MainWindow::setToolsState(const nlohmann::json& json)
{
    auto model = json.value("ChecksumCalculator", CrcModel{ 16, 0x8005, 0xffff, 0x0000, true, true});
    m_crc_calc->setModel(model);
}


void MainWindow::updateSeanceState()
{
    nlohmann::json state; MainClass::instance().seanceConfigs(state);
        auto wgt = static_cast<LogWidget*>(m_dock_man->findDockWidget("SeanceWidget")->widget());
        wgt->setSeanceState(state);
}
