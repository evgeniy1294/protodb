#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <protodb/PluginManager.h>
#include <protodb/PluginManagerDialog.h>
#include <protodb/PluginTreeView.h>

PluginTreeView::PluginTreeView(QWidget* parent)
    : QTreeView(parent)
{
    m_pm = &PluginManager::instance();
    m_fm = new QSortFilterProxyModel();
        m_fm->setSourceModel(m_pm);
        m_fm->setRecursiveFilteringEnabled(true);
        m_fm->setFilterKeyColumn(PluginManager::kColumnName);
    QTreeView::setModel(m_fm);

    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);

    QHeaderView* hh = header();
        hh->setStretchLastSection(false);
        hh->setSectionResizeMode(QHeaderView::Fixed);
        hh->setSectionResizeMode(PluginManager::kColumnName, QHeaderView::Stretch);
        hh->setSectionResizeMode(PluginManager::kColumnPluginEnabled, QHeaderView::ResizeToContents);
        hh->setSectionResizeMode(PluginManager::kColumnVersion, QHeaderView::ResizeToContents);
        hh->setSectionResizeMode(PluginManager::kColumnVendor, QHeaderView::ResizeToContents);
        hh->hideSection(PluginManager::kColumnPluginId);
        hh->hideSection(PluginManager::kColumnLocation);
        hh->hideSection(PluginManager::kColumnDescription);
        hh->hideSection(PluginManager::kColumnDependencies);
        hh->hideSection(PluginManager::kColumnLoaded);
}

void PluginTreeView::setModel(QAbstractItemModel* model)
{
    return;
}

QSortFilterProxyModel* PluginTreeView::getFilterModel() const
{
    return m_fm;
}

void PluginTreeView::setFilterFixedString(const QString& pattern)
{
    m_fm->setFilterFixedString(std::move(pattern));
}

void PluginTreeView::createMenu()
{

}

void PluginTreeView::createActions()
{

}

void PluginTreeView::connectSignals()
{

}
