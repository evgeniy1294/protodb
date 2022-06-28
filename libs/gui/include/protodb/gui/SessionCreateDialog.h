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
    enum Mode { CreateMode, EditMode };

    SessionCreateDialog(SessionManager* sm, QWidget* parent = nullptr);
   ~SessionCreateDialog() = default;

    void setSessionIndex(int idx);
    int sessionIndex() const;

    void setMode( Mode mode );
    bool isCreateMode() const;
    bool isEditMode() const;

private slots:
    void onDialogClicked(QAbstractButton* aBtn);
    void onNameTextChanged(const QString& text);

private:
    void create_gui();
    void create_connections();

private:
    SessionManager* m_sm;
    int  m_session_idx;
    Mode m_mode;

    QDialogButtonBox* m_dialog_buttons;
    QLineEdit* m_name;
    QPlainTextEdit* m_desc;
    QLabel* m_error_label;
};
