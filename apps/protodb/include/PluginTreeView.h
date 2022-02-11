#pragma once

#include <QTreeView>

class QDataWidgetMapper;
class QSortFilterProxyModel;
class PluginManager;
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

    bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event) override;

private:
    QSortFilterProxyModel* m_fm;
    QDataWidgetMapper* m_mapper;
    PluginManager* m_pm;
    PluginDetailDialog* m_detail_dialog;
};
