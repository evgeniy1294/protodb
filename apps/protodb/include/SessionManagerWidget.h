#pragma once

#include "ProtodbSessionManager.h"

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

private:
    void create_gui();
    void create_connections();

private slots:
    void onDialogClicked(QAbstractButton* aBtn);

private:
    QDialogButtonBox* m_dialog_buttons;
    QTableView*   m_sessions_table;
    QCheckBox*    m_restore_chk;
    QPushButton*  m_create_btn;
    QPushButton*  m_rename_btn;
    QPushButton*  m_copy_btn;
    QPushButton*  m_del_btn;
    QPushButton*  m_select_btn;
};
