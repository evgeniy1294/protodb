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

    SessionCreateDialog(QWidget* parent = nullptr);
   ~SessionCreateDialog() = default;

    void setMode( Mode mode );
    bool isCreateMode() const;
    bool isEditMode() const;

    void setSessionName(const QString& name);
    QString sessionName() const;

    void setSessionDescription(const QString& desc);
    QString sessionDescription() const;

private slots:
    void onDialogClicked(QAbstractButton* aBtn);

private:
    void create_gui();
    void create_connections();

private:
    Mode m_mode;

    QDialogButtonBox* m_dialog_buttons;
    QLineEdit* m_name;
    QPlainTextEdit* m_desc;
};
