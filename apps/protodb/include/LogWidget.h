#pragma once

#include "Logger.h"
#include "LogFormatter.h"
#include <QWidget>

class QTableView;
class QPushButton;
class QLabel;
class QLineEdit;
class QComboBox;

namespace protodb {

class LogTableView;
class Logger;
class LogDecorationDialog;
class LogProxyModel;
class SeanceConfigDialog;

class LogWidget: public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget* parent = nullptr);
    ~LogWidget() = default;

    void setSeanceState(const nlohmann::json cfg);
    void getLogStyle(nlohmann::json& json) const;
    void setLogStyle(const nlohmann::json& json);

signals:
    void sByteFormatChanged(LogFormatter::ByteFormat format);

private:
    void createGui();
    void createConnections();

private:
    enum RawDataFormat {
        Hex_Format        = 0,
        Ascii_LF_Format   = 1,
        Ascii_CR_Format   = 2,
        Ascii_CRLF_Format = 3,
        Ascii_None_Format = 4,
    };

    QPushButton* m_mode_btn;
    QPushButton* m_clr_btn;
    QPushButton* m_run;
    QPushButton* m_auto_scroll;
    QPushButton* m_cfg_btn;
    QLineEdit* m_find_le;
    QLineEdit* m_msg_le;
    QComboBox* m_data_format_cmb;

    SeanceConfigDialog* m_conn_dialog;
    Logger* m_log_model;
    LogTableView* m_view;
    LogProxyModel* m_log_proxy_model;

};

} // namespace protodb
