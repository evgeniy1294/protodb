#pragma once

#include <protodb/SessionManager.h>

#include <QDialog>

class QTableView;
class QPushButton;
class QDialogButtonBox;
class QAbstractButton;
class QCheckBox;

class SessionManagerDialog: public QDialog
{
    Q_OBJECT

public:
    SessionManagerDialog(QWidget* parent = nullptr);
   ~SessionManagerDialog() = default;

    void setSessionManager(SessionManager* sm);
    SessionManager* sessionManager() const;

private:
    void create_gui();
    void create_connections();

private slots:
    void onDialogClicked(QAbstractButton* aBtn);

private:
    SessionManager* m_sm = nullptr;

    QDialogButtonBox* m_dialog_buttons;
    QTableView*   m_sessions_table;
    QCheckBox*    m_restore_chk;
    QPushButton*  m_create_btn;
    QPushButton*  m_rename_btn;
    QPushButton*  m_copy_btn;
    QPushButton*  m_del_btn;
    QPushButton*  m_select_btn;
};
