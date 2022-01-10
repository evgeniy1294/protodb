#include <QDir>
#include <QBrush>
#include <QJsonArray>
#include <QApplication>
#include <protodb/PluginManager.h>

class PluginManagerPrivate
{
public:
    PluginManagerPrivate();
   ~PluginManagerPrivate() = default;

    void LoadPlugins(const QString& path);

    // New API
    void searchPlugins();


private:
    struct PluginInfo {
        QString id;
        QString file_name; // Под удаление
        QString path;
        QString name;
        QString description;
        QString version;
        QString vendor;
        QStringList deps;

        bool conflict = false;
        bool used = false;
        bool broken_dependency = false;

        QPluginLoader* loader;
    };

    struct Group {
        QString id;
        QString name;
        QList<PluginInfo> plugins;
    };

private:
    Q_DECLARE_PUBLIC(PluginManager);
    PluginManager* q_ptr;

private:
    QStringList m_disabled_ids;
    QStringList m_app_directory_list;
    QStringList m_user_directory_list;
    QList<Group> m_groups;

    bool m_has_conflict;

private: // New API
    QString m_base_directory;
    QString m_manual_install_directory;
};

PluginManager& PluginManager::instance()
{
    static PluginManager manager;
    return manager;
}

void PluginManager::setDirectories(const QStringList& dirs)
{
    Q_D(PluginManager);
    d->m_app_directory_list = dirs;
}

void PluginManager::addDirectory(const QString& dir)
{
    Q_D(PluginManager);
    if (!d->m_app_directory_list.contains(dir)) {
        d->m_app_directory_list.append(dir); // Теоретически можно запихать одну и ту же директорию. Нужно делать через absolute path
    }
}

void PluginManager::removeDirectory(const QString& dir)
{
    Q_D(PluginManager);
    if (d->m_app_directory_list.contains(dir)) {
        d->m_app_directory_list.removeAll(dir);
    }
}

QStringList PluginManager::getDirectories() const
{
    Q_D(const PluginManager);
    return d->m_app_directory_list;
}

void PluginManager::setUserDirectories(const QStringList& dirs)
{
    Q_D(PluginManager);
    d->m_user_directory_list = dirs;
}

void PluginManager::addUserDirectory(const QString& dir)
{
    Q_D(PluginManager);
    if (!d->m_user_directory_list.contains(dir)) {
        d->m_user_directory_list.append(dir); // Теоретически можно запихать одну и ту же директорию. Нужно делать через absolute path
    }
}

void PluginManager::removeUserDirectory(const QString& dir)
{
    Q_D(PluginManager);
    if (d->m_user_directory_list.contains(dir)) {
        d->m_user_directory_list.removeAll(dir);
    }
}

QStringList PluginManager::getUserDirectories() const
{
    Q_D(const PluginManager);
    return d->m_user_directory_list;
}

void PluginManager::setGroupName(const QString& group, const QString& name)
{
    Q_D(PluginManager);

    for( int i = 0; i < d->m_groups.size(); ++i )
    {
        if( d->m_groups.at(i).id != group )
            continue;

        d->m_groups[i].name = name;
        emit dataChanged( index( i, PluginManager::kColumnName ),
                          index( i, PluginManager::kColumnName ),
                          QVector<int>() << Qt::DisplayRole );
        return;
    }
}

bool PluginManager::hasBrokenDependencies() const
{
    return false;
}

bool PluginManager::hasConflict() const
{
    Q_D(const PluginManager);
    return d->m_has_conflict;
}

QStringList PluginManager::getDisabledIds() const
{
    Q_D(const PluginManager);
    return d->m_disabled_ids;
}

void PluginManager::setDisabledIds(const QStringList& ids)
{
    Q_D(PluginManager);
    d->m_disabled_ids = ids;
}

QList<QPluginLoader*> PluginManager::getLoaders(const QString &group)
{
    Q_D(PluginManager);
    QList< QPluginLoader *> ret;

    for (auto &it: d->m_groups)  {
        if (group.isEmpty() || group == it.id) {
            for (auto&& plugin: qAsConst(it.plugins)) {
                ret.append(plugin.loader);
            }
        }
    }

    return ret;
}

int PluginManager::rowCount(const QModelIndex& parent) const
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

int PluginManager::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return kColumnCount;
}

QVariant PluginManager::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal) {

        if (role == Qt::DisplayRole) {
            switch(section) {
                case kColumnName:
                    return tr("Name");
                case kColumnPluginId:
                    return tr("IID");
                case kColumnGroup:
                    return tr("Group");
                case kColumnVersion:
                    return tr("Version");
                case kColumnVendor:
                    return tr("Vendor");
                case kColumnLocation:
                    return tr("Location");
                case kColumnDescription:
                    return tr("Description");
                case kColumnDependencies:
                    return tr("Dependencies");
                case kColumnLoaded:
                    return tr("State");

                default:
                    break;
            }
        }

    }

    return QVariant();
}

QVariant PluginManager::data(const QModelIndex& index, int role) const
{
    Q_D(const PluginManager);

    if ( !index.isValid() ) {
        return QVariant();
    }

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
            case Qt::BackgroundRole:
            {
                if( plugin.conflict )
                    return QBrush(Qt::red);
                else
                    return {};
            }

            case Qt::EditRole: [[fallthrough]];
            case Qt::DisplayRole:
            {
                switch( index.column() )
                {
                    case kColumnName:
                        if( plugin.name.isEmpty() && role == Qt::DisplayRole)
                            return plugin.id;
                        return plugin.name;

                    case kColumnPluginId:
                        return plugin.id;

                    case kColumnGroup:
                        return group.name;

                    case kColumnVersion:
                        return plugin.version;

                    case kColumnVendor:
                        return plugin.vendor;

                    case kColumnLocation:
                        return plugin.path + "/" + plugin.file_name;

                    case kColumnDescription:
                        return plugin.description;

                    case kColumnDependencies:
                        return plugin.deps;

                    case kColumnLoaded:
                        return plugin.used ? tr("Load") : tr("Unload");

                    default:
                        break;
                }
                break;
            }

            case Qt::CheckStateRole: {
                if( index.column() == kColumnName )
                {
                    auto disabled = d->m_disabled_ids.contains( plugin.id );
                    return disabled ? Qt::Unchecked : Qt::Checked;
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
                if( index.column() == kColumnName )
                    return group.name;
            }
        }
    }

    return QVariant();
}

bool PluginManager::setData(const QModelIndex& index, const QVariant& value, int role)
{
    Q_D( PluginManager );

    if( ! index.isValid() )
        return false;

    if( Qt::CheckStateRole != role )
        return false;

    if( index.column() != kColumnName )
        return false;

    if( index.parent().isValid() )
    {
        int row = index.internalId()-1;
        if( row < 0 || row >= d->m_groups.size() )
            return false;

        const auto &group = d->m_groups.at( row );
        if( index.row() < 0 || index.row() >= group.plugins.size() )
            return false;

        const auto &plugin = group.plugins.at( index.row() );

        auto dis_inx = d->m_disabled_ids.indexOf( plugin.id );
        const auto old_state = dis_inx == -1; // Нет в списке отключенных
        const auto new_state = value.toInt() == Qt::Checked;

        if( old_state != new_state )
        {
            if( new_state )
                d->m_disabled_ids.removeAt( dis_inx );
            else
                d->m_disabled_ids.append( plugin.id );

            if( ! plugin.conflict )
                emit dataChanged( index, index, {Qt::CheckStateRole} );
            else
            {
                // Поиск конфликтного плагина, для оповещения представления
                for( int i = 0; i < d->m_groups.size(); ++i )
                {
                    auto &i_group = d->m_groups[ i ];
                    for( int j = 0; j < i_group.plugins.size(); ++ j )
                    {
                        auto &j_plugin = i_group.plugins[ j ];
                        if( j_plugin.id == plugin.id )
                        {
                            auto parent = PluginManager::index( i, 0 );
                            auto i_index = PluginManager::index( j, kColumnName, parent );
                            emit dataChanged( i_index, i_index, {Qt::CheckStateRole} );
                        }
                    }
                }
            }

            return true;
        }
    }

    return false;
}

PluginManager::PluginManager(QObject* parent)
    : QAbstractItemModel(parent)
    , d_ptr(new PluginManagerPrivate)
{
    Q_D(PluginManager);
    d->q_ptr = this;
}

PluginManager::~PluginManager()
{
    // Выгрузка всех плагинов?
}

void PluginManager::mark_loaded(const QString& group, QPluginLoader* loader)
{
    Q_D(PluginManager);

    for( int i = 0; i < d->m_groups.size(); ++i )
    {
        auto &i_group = d->m_groups[ i ];
        for( int j = 0; j < i_group.plugins.size(); ++ j )
        {
            auto &j_plugin = i_group.plugins[ j ];
            if( j_plugin.loader == loader )
            {
                j_plugin.used = true;
                auto parent = index( i, 0 );
                auto j_index = index( j, kColumnLoaded, parent );
                emit dataChanged( j_index, j_index, {Qt::DisplayRole} );
                emit dataChanged( index( j, 0, parent ),
                                  index( j, kColumnLoaded, parent ),
                                  { Qt::BackgroundRole} );

                return;
            }
        }
    }
}


QModelIndex PluginManager::index( int row, int col, const QModelIndex &parent ) const
{
    if( ! hasIndex(row, col, parent) )
        return QModelIndex();

    return createIndex( row, col, parent.isValid() ? parent.row()+1 : quintptr(0) );
}

Qt::ItemFlags PluginManager::flags(const QModelIndex& index) const
{
    if( index.internalId() > 0)
    {
        if( index.column() == kColumnName )
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

void PluginManager::setBaseDirectory(const QString& dir)
{
    Q_D(PluginManager);
    d->m_base_directory = dir;
}

void PluginManager::setManualInstallDirectory(const QString& dir)
{
    Q_D(PluginManager);
    d->m_manual_install_directory = dir;
}

QString PluginManager::baseDirectory() const
{
    Q_D(const PluginManager);
    return d->m_base_directory;
}

QString PluginManager::manualInstallDirectory() const
{
    Q_D(const PluginManager);
    return d->m_manual_install_directory;
}

void PluginManager::loadPlugins()
{
    Q_D(PluginManager);
    for(QString &dir_path: d->m_user_directory_list)
        d->LoadPlugins(dir_path);

    for(QString &dir_path: d->m_app_directory_list)
        d->LoadPlugins(dir_path);
}

void PluginManager::unloadPlugins()
{
    Q_D(PluginManager);
    for( auto it = d->m_groups.cbegin(); it != d->m_groups.cend(); ++it )
        for( auto jt = it->plugins.cbegin(); jt != it->plugins.cend(); ++jt )
            jt->loader->unload();
}

PluginManagerPrivate::PluginManagerPrivate()
    : q_ptr(nullptr)
    , m_has_conflict(false)
{

}

void PluginManagerPrivate::LoadPlugins(const QString& path)
{
    Q_Q(PluginManager);

    QDir dir(path);
        dir.setNameFilters(QStringList() << "*.so" << "*.dll");
    auto files = dir.entryList(QDir::Files);


    for (const auto& file: qAsConst(files)) {
        PluginInfo plugin;
        plugin.loader = new QPluginLoader(dir.absoluteFilePath(file));

        auto md = plugin.loader->metaData();
            plugin.id = md["IID"].toString();

        if (!plugin.id.contains(QApplication::applicationName())) {
            plugin.loader->deleteLater();
            continue;
        }

        auto user_data = md["MetaData"].toObject();
        auto group_id = user_data["group"].toString();

        // Поиск группы среди известных
        int group_index = -1;
        for (int i = 0; i < m_groups.size(); ++i) {
            if( m_groups.at( i ).id == group_id )
            {
                group_index = i;
                break;
            }
        }

        // Если группа неизвестна, добавляем новую
        if( group_index == -1 )
        {
            Group new_group;
            new_group.id = group_id;
            if( group_id.isEmpty() )
                new_group.name = QObject::tr("No groups");
            else
                new_group.name = group_id;

            group_index = m_groups.size();
            q->beginInsertRows( QModelIndex(), group_index, group_index );
            m_groups.append( new_group );
            q->endInsertRows();
        }

        plugin.file_name = file;
        plugin.path = path;
        plugin.name = user_data["name"].toString();
        plugin.version = user_data["version"].toString();
        plugin.description = user_data[ "description" ].toString();
        plugin.vendor  = user_data["vendor"].toString();

        auto depsArray = user_data["dependencies"].toArray();
        for (const auto& dep: depsArray) {
            auto str = dep.toString();

            if (!str.isEmpty()) {
                plugin.deps.append(str);
            }
        }

        bool load = ! m_disabled_ids.contains( plugin.id );

        if (load) {
            if (!plugin.loader->load()) {
                plugin.loader->deleteLater(); // А сам плагин останется + останется группа
            }
        }

        // Поиск конфликта
        for( int i = 0; i < m_groups.size(); ++i )
        {
            auto &i_group = m_groups[ i ];
            for( int j = 0; j < i_group.plugins.size(); ++ j )
            {
                auto &j_plugin = i_group.plugins[ j ];
                if( j_plugin.id == plugin.id )
                {
                    plugin.conflict = true;
                    j_plugin.conflict = true;
                    auto parent = q->index( i, 0 );
                    emit q->dataChanged( q->index(j, 0, parent ),
                                         q->index(j, PluginManager::kColumnCount-1, parent ),
                                         {Qt::BackgroundRole});
                    m_has_conflict = true;
                    emit q->sConflictDetected( plugin.id );
                }
            }
        }

        // Добавление в модель плагина
        Group &cur_group = m_groups[ group_index ];
        q->beginInsertRows( q->index( group_index, 0), cur_group.plugins.size(), cur_group.plugins.size() );
        cur_group.plugins.append( plugin );
        q->endInsertRows();
    }
}

void PluginManagerPrivate::searchPlugins()
{
    Q_Q(PluginManager);

    QDir dir;
        dir.setNameFilters({"*.so", "*.dll"});
        dir.setSorting(QDir::NoSort);
        dir.setFilter(QDir::Files);

    QStringList filePathList;

    if (!m_base_directory.isEmpty()) {
       dir.setPath(m_base_directory);
       for(auto& file: dir.entryList()) {
           filePathList << dir.absoluteFilePath(file);
       }
    }

    if (!m_manual_install_directory.isEmpty() &&
            m_manual_install_directory != m_base_directory)
    {
        dir.setPath(m_manual_install_directory);
        for(auto& file: dir.entryList()) {
            filePathList << dir.absoluteFilePath(file);
        }
    }

    for (const auto& pathToFile: qAsConst(filePathList)) {
        PluginInfo plugin;
        plugin.loader = new QPluginLoader(pathToFile);


        auto md = plugin.loader->metaData();
            plugin.id = md["IID"].toString();

        if (!plugin.id.contains(QApplication::applicationName())) {
            plugin.loader->deleteLater();
            continue;
        }

        auto user_data = md["MetaData"].toObject();
        auto group_id = user_data["group"].toString();

        // Поиск группы среди известных
        int group_index = -1;
        for (int i = 0; i < m_groups.size(); ++i) {
            if( m_groups.at( i ).id == group_id )
            {
                group_index = i;
                break;
            }
        }

        // Если группа неизвестна, добавляем новую
        if( group_index == -1 )
        {
            Group new_group;
            new_group.id = group_id;
            if( group_id.isEmpty() )
                new_group.name = QObject::tr("No groups");
            else
                new_group.name = group_id;

            group_index = m_groups.size();
            q->beginInsertRows( QModelIndex(), group_index, group_index );
            m_groups.append( new_group );
            q->endInsertRows();
        }

        plugin.path        = pathToFile;
        plugin.name        = user_data["name"].toString();
        plugin.version     = user_data["version"].toString();
        plugin.description = user_data[ "description" ].toString();
        plugin.vendor      = user_data["vendor"].toString();

        auto depsArray = user_data["dependencies"].toArray();
        for (const auto& dep: depsArray) {
            auto str = dep.toString();

            if (!str.isEmpty()) {
                plugin.deps.append(str);
            }
        }
    }
}
