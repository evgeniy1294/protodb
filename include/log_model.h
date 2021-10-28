#pragma once

#include <QAbstractTableModel>
#include <QVariant>
#include <QList>

#include "event.h"

class LogModel: public QAbstractTableModel {
    Q_OBJECT

public:
    enum ColumnNames {
      kColumnTimestamp = 0,
      kColumnChannel   = 1,
      kColumnBytes     = 2,
      kColumnUser      = 3,

      kColumnCount
    };

    enum DataFormat {
        kDataFormatHex   = 0,
        kDataFormatAscii = 1
    };

public:
    LogModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& aParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& aParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const override;
    QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole) const override;
    bool setData(const QModelIndex &aIndex, const QVariant &aValue, int aRole = Qt::EditRole) override;
    //Qt::ItemFlags flags(const QModelIndex &aIndex) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void append(const Event&);
    void clear();

    DataFormat dataFormat();
    void setDataFormat(DataFormat aFormat);

signals:
    void dataFormatChanget(DataFormat);

private:
    QList<Event> mLog;
    DataFormat mDataFormat;
};
