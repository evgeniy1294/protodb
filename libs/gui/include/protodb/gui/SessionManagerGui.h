#pragma once

#include <protodb/SessionManager.h>

#include <QDialog>

class QTableView;
class QPushButton;
class QDialogButtonBox;
class QAbstractButton;
class QCheckBox;
class QTextBrowser;
class QLineEdit;
class QSortFilterProxyModel;

class SessionManagerGui: public QDialog
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

private slots:
    void onDialogClicked(QAbstractButton* aBtn);
    void onCreateClicked();
    void onChangeClicked();
    void onRmClicked();

private:
    SessionManager* m_sm = nullptr;

    QSortFilterProxyModel* m_proxy_model;

    QDialogButtonBox* m_dialog_buttons;
    QTextBrowser* m_desc_browser;
    QTableView*   m_sessions_table;
    QCheckBox*    m_restore_chk;
    QLineEdit*    m_filter_le;
    QPushButton*  m_create_btn;
    QPushButton*  m_change_btn;
    QPushButton*  m_copy_btn;
    QPushButton*  m_rm_btn;
    QPushButton*  m_select_btn;
};
