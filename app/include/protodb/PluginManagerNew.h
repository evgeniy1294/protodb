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
        kColGroup,
        kColIid,
        kColVersion,
        kColVendor,
        kColDescription,
        kColFile,
        kColFault,

        kColCount
    };

public:
    static PluginManagerNew& instance();

    void setMainDirectory(const QString& dir);
    void setManualInstallDirectory(const QString& dir);

    QString mainDirectory() const;
    QString manualInstallDirectory() const;

private:
    PluginManagerNew(QObject* parent = nullptr);
   ~PluginManagerNew();

private:
    Q_DECLARE_PRIVATE(PluginManagerNew)
    PluginManagerNewPrivate* const d_ptr;
};
