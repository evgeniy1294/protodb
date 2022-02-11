#pragma once

#include <QTableView>
#include <protodb/shared_types/LogTypes.h>

class QMenu;
class QAction;
class LogDecorationDialog;
class LogDecorator;
class LogItemDelegate;

class LogTableView: public QTableView
{
    Q_OBJECT

public:
    LogTableView(QWidget* parent = nullptr);
    void setModel(QAbstractItemModel *model) override;
    void setDecorator(LogDecorator *decorator);
    void setByteFormat(ByteFormat format);

signals:
    void sToAnalyzer(const QByteArray&);

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
};
