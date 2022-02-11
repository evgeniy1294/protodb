#pragma once

#include <QDialog>

class QDataWidgetMapper;
class QDialogButtonBox;
class QLineEdit;
class QTextBrowser;
class QAbstractButton;

class PluginDetailDialog: public QDialog
{
    Q_OBJECT

public:
    PluginDetailDialog(QWidget* parent = nullptr);
   ~PluginDetailDialog() = default;

    void setMapper(QDataWidgetMapper* mapper);
    QDataWidgetMapper* mapper() const;

private:
    void createGui();
    void connectSignals();

private slots:
    void onDialogClicked(QAbstractButton* btn);

private:
    QDataWidgetMapper* m_mapper;
    QDialogButtonBox* m_btn_box;
    QLineEdit* m_name;
    QLineEdit* m_version;
    QLineEdit* m_vendor;
    QLineEdit* m_group;
    QLineEdit* m_location;
    QTextBrowser* m_descr;
};
