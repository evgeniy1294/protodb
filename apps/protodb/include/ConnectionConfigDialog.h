#pragma once

#include <QDialog>
#include <QMap>
#include <QWidget>

#include <protodb/configurable/Configurable.h>
#include <protodb/utils/JsonBaseUtils.h>

class QGroupBox;
class QButtonGroup;
class QAbstractButton;
class QDialogButtonBox;
class QLineEdit;
class QVBoxLayout;
class QGridLayout;
class IOWidgetFactory;
class IOWidget;
class LogFormatWidget;

class ConnectionConfigDialog: public QDialog, public Configurable {
    Q_OBJECT

public:
    ConnectionConfigDialog(QWidget* aParent = nullptr);
   ~ConnectionConfigDialog() = default;

    void connectionConfig(nlohmann::json& json);

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

    LogFormatWidget* m_log_format_wiget;

    QMap<QString, QAbstractButton*> m_selection_btns;
    QMap<QString, IOWidget*> m_io_widgets;
    QString m_curr_cid;
    nlohmann::json m_curr_cfg;
};

