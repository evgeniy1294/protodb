#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QDataWidgetMapper>

#include <protodb/PluginManager.h>
#include <protodb/PluginManagerDialog.h>
#include <protodb/PluginDetailDialog.h>
#include <protodb/PluginTreeView.h>

PluginTreeView::PluginTreeView(QWidget* parent)
    : QTreeView(parent)
{
    m_pm = &PluginManager::instance();
    m_fm = new QSortFilterProxyModel();
        m_fm->setSourceModel(m_pm);
        m_fm->setRecursiveFilteringEnabled(true);
        m_fm->setFilterKeyColumn(PluginManager::kColumnName);
        m_fm->setFilterCaseSensitivity(Qt::CaseInsensitive);
    QTreeView::setModel(m_fm);

    setContextMenuPolicy(Qt::CustomContextMenu);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);


    QHeaderView* hh = header();
        hh->setStretchLastSection(false);
        hh->setSectionResizeMode(QHeaderView::Fixed);
        hh->setSectionResizeMode(PluginManager::kColumnName, QHeaderView::Stretch);
        hh->setSectionResizeMode(PluginManager::kColumnVersion, QHeaderView::ResizeToContents);
        hh->setSectionResizeMode(PluginManager::kColumnVendor, QHeaderView::ResizeToContents);
        hh->hideSection(PluginManager::kColumnPluginId);
        hh->hideSection(PluginManager::kColumnGroup);
        hh->hideSection(PluginManager::kColumnLocation);
        hh->hideSection(PluginManager::kColumnDescription);
        hh->hideSection(PluginManager::kColumnDependencies);
        hh->hideSection(PluginManager::kColumnLoaded);

    m_mapper = new QDataWidgetMapper(this);
        m_mapper->setModel(m_pm);
        m_mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    m_detail_dialog = new PluginDetailDialog(this);
        m_detail_dialog->resize(540, 480);
        m_detail_dialog->setMapper(m_mapper);

    connectSignals();
}

void PluginTreeView::setModel(QAbstractItemModel* model)
{
    return;
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
    connect(this, &PluginTreeView::doubleClicked, this, [this](const QModelIndex &a_index) {
        auto index = m_fm->mapToSource(a_index);

        if (!m_pm->hasChildren(index)) {
            m_mapper->setRootIndex(index.parent());
            m_mapper->setCurrentModelIndex(index);

            m_detail_dialog->show();
        }
    });
}
