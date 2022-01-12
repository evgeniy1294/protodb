#pragma once

#include <QAbstractItemModel>
#include <QPluginLoader>

class PluginManagerPrivate;

class PluginManager: public QAbstractItemModel
{
    Q_OBJECT
    Q_DISABLE_COPY(PluginManager)

public:
    enum Columns {
        kColumnName = 0,
        kColumnPluginId,
        kColumnPluginEnabled,
        kColumnGroup,
        kColumnVersion,
        kColumnVendor,
        kColumnLocation,
        kColumnDescription,
        kColumnDependencies,
        kColumnLoaded,

        kColumnCount
    };

public:
    static PluginManager& instance();

    void loadPlugins();
    void unloadPlugins();

    void setDirectories(const QStringList& dirs);
    void addDirectory(const QString& dir);
    void removeDirectory(const QString& dir);
    QStringList getDirectories() const;

    void setUserDirectories(const QStringList& dirs);
    void addUserDirectory(const QString& path);
    void removeUserDirectory(const QString& path);
    QStringList getUserDirectories() const;

    void setGroupName(const QString& group, const QString& name);

    template <typename T> QList<T*> getPlugins( const QString &group = QString() );

    bool hasBrokenDependencies() const;
    bool hasConflict() const;
    QStringList getDisabledIds() const;
    void setDisabledIds(const QStringList& ids);

    QList<QPluginLoader*> getLoaders(const QString& group = QString());

    // --------- [ MODEL INTERFACE ] ---------- //
    int rowCount( const QModelIndex &parent ) const override;
    int columnCount( const QModelIndex &parent ) const override;
    QVariant headerData( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QModelIndex index( int row, int col, const QModelIndex &parent = QModelIndex() ) const override;
    Qt::ItemFlags flags( const QModelIndex &index ) const override;
    QModelIndex parent( const QModelIndex &index ) const override;
    bool hasChildren( const QModelIndex &parent = QModelIndex() ) const override;
    QModelIndex sibling( int row, int column, const QModelIndex &index ) const override;


signals:
    void sConflictDetected(QString id);
    void sBrokenDependencyDetected(QString id);

private:
    PluginManager(QObject* parent = nullptr);
    ~PluginManager();
    void mark_loaded(const QString& group, QPluginLoader* loader); // ?

private:
    Q_DECLARE_PRIVATE(PluginManager)
    PluginManagerPrivate* const d_ptr;
};

template <typename T>
QList<T*> PluginManager::getPlugins( const QString &group )
{
    QList<T*> ret;
    auto loaders = getLoaders(group);
    for(const auto &it : loaders)
    {
        auto obj = it->instance();
        if(!obj)
        {
            it->unload();  // Удалить из списка?
            continue;
        }
        auto plugin = qobject_cast<T*>(obj);
        if( plugin )
        {
            mark_loaded(group, it);
            ret.append(plugin);
        }
    }
    return ret;
}
