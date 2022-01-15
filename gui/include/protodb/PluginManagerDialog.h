#pragma once

#include <QDialog>

class QDialogButtonBox;
class QAbstractButton;
class QLineEdit;
class PluginTreeView;

class PluginManagerDialog: public QDialog
{
    Q_OBJECT

public:
    PluginManagerDialog(QWidget* parent = nullptr);
   ~PluginManagerDialog() = default;

private:
    void createGui();
    void connectSignals();

private slots:
    void onDialogClicked(QAbstractButton* aBtn);

private:
    PluginTreeView* m_view;
    QDialogButtonBox* m_dialog_buttons;
    QLineEdit* m_filter_line;
};
