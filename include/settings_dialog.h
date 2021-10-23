#pragma once

#include <QDialog>
#include <QList>
#include <QWidget>

class QListWidget;
class QDialogButtonBox;

class SettingsDialog: public QDialog {
    Q_OBJECT

public:
    SettingsDialog(QWidget* aParent = nullptr);
    ~SettingsDialog() = default;

private:
    void createGui();

private:
    QListWidget* mWgtSelector;
    QDialogButtonBox* mBtnBox;
    QList<QWidget*> mWidgetList;
};
