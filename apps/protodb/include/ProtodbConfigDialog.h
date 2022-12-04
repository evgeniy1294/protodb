#pragma once

#include <QDialog>

#include <nlohmann/json.hpp>

class QLineEdit;
class QPushButton;
class QDialogButtonBox;

namespace protodb {

class ProtodbConfigDialog: public QDialog {
    Q_OBJECT
public:
    ProtodbConfigDialog(QObject* parent = nullptr);
   ~ProtodbConfigDialog() = default;

    void setState(const nlohmann::json& json);
    void state(nlohmann::json& json);

private:
    void create_gui();
    void connect_signals();

private:
    QLineEdit* m_plugin_install_path;
    QLineEdit* m_sessions_storage_path;
    QPushButton* m_plugin_install_fd;
    QPushButton* m_sessions_storage_fd;
    QDialogButtonBox* m_dialog_btns;
};

} // namespace protodb

