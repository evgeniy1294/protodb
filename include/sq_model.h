#pragma once

#include <QAbstractTableModel>
#include <QVariant>
#include <QPointer>

#include "sequence_storage.h"


class SqModel: public QAbstractTableModel
{
    Q_OBJECT

    enum ColumnNames {
      kColumnSqName     = 0,
      kColumnTrigName   = 1,
      kColumnRepeatTime = 2,
      kColumnSendBtn    = 3
    };

public:
    SqModel(QObject* parent = nullptr);

    void setStorage(const QPointer<SqStorage>& aStorage);

    int rowCount(const QModelIndex& aParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& aParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const override;
    QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole) const override;
    bool setData(const QModelIndex &aIndex, const QVariant &aValue, int aRole = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &aIndex) const override;

signals:


public slots:
    void addSequence(const QUuid& aUuid, int aIndex);
    void removeSequence(const QUuid& aUuid, int aIndex);
    void clearStorage();

private:
    QPointer<SqStorage> mStorage;
};



