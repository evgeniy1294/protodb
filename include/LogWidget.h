#pragma once

#include <QWidget>
#include "logging.h"

class LogTableView;
class QTableView;
class QPushButton;
class QLabel;
class QLineEdit;
class ConnectionConfigDialog;
class LogModel;
class LogDecorationDialog;
class LuaApi;

class LogWidget: public QWidget
{
    Q_OBJECT

public:
    explicit LogWidget(QWidget* parent = nullptr);
    ~LogWidget() = default;

    void setModel(LogModel* model);
    LogModel model() const;

signals:
    void sByteFormatChanged(ByteFormat format);

private:
    void createGui();
    void createConnections();

private:
     QPushButton* m_mode_btn;
     QPushButton* m_clr_btn;
     QPushButton* m_run;
     QPushButton* m_cfg_btn;
     QLineEdit* m_find_le;
     QLineEdit* m_msg_le;
     LogTableView* m_view;
     ConnectionConfigDialog* m_conn_dialog;
     LogDecorationDialog* m_decoration_dialog;

     LuaApi* m_lua_api;
     LogModel* m_log_model;
};

