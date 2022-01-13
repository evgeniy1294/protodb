#include <protodb/PluginManagerNew.h>

#include <QApplication>
#include <QBrush>
#include <QDir>
#include <QJsonArray>

class PluginManagerNewPrivate
{
    struct PluginInfo { // Нужен конструктор по-умолчанию
        QString file;
        QString iid;
        QString name;
        QString version;
        QString vendor;
        QString description;
        QStringList relations;
        QStringList deps;

        int state;

        QPluginLoader* loader;
    };

    struct Group {
        QString id;
        QString name;
        QList<PluginInfo> plugins;
    };

public:
    enum StateFlag: int {
        State_Enabled = 0x00000001,
        State_Loaded  = 0x00000002,
        State_Fault   = 0x00000004,
    };
    Q_DECLARE_FLAGS(State, StateFlag)

public:
    PluginManagerNewPrivate();
   ~PluginManagerNewPrivate() = default;

    void loadPlugins(const QMap<QString, bool>& iids);
    void searchPlugins();
    bool enablePlugin (PluginInfo &plugin);
    bool disablePlugin(PluginInfo &plugin);

    bool isEnabled(const PluginInfo& plugin) const;
    bool isDisabled(const PluginInfo& plugin) const;
    bool isFault(const PluginInfo& plugin) const;
    bool isLoaded(const PluginInfo& plugin) const;

private:
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
    Q_DECLARE_PUBLIC(PluginManagerNew);
    PluginManagerNew* q_ptr;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PluginManagerNewPrivate::State)

PluginManagerNewPrivate::PluginManagerNewPrivate()
    : q_ptr(nullptr)
    , m_latch(false)
{

}

void PluginManagerNewPrivate::loadPlugins(const QMap<QString, bool> &iids)
{
    if (m_latch) {
        return;
    }

    searchPlugins();

    for (auto& group: m_groups) {
        for (auto& plugin: group.plugins) {
            loadPlugin(plugin, iids);
        }
    }

    m_latch = true;
    return;
}

void PluginManagerNewPrivate::searchPlugins()
{
    Q_Q(PluginManagerNew);

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
        plugin.loader = new QPluginLoader(pathToFile);

        auto md = plugin.loader->metaData();
            plugin.iid = md["IID"].toString();

        if (!plugin.iid.contains(QApplication::applicationName())) {
            plugin.loader->deleteLater();
            continue;
        }

        auto user_data = md["MetaData"].toObject();
            plugin.file        = pathToFile;
            plugin.name        = user_data["Name"].toString(plugin.iid);
            plugin.version     = user_data["Version"].toString();
            plugin.vendor      = user_data["Vendor"].toString();
            plugin.description = user_data["Description"].toString();
            plugin.state       = user_data["DisabledByDefault"].toBool(false) ? 0 : State_Enabled;

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

bool PluginManagerNewPrivate::enablePlugin(PluginInfo &plugin)
{
    if (isEnabled(plugin)) {
        return true;
    }

    auto conflicts = findConflicts(plugin, true);

    if (!conflicts.empty()) {
        m_last_error = "Plugin can't be enabled, because the plugin has conflict:";

        for (auto& it: conflicts)
            m_last_error.append(' ' + it);

        return false;
    }

    auto brokenDeps = findBrokenDependencies(plugin);

    if (!brokenDeps.empty()) {
        m_last_error = "Plugin can't be enabled, because the plugin has unresolved dependency:";

        for (auto& it: brokenDeps)
            m_last_error.append(' ' + it);

        return false;
    }

    plugin.state |= State_Enabled;

    return true;
}

bool PluginManagerNewPrivate::disablePlugin(PluginInfo& plugin)
{
    if (isDisabled(plugin)) {
        return true;
    }

    if (!plugin.relations.empty()) {
        m_last_error = "Plugin can't be enabled, because the plugin has many relations:";

        for (auto& it: plugin.relations)
            m_last_error.append(' ' + it);

        return false;
    }

    plugin.state &= ~State_Enabled;

    return true;
}

bool PluginManagerNewPrivate::isEnabled(const PluginInfo &plugin) const
{
    return (plugin.state & State_Enabled) != 0;
}

bool PluginManagerNewPrivate::isDisabled(const PluginInfo &plugin) const
{
    return (plugin.state & State_Enabled) == 0;
}

bool PluginManagerNewPrivate::isFault(const PluginInfo &plugin) const
{
    return (plugin.state & State_Fault) != 0;
}

bool PluginManagerNewPrivate::isLoaded(const PluginInfo &plugin) const
{
    return (plugin.state & State_Loaded) != 0;
}

QPair<int, int> PluginManagerNewPrivate::findPluginByIid(const QString &iid) const
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

QStringList PluginManagerNewPrivate::findConflicts(const PluginInfo& plugin, bool pedantic) const
{
    Q_Q(const PluginManagerNew);
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

QStringList PluginManagerNewPrivate::findBrokenDependencies(const PluginInfo& plugin) const
{
    Q_Q(const PluginManagerNew);
    QStringList ret;

    if (plugin.deps.contains(plugin.iid)) {
        return ret << QObject::tr("Invalid dependency list");
    }

    for (auto& iid: plugin.deps) {
        auto [grp, idx] = findPluginByIid(iid);

        if (grp >= 0) {
            auto dependency = m_groups[grp].plugins.at(idx);

            if (isEnabled(dependency)) {
                continue;
            }
        }

        ret << plugin.name;
    }

    return ret;
}

void PluginManagerNewPrivate::loadPlugin(PluginInfo &plugin, const QMap<QString, bool>& iids)
{
    Q_Q(PluginManagerNew);

    if (iids.contains(plugin.iid)) {

        if (iids[plugin.iid])
            plugin.state |= State_Enabled;
        else
            plugin.state &= ~State_Enabled;

    }

    if (isDisabled(plugin))
        return;

    plugin.state &= ~State_Enabled;

    if (!findConflicts(plugin).empty()) {
        plugin.state |= State_Fault;
        return;
    }

    if (plugin.deps.contains(plugin.iid)) {
        plugin.state |= State_Fault;
        return;
    }

    for (auto& iid: plugin.deps) {
        auto [grp, idx] = findPluginByIid(iid);

        if (grp >= 0) {
            auto dependency = m_groups[grp].plugins.at(idx);
            loadPlugin(dependency, iids);

            if (isLoaded(dependency)) {
                if (!dependency.relations.contains(plugin.iid)) {
                    dependency.relations << plugin.iid;
                }
                continue;
            }
        }

        plugin.state |= State_Fault;

        return;
    }

    plugin.state |= (plugin.loader->load()) ? State_Loaded : State_Fault;
    plugin.state |= isLoaded(plugin) ? State_Enabled : 0;

    return;
}

PluginManagerNew& PluginManagerNew::instance()
{
    static PluginManagerNew _instance;
    return _instance;
}

void PluginManagerNew::loadPlugins(QMap<QString, bool> iids)
{
    Q_D(PluginManagerNew);

    beginResetModel();
        d->loadPlugins(iids);
    endResetModel();
}

void PluginManagerNew::setMainDirectory(const QString &dir)
{
    Q_D(PluginManagerNew);
    d->m_main_directory = dir;
}

void PluginManagerNew::setManualInstallDirectory(const QString &dir)
{
    Q_D(PluginManagerNew);
    d->m_manual_install_directory = dir;
}

QString PluginManagerNew::mainDirectory() const
{
    Q_D(const PluginManagerNew);
    return d->m_main_directory;
}

QString PluginManagerNew::manualInstallDirectory() const
{
    Q_D(const PluginManagerNew);
    return d->m_manual_install_directory;
}

void PluginManagerNew::setGroupName(const QString &group_id, const QString &name)
{
    Q_D(PluginManagerNew);

    for( int i = 0; i < d->m_groups.size(); ++i )
    {
        if( d->m_groups.at(i).id != group_id )
            continue;

        d->m_groups[i].name = name;
        emit dataChanged( index( i, PluginManagerNew::kColName ),
                          index( i, PluginManagerNew::kColName ),
                          QVector<int>() << Qt::DisplayRole );
        return;
    }
}

int PluginManagerNew::rowCount(const QModelIndex &parent) const
{
    Q_D(const PluginManagerNew);
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

int PluginManagerNew::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return kColCount;
}

QVariant PluginManagerNew::headerData(int section, Qt::Orientation orientation, int role) const
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

QVariant PluginManagerNew::data(const QModelIndex &index, int role) const
{
    Q_D(const PluginManagerNew);

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
            case Qt::ForegroundRole: {

                if( d->isFault(plugin) )
                    return QColor(Qt::red);
                else
                    return {};
            }

            case Qt::DisplayRole:
            case Qt::ToolTipRole:
            case Qt::EditRole: {
                switch( index.column() )
                {
                    case kColName:
                        return plugin.name;

                    case kColEnabled:
                        if( role == Qt::EditRole )
                        {
                            auto enabled = d->isEnabled(plugin);
                            return enabled ? Qt::Checked : Qt::Unchecked;
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
                    auto enabled = d->isEnabled(plugin);
                    return enabled ? Qt::Checked : Qt::Unchecked;
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

bool PluginManagerNew::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_D( PluginManagerNew );
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
        const auto old_state = d->isEnabled(plugin);
        const auto new_state = value.toInt() == Qt::Checked;

        if( old_state != new_state )
        {
            if( new_state )
                ret = d->enablePlugin(plugin);
            else
                ret = d->disablePlugin(plugin);

            if( ret )
                emit dataChanged( index, index, {Qt::CheckStateRole} );

            return ret;
        }
    }

    return ret;
}

QModelIndex PluginManagerNew::index(int row, int col, const QModelIndex &parent) const
{
    if( ! hasIndex(row, col, parent) )
        return QModelIndex();

    return createIndex( row, col, parent.isValid() ? parent.row()+1 : quintptr(0) );
}

Qt::ItemFlags PluginManagerNew::flags(const QModelIndex &index) const
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

QModelIndex PluginManagerNew::parent(const QModelIndex& index) const
{
    return index.internalId() > 0 ? createIndex( index.internalId()-1, 0, quintptr(0) ) : QModelIndex();
}

bool PluginManagerNew::hasChildren(const QModelIndex &parent) const
{
    return parent.internalId() <= 0;
}

QModelIndex PluginManagerNew::sibling(int row, int column, const QModelIndex &index) const
{
    return createIndex( row, column, index.internalId() );
}

PluginManagerNew::PluginManagerNew(QObject *parent)
    : QAbstractItemModel( parent )
    , d_ptr( new PluginManagerNewPrivate() )
{
    Q_D(PluginManagerNew);
    d->q_ptr = this;
}

PluginManagerNew::~PluginManagerNew()
{
    // Через new были созданы QPluginLoader'ы, их нужно грохнуть
}
