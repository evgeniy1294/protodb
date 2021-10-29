#pragma once

#include <QDialog>
#include <QList>
#include <QWidget>

class QGroupBox;
class QButtonGroup;
class QRadioButton;
class QDialogButtonBox;
class QLineEdit;

class ConnectionConfigDialog: public QDialog {
    Q_OBJECT

public:
    ConnectionConfigDialog(QWidget* aParent = nullptr);
    ~ConnectionConfigDialog() = default;

private:
    void createGui();
    void connectSignals();

private:
    QPushButton* m_file_button;
    QButtonGroup* m_mode_selector;
    QLineEdit* m_logfile;
    QDialogButtonBox* m_dialog_buttons;

    QList<QWidget*> m_widgets;
};

