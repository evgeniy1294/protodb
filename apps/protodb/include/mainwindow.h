#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tool_bar.h"
#include <QMainWindow>

namespace ads {
  class CDockManager;
}

class PluginManagerDialog;
class SessionManagerGui;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow() = default;

  void restoreState();
  void saveState();

private slots:
  void exit();

private:
  void createGui();
  void createDock();
  void createActions();
  void createToolBar();
  void connectSignals();

private:
  QAction* m_new;
  QAction* m_save;
  QAction* m_save_as;
  QAction* m_open;
  QAction* m_options;
  QAction* m_plugins;
  QAction* m_export_tables;
  QAction* m_import_tables;
  QAction* m_about;
  QAction* m_about_qt;
  QAction* m_help_content;
  QAction* m_sessions;
  QAction* m_exit;
  isa_tool_bar* m_toolbar;
  ads::CDockManager* m_dock_man;

  PluginManagerDialog* m_plugin_manager_dialog;
  SessionManagerGui* m_session_manager_dialog;
};


#endif // MAINWINDOW_H
