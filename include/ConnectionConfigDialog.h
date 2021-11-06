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

private slots:
    void showFileDialog(QString& path);

private:
    QPushButton* m_log_btn;
    QPushButton* m_scr_btn;
    QButtonGroup* m_mode_group;
    QLineEdit* m_log_le;
    QLineEdit* m_scr_le;
    QDialogButtonBox* m_dialog_btn;

    QList<QWidget*> m_widgets;
};

