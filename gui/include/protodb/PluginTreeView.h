#pragma once

#include <QTreeView>

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
    QSortFilterProxyModel* getFilterModel() const;

public slots:
    void setFilterFixedString(const QString& pattern);

private:
    void createMenu();
    void createActions();
    void connectSignals();

private:
    QSortFilterProxyModel* m_fm;
    PluginManager* m_pm;
    PluginDetailDialog* m_detail_dialog;
};
