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
        kColName = 0,
        kColEnabled,
        kColGroup,
        kColIid,
        kColVersion,
        kColVendor,
        kColDescription,
        kColFile,

        kColCount
    };

public:
    static PluginManager& instance();

    void loadPlugins(QMap<QString, bool> iids);
    QMap<QString, bool> getPluginsState() const;

    void setMainDirectory(const QString& dir);
    void setManualInstallDirectory(const QString& dir);

    QString mainDirectory() const;
    QString manualInstallDirectory() const;

    void setGroupName(const QString& group_id, const QString& name);

    template <typename T> QList<T*> getPlugins( const QString &group = QString() );

    QList<QPluginLoader*> getLoaders(const QString& group = QString());

    QString lastError() const;
    void clearError();

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

private:
    PluginManager(QObject* parent = nullptr);
   ~PluginManager();

    void mark_fault(const QString& group, QPluginLoader* loader);
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
            it->unload();
            mark_fault(group, it);

            continue;
        }

        auto plugin = qobject_cast<T*>(obj);
        if( plugin )
        {
            ret.append(plugin);
        }
    }

    return ret;
}
