#pragma once

#include <protodb/export/gui_cfg.hpp>

#include <QDialog>

class QPushButton;
class QDialogButtonBox;
class QAbstractButton;
class QLineEdit;
class QTextBrowser;
class QDataWidgetMapper;
class QPlainTextEdit;

namespace protodb {

class PluginTreeView;

class PROTODB_GUI_EXPORT PluginManagerDialog: public QDialog
{
    Q_OBJECT

public:
    PluginManagerDialog(QWidget* parent = nullptr);
   ~PluginManagerDialog() = default;

private:
    void createGui();
    void connectSignals();
    void initMapper();

private slots:
    void onDialogClicked(QAbstractButton* aBtn);

private:
    PluginTreeView* m_view;

    QDataWidgetMapper* m_mapper;
    QDialogButtonBox* m_dialog_buttons;
    QLineEdit* m_filter_line;
    QLineEdit* m_plugin_location;
    QTextBrowser* m_plugin_descr;
    QPlainTextEdit* m_plugin_deps;
    //QPushButton* m_detail_btn;
};

} // namespace protodb
