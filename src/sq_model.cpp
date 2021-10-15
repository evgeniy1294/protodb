#include <QFont>
#include <QBrush>
#include <QPointer>
#include <QDebug>

#include "sq_model.h"


SqModel::SqModel(QObject* parent)
    : QAbstractTableModel(parent)
    , mStorage(nullptr)
{

}

void SqModel::setStorage(const QPointer<SqStorage>& aStorage)
{
    if (mStorage) {
      disconnect(mStorage);
    }

    mStorage = aStorage;
      connect(mStorage, &SqStorage::sSeqAppended, this, &SqModel::addSequence);
      connect(mStorage, &SqStorage::sSeqRemoved, this, &SqModel::removeSequence);
      connect(mStorage, &SqStorage::sCleared, this, &SqModel::clearStorage);

  beginInsertRows(QModelIndex(), 0, mStorage->size());
  endInsertRows();
}

int SqModel::rowCount(const QModelIndex& aParent) const
{
  return /*(!mStorage.isNull()) ? 0 :*/ mStorage->size();
}

int SqModel::columnCount(const QModelIndex& aParent) const
{
  return 4;
}


QVariant SqModel::data(const QModelIndex& aIndex, int aRole) const
{
  auto row = aIndex.row();
  auto col = aIndex.column();


  if (aRole == Qt::DisplayRole) {
    auto sq = mStorage->getSequence(row);

    switch (col) {
      case kColumnSqName:
          return sq->name();

      case kColumnTrigName:
          return sq->triggerName();

      case kColumnRepeatTime:
          return QString("100ms");

      case kColumnSendBtn:
          return QString("B"); // "Send" Button
    }
  }


  return QVariant();
}

QVariant SqModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
{
    if (aRole == Qt::DisplayRole && aOrientation == Qt::Horizontal) {
        switch (aSection) {
            case kColumnSqName:
                return QString(tr("Name"));
            case kColumnTrigName:
                return QString(tr("Triggered"));
            case kColumnRepeatTime:
                return QString(tr("Repeat"));
            case kColumnSendBtn:
                return QString(""); // "Send" Button
        }
    }

    return QVariant();
}

bool SqModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    bool ret = false;

    int row = aIndex.row();
    int col = aIndex.column();


    if ( checkIndex(aIndex) )
    {
        auto sq = mStorage->getSequence(row);

        if (aRole == Qt::EditRole)
        {
            if (col == kColumnSqName) {
              sq->setName(aValue.toString());
            }
            else if (col == kColumnTrigName) {
              sq->setTriggerName(aValue.toString());
            }

            ret = (col == kColumnSqName || col == kColumnTrigName);
        }
    }

    return ret;
}



Qt::ItemFlags SqModel::flags(const QModelIndex& aIndex) const
{
    int row[[maybe_unused]] = aIndex.row();
    int col = aIndex.column();

    Qt::ItemFlags flags = QAbstractTableModel::flags(aIndex);

    if (col == kColumnSqName || col == kColumnTrigName) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

void SqModel::addSequence(const QUuid& aUuid, int aIndex)
{
  beginInsertRows(QModelIndex(), aIndex, aIndex);
  endInsertRows();
}

void SqModel::removeSequence(const QUuid& aUuid, int aIndex)
{

}

void SqModel::clearStorage()
{

}


