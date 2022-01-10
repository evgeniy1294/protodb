#include <protodb/PluginManagerNew.h>

#include <QApplication>
#include <QDir>
#include <QJsonArray>

class PluginManagerNewPrivate
{
public:
    enum PluginState: int {
        Enabled,
        Disabled,
        Loaded,
        Fault,
    };

public:
    PluginManagerNewPrivate();
   ~PluginManagerNewPrivate() = default;

    void loadPlugins();
    void searchPlugins();
    bool enablePlugin(const QString& iid);
    bool disablePlugin(const QString& iid);


private:
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
        QString fault;

        QPluginLoader* loader;
    };

    struct Group {
        QString id;
        QString name;
        QList<PluginInfo> plugins;
    };

    struct Index { int grp; int idx; };

private:
    Q_DECLARE_PUBLIC(PluginManagerNew);
    PluginManagerNew* q_ptr;

private:
    PluginInfo& getPluginByIid(const QString& iid) const;
    PluginInfo& getPluginByName(const QString& name) const;
    void checkPluginConflicts(PluginInfo& plugin, int grp, int idx);
    void checkPluginDependencies(PluginInfo& plugin, int grp, int idx);

    QStringList m_disabled_ids;
    QString m_main_directory;
    QString m_manual_install_directory;
    QList<Group> m_groups;
};


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
            plugin.state       = user_data["DisabledByDefault"].toBool(false) ? Disabled : Enabled;
            plugin.state       = m_disabled_ids.contains(plugin.iid) ? Disabled : plugin.state;

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

void PluginManagerNewPrivate::checkPluginConflicts(PluginInfo &plugin, int grp, int idx)
{
    Q_Q(PluginManagerNew);
    static const QString fault_str("Conflict with ");

    if (plugin.state != Enabled)
        return;

    for( int i = 0; i < m_groups.size(); i++ )
    {
        auto &group = m_groups[i];

        for( int j = 0; j < group.plugins.size(); j++ )
        {
            if (i == grp && j == idx)
                continue;

            auto &j_plugin = group.plugins[j];

            if (j_plugin.state == Enabled) {

                if (j_plugin.iid == plugin.iid)
                {
                    // Возможно, имеет смысл включать плагин из main, а из manual ставить Fault
                    plugin.state   = Fault;
                    j_plugin.state = Fault;

                    plugin.fault = fault_str + j_plugin.file;
                    j_plugin.fault = fault_str + plugin.file;

                    auto parent = q->index( i, 0 );
                    emit q->dataChanged( q->index(j, 0, parent ),
                                         q->index(j, PluginManagerNew::kColCount-1, parent ),
                                         {Qt::BackgroundRole});

                    return;
                }
            }
        }
    }

    return;
}

void PluginManagerNewPrivate::checkPluginDependencies(PluginInfo &plugin, int grp, int idx)
{
    Q_Q(PluginManagerNew);
    static const QString fault_str("Broken dependencies: ");

    if (plugin.state != Enabled)
        return;

    for (auto& dep: plugin.deps) {

    }

    return;
}

