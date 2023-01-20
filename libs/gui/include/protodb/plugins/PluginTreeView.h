#pragma once

#include <protodb/export/gui_cfg.hpp>

#include <QTreeView>

class QDataWidgetMapper;
class QSortFilterProxyModel;

namespace protodb
{

class PluginManager;

class PROTODB_GUI_EXPORT PluginTreeView: public QTreeView
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
    PluginManager* m_pm;
};

} // namespace protodb
