#pragma once

#include "tool_bar.h"

#include <QMainWindow>

#include <nlohmann/json_fwd.hpp>

class QMenu;

namespace ads {
  class CDockManager;
  class CDockWidget;
}

namespace protodb {

class PluginManagerDialog;
class SessionManagerGui;
class ProtodbConfigDialog;
class ChecksumCalculator;
class LogWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

    void updateSeanceState();

    void getWidgetsState(nlohmann::json& json) const;
    void setWidgetsState(const nlohmann::json& json);

private:
    void createGui();
    void createDock();
    void createActions();
    void createToolBar();
    void connectSignals();

    void closeEvent(QCloseEvent *event);

private:
    QAction* m_show_wgt_menu;
    QAction* m_options;
    QAction* m_plugins;
    QAction* m_export_tables;
    QAction* m_import_tables;
    QAction* m_export_log;
    QAction* m_import_log;
    QAction* m_about;
    QAction* m_about_qt;
    QAction* m_help_content;
    QAction* m_sessions;
    QAction* m_exit;
    QAction* m_show_crc_calc;
    QMenu*   m_wgt_menu;
    isa_tool_bar* m_toolbar;
    ads::CDockManager* m_dock_man;

    LogWidget* m_log_widget;
    PluginManagerDialog* m_plugin_manager_dialog;
    SessionManagerGui* m_session_manager_dialog;
    ProtodbConfigDialog* m_config_dialog;
    ChecksumCalculator* m_crc_calc;
};

} // namespace protodb
