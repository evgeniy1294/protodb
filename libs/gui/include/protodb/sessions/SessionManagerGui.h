#pragma once

#include <protodb/export/gui_cfg.hpp>

#include <QDialog>

class QTableView;
class QPushButton;
class QDialogButtonBox;
class QAbstractButton;
class QCheckBox;
class QTextBrowser;
class QLineEdit;
class QSortFilterProxyModel;
class QItemSelection;

namespace protodb {

class SessionManager;

class PROTODB_GUI_EXPORT SessionManagerGui: public QDialog
{
    Q_OBJECT

public:
    SessionManagerGui(QWidget* parent = nullptr);
   ~SessionManagerGui() = default;

    void setSessionManager(SessionManager* sm);
    SessionManager* sessionManager() const;

private:
    void create_gui();
    void create_connections();
    void disable_control_btn();
    void enable_control_btn();

private slots:
    void onCreateClicked();
    void onSwitchToClicked();
    void onCopyClicked();
    void onEditClicked();
    void onRmClicked();
    void onExportClicked();
    void onImportClicked();
    void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);

private:
    SessionManager* m_sm = nullptr;

    QSortFilterProxyModel* m_proxy_model;

    QPushButton* m_close_btn;
    QTextBrowser* m_desc_browser;
    QTableView*   m_sessions_table;
    QLineEdit*    m_filter_le;
    QPushButton*  m_create_btn;
    QPushButton*  m_change_btn;
    QPushButton*  m_copy_btn;
    QPushButton*  m_rm_btn;
    QPushButton*  m_switch_to_btn;
    QPushButton*  m_import_btn;
    QPushButton*  m_export_btn;
};

} // namespace protodb
