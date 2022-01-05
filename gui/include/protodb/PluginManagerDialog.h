#pragma once

#include <QDialog>

class QTreeWidget;
class QDialogButtonBox;
class QLineEdit;

class PluginManagerDialog: public QDialog
{
    Q_OBJECT

public:
    PluginManagerDialog(QWidget* parent = nullptr);
   ~PluginManagerDialog() = default;

private:
    void createGui();
    void connectSignals();

private:
    QTreeWidget* m_tree;
    QDialogButtonBox* m_dialog_buttons;
    QLineEdit* m_filter_line;
};
