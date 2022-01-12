#pragma once

#include <QTreeView>

class QDataWidgetMapper;
class QSortFilterProxyModel;
class PluginManagerNew;
class PluginDetailDialog;

class PluginTreeView: public QTreeView
{
    Q_OBJECT

public:
    PluginTreeView(QWidget* parent = nullptr);
   ~PluginTreeView() = default;

    void setModel(QAbstractItemModel *model) override;

public slots:
    void setFilterFixedString(const QString& pattern);

private:
    void createMenu();
    void createActions();
    void connectSignals();

private:
    QSortFilterProxyModel* m_fm;
    QDataWidgetMapper* m_mapper;
    PluginManagerNew* m_pm;
    PluginDetailDialog* m_detail_dialog;
};
