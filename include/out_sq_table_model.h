#pragma once

#include <QAbstractTableModel>
#include <QVariant>
#include <QList>

#include "outgoing_sequence.h"

class OutSqTableModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnNames {
      kColumnSqName      = 0,
      kColumnRepeatTime  = 1,
      kColumnDescription = 2,
      kColumnCharStr     = 3,
      kColumnSendBtn     = 4,

      kColumnCount
    };

public:
    OutSqTableModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& aParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& aParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const override;
    QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole) const override;
    bool setData(const QModelIndex &aIndex, const QVariant &aValue, int aRole = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &aIndex) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;



public slots:
    void onSendSequence(const QModelIndex& index);

private:
    QList<OutgoingSequence> mList;
};



