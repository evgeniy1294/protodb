#pragma once

#include <QAbstractTableModel>
#include <QList>
#include <QMap>

#include "Configurable.h"

class PluginAbstract;

class PluginManager: public QAbstractTableModel, public Configurable
{
    Q_OBJECT

public:
    PluginManager(QStringList path, QObject* parent = nullptr);

    void loadPlugins();
    bool locked() const;

    template<class T> QList<T*> get(const QString group = QString());

public:
    const QString& jsonPrefix() const override;
    void fromJson(const nlohmann::json& json) override;
    void toJson(nlohmann::json& json) const override;

public:
    enum ColumnNames {
        kColumnName        = 0,
        kColumnVersion     = 1,
        kColumnVendor      = 2,
        kColumnDescription = 3,
        kColumnLoad        = 4,

        kColumnCount
    };

    int rowCount(const QModelIndex& aParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& aParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const override;
    QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole) const override;
    bool setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole = Qt::EditRole) override;

private:
    bool m_lock;
    QList<PluginAbstract*> m_plugins;
    QMap<QString, bool>    m_load;
};
