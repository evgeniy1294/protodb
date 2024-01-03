#pragma once

#include <QDialog>
#include <QMap>
#include <QWidget>

#include <protodb/configurable/Configurable.h>
#include <nlohmann/json.hpp>

class QGroupBox;
class QButtonGroup;
class QAbstractButton;
class QDialogButtonBox;
class QLineEdit;
class QVBoxLayout;
class QGridLayout;
class QTabWidget;

namespace protodb {

class ConnectionConfigWidget;
class LogFormatWidget;
class DelimetersConfigWidget;

class SeanceConfigDialog: public QDialog, public Configurable
{
    Q_OBJECT

public:
    SeanceConfigDialog(QWidget* aParent = nullptr);
   ~SeanceConfigDialog() = default;

    //void seanceConfig(nlohmann::json& json);

    void setConfig(const nlohmann::json& json) override;
    void config(nlohmann::json& json) const override;
    void defaultConfig(nlohmann::json& json) const override;

    void setState(const nlohmann::json& json) override;
    void state(nlohmann::json& json) const override;
    void defaultState(nlohmann::json& json) const override;

private:
    void createGui();
    void connectSignals();
    void replaceIOWiget(const QString& cid);

private slots:
    void showFileDialog(QString& path);
    bool event(QEvent *e) override;

private:
    QPushButton* m_log_btn;
    QPushButton* m_scr_btn;
    QButtonGroup* m_selection_group;
    QLineEdit* m_log_le;
    QLineEdit* m_scr_le;
    QDialogButtonBox* m_dialog_btn;
    QVBoxLayout* m_selector_layout;
    QWidget* m_dummy_wgt;
    QGridLayout* m_layout;
    QWidget* m_current_iowiget;
    QTabWidget* m_tab_widget;

    LogFormatWidget* m_log_format_wiget;
    DelimetersConfigWidget* m_delimeters_cfg_widget;

    QMap<QString, QAbstractButton*> m_selection_btns;
    QMap<QString, ConnectionConfigWidget*> m_io_widgets;
    QString m_curr_cid;
    nlohmann::json m_curr_cfg;
};

} // namespace protodb
