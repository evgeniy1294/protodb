#pragma once

#include <QDialog>

class SessionManager;
class QLineEdit;
class QPlainTextEdit;
class QAbstractButton;
class QDialogButtonBox;
class QLabel;

class SessionCreateDialog: public QDialog
{
    Q_OBJECT
public:
    SessionCreateDialog(SessionManager* sm, QWidget* parent = nullptr);
   ~SessionCreateDialog() = default;

    void setSessionIndex(int idx);
    int sessionIndex() const;

    void setCreateMode(bool on = true);
    bool isCreateMode() const;

    void setChangeMode(bool on = true);
    bool isChangeMode() const;

private slots:
    void onDialogClicked(QAbstractButton* aBtn);
    void onNameTextChanged(const QString& text);

private:
    void create_gui();
    void create_connections();

private:
    SessionManager* m_sm;
    int  m_session_idx;
    bool m_create_mode;

    QDialogButtonBox* m_dialog_buttons;
    QLineEdit* m_name;
    QPlainTextEdit* m_desc;
    QLabel* m_error_label;
};
