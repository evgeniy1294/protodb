#include "plugins/PluginManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QJsonArray>

class PluginManagerPrivate
{
    struct PluginInfo {
        QString file;
        QString iid;
        QString name;
        QString version;
        QString vendor;
        QString description;
        QStringList relations;
        QStringList deps;

        bool saved_state;
        bool enabled;
        bool loaded;
        bool fault;

        QPluginLoader* loader;
    };

    struct Group {
        QString id;
        QString name;
        QList<PluginInfo> plugins;
    };

public:
    PluginManagerPrivate();
   ~PluginManagerPrivate() = default;

    void loadPlugins(const QMap<QString, bool>& iids);
    void searchPlugins();
    bool enablePlugin (PluginInfo &plugin);
    bool disablePlugin(PluginInfo &plugin);

private:
    QString getNameByIid(const QString& iid, const QString& defaultValue );
    QPair<int /*group*/, int /*plugin*/> findPluginByIid(const QString& iid) const;
    QStringList findConflicts(const PluginInfo& plugin, bool pedantic = false) const;
    QStringList findBrokenDependencies(const PluginInfo& plugin) const;
    void loadPlugin(PluginInfo& plugin, const QMap<QString, bool>& iids);

private:
    bool m_latch;

    QString m_main_directory;
    QString m_manual_install_directory;
    QString m_last_error;
    QList<Group> m_groups;

private:
    Q_DECLARE_PUBLIC(PluginManager);
    PluginManager* q_ptr;
};


PluginManagerPrivate::PluginManagerPrivate()
    : q_ptr(nullptr)
    , m_latch(false)
{

}

void PluginManagerPrivate::loadPlugins(const QMap<QString, bool> &iids)
{
    if (m_latch) {
        return;
    }

    searchPlugins();

    for (auto& group: m_groups) {
        for (auto& plugin: group.plugins) {
            if (iids.contains(plugin.iid)) {
                plugin.enabled = iids[plugin.iid];
            }
        }
    }

    for (auto& group: m_groups) {
        for (auto& plugin: group.plugins) {
            loadPlugin(plugin, iids);
        }
    }

    m_latch = true;
    return;
}

void PluginManagerPrivate::searchPlugins()
{
    Q_Q(PluginManager);

    QDir dir;
        dir.setSorting(QDir::NoSort);
        dir.setFilter(QDir::Files);

    QStringList filePathList;

    dir.setPath(m_main_directory);
    if (dir.exists()) {
       for(auto& file: dir.entryList()) {
           filePathList << dir.absoluteFilePath(file);
       }
    }

    dir.setPath(m_manual_install_directory);
    if (!dir.exists() && m_manual_install_directory != m_main_directory)
    {
        for(auto& file: dir.entryList()) {
            filePathList << dir.absoluteFilePath(file);
        }
    }

    for (const auto& pathToFile: qAsConst(filePathList)) {
        PluginInfo plugin;
        plugin.loader = new QPluginLoader(pathToFile, q);

        auto md = plugin.loader->metaData();
            plugin.iid = md["IID"].toString();

        if (!plugin.iid.contains(QCoreApplication::applicationName())) {
            plugin.loader->deleteLater();
            continue;
        }

        auto user_data = md["MetaData"].toObject();
            plugin.file        =  pathToFile;
            plugin.name        =  user_data["Name"].toString(plugin.iid);
            plugin.version     =  user_data["Version"].toString();
            plugin.vendor      =  user_data["Vendor"].toString();
            plugin.description =  user_data["Description"].toString();
            plugin.enabled     = !user_data["DisabledByDefault"].toBool(false);

            auto depsArray = user_data["Dependencies"].toArray();
            for (const auto& dep: depsArray) {
                auto str = dep.toString();

                if (!str.isEmpty()) {
                    plugin.deps.append(str);
                }
            }

        auto group = user_data["Group"].toString();

        int gidx = -1;
        for (int i = 0; i < m_groups.size(); ++i) {
            if( m_groups.at(i).id == group )
            {
                gidx = i;
                break;
            }
        }

        if( gidx == -1 )
        {
            Group new_group;
                new_group.id = group;

            if(group.isEmpty())
                new_group.name = QObject::tr("No groups");
            else
                new_group.name = group;

            gidx = m_groups.size();

            q->beginInsertRows(QModelIndex(), gidx, gidx);
                m_groups.append(new_group);
            q->endInsertRows();
        }

        Group &cur_group = m_groups[gidx];
        q->beginInsertRows(q->index(gidx, 0), cur_group.plugins.size(), cur_group.plugins.size());
            cur_group.plugins.append(plugin);
        q->endInsertRows();
    }
}

bool PluginManagerPrivate::enablePlugin(PluginInfo &plugin)
{
    if (plugin.enabled) {
        return true;
    }

    auto conflicts = findConflicts(plugin, true);

    if (!conflicts.empty()) {
        m_last_error = QObject::tr("Plugin can't be enabled, because the plugin has conflict: ");

        for (auto& it: conflicts)
            m_last_error.append('\n' + it);

        return false;
    }

    auto brokenDeps = findBrokenDependencies(plugin);

    if (!brokenDeps.empty()) {
        m_last_error = "Plugin can't be enabled, because the plugin has unresolved dependency:";

        for (auto& it: brokenDeps)
            m_last_error.append('\n' + getNameByIid(it, it));

        return false;
    }

    for (auto& iid: plugin.deps) {
        auto [grp, idx] = findPluginByIid(iid);

        if (grp >= 0) {
            auto& dependency = m_groups[grp].plugins[idx];
            dependency.relations.append(plugin.iid);
        }
    }

    plugin.enabled = true;

    return true;
}

bool PluginManagerPrivate::disablePlugin(PluginInfo& plugin)
{
    if (!plugin.enabled) {
        return true;
    }

    if (!plugin.relations.empty()) {
        m_last_error = "Plugin can't be disabled, because the plugin has relations: ";

        for (auto& it: plugin.relations)
            m_last_error.append('\n' + getNameByIid(it, it));

        return false;
    }

    for (auto& iid: plugin.deps) {
        auto [grp, idx] = findPluginByIid(iid);

        if (grp >= 0) {
            auto& dependency = m_groups[grp].plugins[idx];

            if (dependency.relations.contains(plugin.iid)) {
                dependency.relations.removeAll(plugin.iid);
            }
        }
    }

    plugin.enabled = false;

    return true;
}

QString PluginManagerPrivate::getNameByIid(const QString& iid, const QString& defaultValue)
{
    auto [grp, idx] = findPluginByIid(iid);
    if (grp >= 0) {
        auto& plugin = m_groups[grp].plugins[idx];
        return plugin.name;
    }

    return defaultValue;
}

QPair<int, int> PluginManagerPrivate::findPluginByIid(const QString &iid) const
{
    for (int i = 0; i < m_groups.size(); i++) {
        auto& group = m_groups[i];

        for (int j = 0; j < group.plugins.size(); j++) {
            auto& plugin = group.plugins[j];

            if (plugin.iid == iid) {
                return {i, j};
            }
        }
    }

    return {-1, -1};
}

QStringList PluginManagerPrivate::findConflicts(const PluginInfo& plugin, bool pedantic) const
{
    Q_Q(const PluginManager);
    QStringList ret;

    for( int i = 0; i < m_groups.size(); i++ )
    {
        auto &group = m_groups[i];

        for( int j = 0; j < group.plugins.size(); j++ )
        {
            auto& j_plugin = group.plugins.at(j);

            if (plugin.file == j_plugin.file) {
                continue;
            }

            if (j_plugin.iid == plugin.iid) {
                ret << j_plugin.file;
                if (!pedantic) return ret;
            }
        }
    }

    return ret;
}

QStringList PluginManagerPrivate::findBrokenDependencies(const PluginInfo& plugin) const
{
    Q_Q(const PluginManager);
    QStringList ret;

    if (plugin.deps.contains(plugin.iid)) {
        return ret << QObject::tr("Invalid dependency list");
    }

    for (auto& iid: plugin.deps) {
        auto [grp, idx] = findPluginByIid(iid);

        if (grp >= 0) {
            auto& dependency = m_groups[grp].plugins[idx];

            if (dependency.enabled) {
                continue;
            }
        }

        ret << iid;
    }

    return ret;
}

void PluginManagerPrivate::loadPlugin(PluginInfo &plugin, const QMap<QString, bool>& iids)
{
    Q_Q(PluginManager);

    if (!plugin.enabled)
        return;

    plugin.enabled = false;

    if (!findConflicts(plugin).empty()) {
        plugin.fault = true;
        return;
    }

    // TODO: сделать полную проверку на отсутствие циклических зависимостей
    if (plugin.deps.contains(plugin.iid)) {
        plugin.fault = true;
        return;
    }

    for (auto& iid: plugin.deps) {
        auto [grp, idx] = findPluginByIid(iid);

        if (grp >= 0) {
            auto& dependency = m_groups[grp].plugins[idx];
            loadPlugin(dependency, iids);

            if (dependency.loaded) {
                dependency.relations.append(plugin.iid);
                continue;
            }
        }

        return;
    }

    plugin.loaded  =  plugin.loader->load();
    plugin.enabled =  plugin.loaded;
    plugin.fault   = !plugin.loaded;

    return;
}

PluginManager& PluginManager::instance()
{
    static PluginManager _instance;
    return _instance;
}

void PluginManager::loadPlugins(QMap<QString, bool> iids)
{
    Q_D(PluginManager);

    beginResetModel();
        d->loadPlugins(iids);
    endResetModel();

    saveState();
}

QMap<QString, bool> PluginManager::getPluginsState() const
{
    Q_D(const PluginManager);
    QMap<QString, bool> ret;

    for (auto& group: d->m_groups) {
        for (auto& plugin: group.plugins) {
            if (!plugin.fault) {
                ret[plugin.iid] = plugin.enabled;
            }
        }
    }

    return ret;
}

void PluginManager::setMainDirectory(const QString &dir)
{
    Q_D(PluginManager);
    d->m_main_directory = dir;
}

void PluginManager::setManualInstallDirectory(const QString &dir)
{
    Q_D(PluginManager);
    d->m_manual_install_directory = dir;
}

QString PluginManager::mainDirectory() const
{
    Q_D(const PluginManager);
    return d->m_main_directory;
}

QString PluginManager::manualInstallDirectory() const
{
    Q_D(const PluginManager);
    return d->m_manual_install_directory;
}

void PluginManager::setGroupName(const QString &group_id, const QString &name)
{
    Q_D(PluginManager);

    for( int i = 0; i < d->m_groups.size(); ++i )
    {
        if( d->m_groups.at(i).id != group_id )
            continue;

        d->m_groups[i].name = name;
        emit dataChanged( index( i, PluginManager::kColName ),
                          index( i, PluginManager::kColName ),
                          QVector<int>() << Qt::DisplayRole );
        return;
    }
}

QList<QPluginLoader *> PluginManager::getLoaders(const QString &group)
{
    Q_D(PluginManager);
    QList<QPluginLoader*> ret;

    for (auto &it: d->m_groups)  {
        if (group.isEmpty() || group == it.id) {
            for (auto&& plugin: qAsConst(it.plugins)) {
                if (plugin.loaded) {
                    ret.append(plugin.loader);
                }
            }
        }
    }

    return ret;
}

QString PluginManager::lastError() const
{
    Q_D(const PluginManager);
    return d->m_last_error;
}

void PluginManager::clearError()
{
    Q_D(PluginManager);
    d->m_last_error.clear();
}

void PluginManager::saveState()
{
    Q_D(PluginManager);

    for (auto& group: d->m_groups) {
        for (auto& plugin: group.plugins) {
            if (!plugin.fault) {
                plugin.saved_state = plugin.enabled;
            }
        }
    }

    return;
}

void PluginManager::restoreState()
{
    Q_D(PluginManager);

    beginResetModel();

    for (auto& group: d->m_groups) {
        for (auto& plugin: group.plugins) {
            if (!plugin.fault) {
                plugin.enabled = plugin.saved_state;
            }
        }
    }

    endResetModel();
}

void PluginManager::resetState()
{
    Q_D(PluginManager);

    beginResetModel();

    for (auto& group: d->m_groups) {
        for (auto& plugin: group.plugins) {
            if (!plugin.fault) {
                plugin.enabled = plugin.loaded;
            }
        }
    }

    endResetModel();
}


int PluginManager::rowCount(const QModelIndex &parent) const
{
    Q_D(const PluginManager);
    int ret = 0;

    if (!parent.isValid()) {
        ret = d->m_groups.size();
    }
    else {
        if (hasChildren(parent)) {
            int p_row = parent.row();
            if( p_row >= 0 && p_row < d->m_groups.size() )
                ret = d->m_groups.at( p_row ).plugins.size();
        }
    }

    return ret;
}

int PluginManager::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return kColCount;
}

QVariant PluginManager::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {

        if (role == Qt::DisplayRole) {
            switch(section) {
                case kColName:
                    return tr("Name");
                case kColEnabled:
                    return tr("Enable");
                case kColGroup:
                    return tr("Group");
                case kColIid:
                    return tr("IID");
                case kColVersion:
                    return tr("Version");
                case kColVendor:
                    return tr("Vendor");
                case kColFile:
                    return tr("Location");
                case kColDescription:
                    return tr("Description");

                default:
                    break;
            }
        }

    }

    return QVariant();
}

QVariant PluginManager::data(const QModelIndex &index, int role) const
{
    Q_D(const PluginManager);

    if ( !index.isValid() )
        return QVariant();

    if (index.parent().isValid()) {
        // Это плагин
        int row = index.internalId()-1;
        if( row < 0 || row >= d->m_groups.size() )
            return QVariant();

        const auto &group = d->m_groups.at(row);
        if( index.row() < 0 || index.row() >= group.plugins.size() )
            return QVariant();

        const auto &plugin = group.plugins.at( index.row() );

        switch ( role )
        {
            case Qt::DecorationRole: {
                if (index.column() == kColName) {
                    /* Перенести функционал в делегат
                    if ( plugin.loaded ) {
                        return QIcon(":/icons/check.svg");
                    }
                    else if ( plugin.fault ) {
                        return QIcon(":/icons/red_cross.svg");
                    }
                    else {
                        return QIcon(":/icons/orange_minus.svg");
                    }
                    */
                }

                break;
            }

            case Qt::ForegroundRole: {
                /* Перенести функционал в делегат
                if( plugin.fault )
                    return QColor(Qt::red);
                else
                    return {};
                */
            }

            case Qt::DisplayRole:
            case Qt::ToolTipRole:
            case Qt::EditRole: {
                switch( index.column() )
                {
                    case kColName:
                        return plugin.name;

                    case kColEnabled:
                        if( role == Qt::EditRole ) {
                            return plugin.enabled ? Qt::Checked : Qt::Unchecked;
                        }
                        break;

                    case kColIid:
                        return plugin.iid;

                    case kColGroup:
                        return group.name;

                    case kColVersion:
                        return plugin.version;

                    case kColVendor:
                        return plugin.vendor;

                    case kColFile:
                        return plugin.file;

                    case kColDescription:
                        return plugin.description;

                    default:
                        break;
                }
                break;
            }

            case Qt::CheckStateRole: {
                if( index.column() == kColEnabled )
                {
                    return plugin.enabled ? Qt::Checked : Qt::Unchecked;
                }
                break;
            }
        }
    }
    else {
        if( index.row() >= 0 && index.row() < d->m_groups.size() )
        {
            const auto &group = d->m_groups.at( index.row() );

            if( role == Qt::DisplayRole )
            {
                if( index.column() == kColName )
                    return group.name;
            }
        }
    }

    return QVariant();
}

bool PluginManager::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D( PluginManager );
    bool ret = false;

    if( ! index.isValid() )
        return ret;

    if( Qt::CheckStateRole != role )
        return ret;

    if( index.column() != kColEnabled )
        return ret;

    if( index.parent().isValid() )
    {
        int row = index.internalId()-1;
        if( row < 0 || row >= d->m_groups.size() )
            return ret;

        auto& group = d->m_groups[row];
        if( index.row() < 0 || index.row() >= group.plugins.size() )
            return ret;

        auto& plugin = group.plugins[index.row()];
        const auto state = value.toInt() == Qt::Checked;

        if( plugin.enabled != state )
        {
            if( state )
                ret = d->enablePlugin(plugin);
            else
                ret = d->disablePlugin(plugin);

            if( ret ) {
                emit dataChanged( index, index, {Qt::CheckStateRole} );
            }

            return ret;
        }
    }

    return ret;
}

QModelIndex PluginManager::index(int row, int col, const QModelIndex &parent) const
{
    if( ! hasIndex(row, col, parent) )
        return QModelIndex();

    return createIndex( row, col, parent.isValid() ? parent.row()+1 : quintptr(0) );
}

Qt::ItemFlags PluginManager::flags(const QModelIndex &index) const
{
    if( index.internalId() )
    {
        if( index.column() == kColEnabled )
            return Qt::ItemIsEnabled |
                   Qt::ItemIsSelectable |
                   Qt::ItemNeverHasChildren |
                   Qt::ItemIsUserCheckable;
        else
            return Qt::ItemIsEnabled |
                   Qt::ItemIsSelectable |
                   Qt::ItemNeverHasChildren;
    }

    return Qt::ItemIsEnabled;
}

QModelIndex PluginManager::parent(const QModelIndex& index) const
{
    return index.internalId() > 0 ? createIndex( index.internalId()-1, 0, quintptr(0) ) : QModelIndex();
}

bool PluginManager::hasChildren(const QModelIndex &parent) const
{
    return parent.internalId() <= 0;
}

QModelIndex PluginManager::sibling(int row, int column, const QModelIndex &index) const
{
    return createIndex( row, column, index.internalId() );
}

PluginManager::PluginManager(QObject *parent)
    : QAbstractItemModel( parent )
    , d_ptr( new PluginManagerPrivate() )
{
    Q_D(PluginManager);
    d->q_ptr = this;
}

PluginManager::~PluginManager()
{
    Q_D(PluginManager);
    delete d;
}

void PluginManager::mark_fault(const QString &group, QPluginLoader *loader)
{
    Q_D(PluginManager);

    for( int i = 0; i < d->m_groups.size(); ++i )
    {
        auto &i_group = d->m_groups[i];

        for( int j = 0; j < i_group.plugins.size(); ++ j )
        {
            auto &j_plugin = i_group.plugins[j];
            if( j_plugin.loader == loader )
            {
                j_plugin.fault   = true;
                j_plugin.enabled = false;
                j_plugin.loaded  = false;

                auto parent = index( i, 0 );
                auto j_index = index( j, kColEnabled, parent );

                emit dataChanged( j_index, j_index, {Qt::DisplayRole} );
                emit dataChanged( index( j, 0, parent ),
                                  index( j, kColCount, parent ),
                                  { Qt::BackgroundRole} );

                return;
            }
        }
    }
}
