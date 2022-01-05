#pragma once

#include <QAbstractItemModel>
#include <QPluginLoader>

class PluginManager: public QAbstractItemModel
{
    Q_OBJECT

public:
    enum ColumnNames {
        kColumnName         = 0,
        kColumnLoad         = 1,
        kColumnVersion      = 2,
        kColumnVendor       = 3,
        kColumnLocation     = 4,
        kColumnGroup        = 5,
        kColumnDescription  = 6,
        kCOlumnDependencies = 7,

        kColumnCount
    };
/*
public:
    static PluginManager& instance();

    void addDirectory(const QString& dir);
    void removeDirectory(const QString& dir);
    QStringList getDirectories() const;

    // ???
    void addGroup(const QString& group);
    void removeGroup(const QString& group);
    QStringList getGroups() const;
    // ???

    bool hasConflict() const;
    QList<QPluginLoader> getLoaders(const QString& group = QString());

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
    Q_DISABLE_COPY(PluginManager)

    PluginManager(QObject* parent = nullptr);
    ~PluginManager();

private:
    QStringList m_directories;*/
};
