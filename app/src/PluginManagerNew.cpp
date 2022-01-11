#include <protodb/PluginManagerNew.h>

#include <QApplication>
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
        QString fault;

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

    void loadPlugins();
    void searchPlugins();
    void enablePlugin (const QPair<int, int>& index);
    void disablePlugin(const QPair<int, int>& index);

    bool isEnabled(const PluginInfo& plugin) const;
    bool isDisabled(const PluginInfo& plugin) const;
    bool isFault(const PluginInfo& plugin) const;
    bool isLoaded(const PluginInfo& plugin) const;

private:
    Q_DECLARE_PUBLIC(PluginManagerNew);
    PluginManagerNew* q_ptr;

private:
    QPair<int /*group*/, int /*plugin*/> findPluginByIid(const QString& iid) const;
    QStringList findConflicts(const PluginInfo& plugin, bool pedantic = false) const;
    QStringList findBrokenDependencies(const PluginInfo& plugin) const;
    void loadPlugin(PluginInfo& plugin);

    QStringList m_disabled_ids;
    QString m_main_directory;
    QString m_manual_install_directory;
    QList<Group> m_groups;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(PluginManagerNewPrivate::State)


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

void PluginManagerNewPrivate::enablePlugin(const QPair<int, int> &index)
{
    Q_Q(PluginManagerNew);

    auto [grp, idx] = index;
    auto plugin = m_groups[grp].plugins.at(idx);

    if (testFlag(State_Enabled)) {
        return;
    }


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

QStringList PluginManagerNewPrivate::findConflicts(const PluginInfo& plugin, bool pedantic) const
{
    Q_Q(const PluginManagerNew);
    QStringList ret;

    if (isEnabled(plugin))
        return ret;

    for( int i = 0; i < m_groups.size(); i++ )
    {
        auto &group = m_groups[i];

        for( int j = 0; j < group.plugins.size(); j++ )
        {
            auto& j_plugin = group.plugins.at(j);

            if (plugin.file == j_plugin.file) {
                continue;
            }

            if (isEnabled(j_plugin) && j_plugin.iid == plugin.iid) {
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
        return ret << QObject::tr("Invalid dependency list (%1)").arg(plugin.name);
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

void PluginManagerNewPrivate::loadPlugin(PluginInfo &plugin)
{
    Q_Q(PluginManagerNew);

    if (isDisabled(plugin))
        return;

    if (!findConflicts(plugin, false).empty()) {
        plugin.state |= State_Fault;
        plugin.fault = QObject::tr("Has conflict");

        return;
    }

    if (plugin.deps.contains(plugin.iid)) {
        plugin.state |= State_Fault;
        plugin.fault = QObject::tr("Invalid dependencies");

        return;
    }

    for (auto& iid: plugin.deps) {
        auto [grp, idx] = findPluginByIid(iid);

        if (grp >= 0) {
            auto dependency = m_groups[grp].plugins.at(idx);
            loadPlugin(dependency);

            if (isLoaded(dependency)) {
                if (!dependency.relations.contains(plugin.iid)) {
                    dependency.relations << plugin.iid;
                }
                continue;
            }
        }

        plugin.state |= State_Fault;
        plugin.fault = QObject::tr("Broken dependencies") + iid;

        return;
    }

    if (!plugin.loader->load()) {
        plugin.state = State_Fault;
        plugin.fault = QObject::tr("Load failed");
    }

    return;
}

