#include <QFont>
#include <QBrush>
#include <QPointer>
#include <QDebug>

#include "sq_model.h"
#include <iostream>

SqModel::SqModel(QObject* parent)
    : QAbstractTableModel(parent)
    , mStorage(nullptr)
{

}

void SqModel::setStorage(const QPointer<SqStorage>& aStorage)
{
    if (aStorage) {
        if (mStorage) {
            disconnect(mStorage);
        }

        mStorage = aStorage;
          connect(mStorage, &SqStorage::sSeqAppended, this, [this](const QUuid& aUuid, int aIndex) {
              beginInsertRows(QModelIndex(), aIndex, aIndex);
              endInsertRows();
          });

          connect(mStorage, &SqStorage::sSeqRemoved, this, [this](const QUuid& aUuid, int aIndex) {
              beginRemoveRows(QModelIndex(), aIndex, aIndex);
              endRemoveRows();
          });

          connect(mStorage, &SqStorage::sCleared, this, [this]() {
              beginResetModel();
              endResetModel();
          });

        beginInsertRows(QModelIndex(), 0, mStorage->size());
        endInsertRows();
    }
}

int SqModel::rowCount(const QModelIndex& aParent) const
{
    return (mStorage) ? mStorage->size() : 0;
}

int SqModel::columnCount(const QModelIndex& aParent) const
{
    return kColumnCount;
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
                return (sq->repeatPeriod() == 0) ? QString(tr("No Repeat")) :
                                                   QString("%1ms").arg(sq->repeatPeriod());

            case kColumnDescription:
                return sq->description();

            case kColumnCharStr:
                return sq->charString();

            case kColumnSendBtn:
                return QString("B"); // "Send" Button
        }
    }

    if (aRole == Qt::EditRole) {
        auto sq = mStorage->getSequence(row);

        switch (col) {
            case kColumnSqName:
                return sq->name();

            case kColumnTrigName:
                return sq->triggerName();

            case kColumnRepeatTime:
                return sq->repeatPeriod();

            case kColumnDescription:
                return sq->description();

            case kColumnCharStr:
                return sq->charString();

            case kColumnSendBtn:
                return QString("B"); // "Send" Button
        }
    }


   return QVariant();
}

QVariant SqModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
{
    if (aRole == Qt::DisplayRole) {

        if (aOrientation == Qt::Horizontal) {
            switch (aSection) {
                case kColumnSqName: return QString(tr("Name"));
                case kColumnTrigName: return QString(tr("Triggered"));
                case kColumnRepeatTime: return QString(tr("Repeat"));
                case kColumnDescription: return QString(tr("Description"));
                case kColumnCharStr: return QString(tr("Sequence"));
                case kColumnSendBtn: return QString(""); // "Send" Button
                default: break;
            }
        }
        else {
            return QString::number(aSection+1);
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
            switch (col) {
                case kColumnSqName:
                    sq->setName(aValue.toString());
                    break;
                case kColumnTrigName:
                    sq->setTriggerName(aValue.toString());
                    break;
                case kColumnRepeatTime:
                    sq->setRepeatPeriod(aValue.toInt());
                    break;
                case kColumnDescription:
                    sq->setDescription(aValue.toString());
                    break;
                case kColumnCharStr:
                    sq->setCharString(aValue.toString());
                    break;
                default:
                    break;
            }

            ret = (col != kColumnSendBtn);
            if (ret) {
               emit dataChanged(aIndex, aIndex);
            }
        }
    }

    return ret;
}

Qt::ItemFlags SqModel::flags(const QModelIndex& aIndex) const
{
    int row[[maybe_unused]] = aIndex.row();
    int col = aIndex.column();

    Qt::ItemFlags flags = QAbstractTableModel::flags(aIndex);

    if (col != kColumnSendBtn) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool SqModel::insertRows(int row, int count, const QModelIndex& parent)
{
    Sequence sq;
    sq.setName(tr("Sequence %1").arg(row+1));
    sq.setDescription(tr("Document me!!!"));
    mStorage->insert(row, sq);

    return true;
}

bool SqModel::removeRows(int row, int count, const QModelIndex &parent)
{
    while(count--) {
        mStorage->remove(row);
    }

    return true;
}


void SqModel::onSendSequence(const QModelIndex& index)
{
  std::cout << "Send sequence: " << index.row() << std::endl;
}


