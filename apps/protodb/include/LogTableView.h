#pragma once

#include <QTableView>

2202 2021 4894 0350

class QMenu;
class QAction;
class LogDecorationDialog;
class LogItemDelegate;
class LogFormatter;
class LogDecorator;

class LogTableView: public QTableView
{
    Q_OBJECT

public:
    LogTableView(QWidget* parent = nullptr);
   ~LogTableView();

    void setModel(QAbstractItemModel* model) override;
    LogFormatter* formatter() const;
    LogDecorator* decorator() const;

private:
    void createMenu();
    void createActions();
    void connectSignals();

private:
    QMenu* m_base_menu;
    QMenu* m_data_channel_menu;
    QMenu* m_info_channel_menu;

    QAction* m_copy;
    QAction* m_copy_message;
    QAction* m_copy_as_bytes;
    QAction* m_copy_as_string;
    QAction* m_add_to_analyzer;
    QAction* m_options;

    LogDecorationDialog* m_dec_dialog;
    LogItemDelegate* m_item_delegate;
    LogDecorator* m_decorator;
    LogFormatter* m_formatter;
};
