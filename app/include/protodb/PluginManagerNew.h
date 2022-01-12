#pragma once

#include <QAbstractItemModel>
#include <QPluginLoader>

class PluginManagerNewPrivate;

class PluginManagerNew: public QAbstractItemModel
{
    Q_OBJECT
    Q_DISABLE_COPY(PluginManagerNew)

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
        //kColFault,

        kColCount
    };

public:
    static PluginManagerNew& instance();

    void loadPlugins(QMap<QString, bool> iids);

    void setMainDirectory(const QString& dir);
    void setManualInstallDirectory(const QString& dir);

    QString mainDirectory() const;
    QString manualInstallDirectory() const;

    void setGroupName(const QString& group_id, const QString& name);

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
    PluginManagerNew(QObject* parent = nullptr);
   ~PluginManagerNew();

private:
    Q_DECLARE_PRIVATE(PluginManagerNew)
    PluginManagerNewPrivate* const d_ptr;
};
