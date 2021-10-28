#include <QFont>
#include <QBrush>
#include <QPointer>
#include <QDebug>

#include "out_sq_table_model.h"
#include <iostream>

OutSqTableModel::OutSqTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{

}


int OutSqTableModel::rowCount(const QModelIndex& aParent) const
{
    return mList.size();
}

int OutSqTableModel::columnCount(const QModelIndex& aParent) const
{
    return kColumnCount;
}


QVariant OutSqTableModel::data(const QModelIndex& aIndex, int aRole) const
{
    auto row = aIndex.row();
    auto col = aIndex.column();


    if (checkIndex(aIndex)) {
        auto& sq = mList.at(row);

        if (aRole == Qt::DisplayRole) {
            switch (col) {
                case kColumnSqName:
                    return sq.name();

                case kColumnRepeatTime:
                    return (sq.repeatPeriod() == 0) ?
                                QString(tr("No Repeat")) :
                                QString("%1ms").arg(sq.repeatPeriod());

                case kColumnDescription:
                    return sq.description();

                case kColumnCharStr:
                    return sq.dslString();

                case kColumnSendBtn:
                    return QString("B");
            }
        }

        if (aRole == Qt::EditRole) {
            switch (col) {
                case kColumnSqName:
                    return sq.name();

                case kColumnRepeatTime:
                    return sq.repeatPeriod();

                case kColumnDescription:
                    return sq.description();

                case kColumnCharStr:
                    return sq.dslString();

                case kColumnSendBtn:
                    return QString("B"); // "Send" Button
            }
        }
    }


   return QVariant();
}

QVariant OutSqTableModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
{
    if (aRole == Qt::DisplayRole) {

        if (aOrientation == Qt::Horizontal) {
            switch (aSection) {
                case kColumnSqName: return QString(tr("Name"));
                case kColumnRepeatTime: return QString(tr("Repeat"));
                case kColumnDescription: return QString(tr("Description"));
                case kColumnCharStr: return QString(tr("Sequence"));
                case kColumnSendBtn: return QString("");
                default: break;
            }
        }
        else {
            return QString::number(aSection+1);
        }

    }

    return QVariant();
}


bool OutSqTableModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    bool ret = false;

    int row = aIndex.row();
    int col = aIndex.column();

    if ( checkIndex(aIndex) )
    {
        auto& sq = mList[row];

        if (aRole == Qt::EditRole)
        {
            switch (col) {
                case kColumnSqName:
                    sq.setName(aValue.toString());
                    break;
                case kColumnRepeatTime:
                    sq.setRepeatPeriod(aValue.toInt());
                    break;
                case kColumnDescription:
                    sq.setDescription(aValue.toString());
                    break;
                case kColumnCharStr:
                    sq.setDslString(aValue.toString());
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

Qt::ItemFlags OutSqTableModel::flags(const QModelIndex& aIndex) const
{
    int row[[maybe_unused]] = aIndex.row();
    int col = aIndex.column();

    Qt::ItemFlags flags = QAbstractTableModel::flags(aIndex);

    if (col != kColumnSendBtn && col != kColumnSqName) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool OutSqTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    OutgoingSequence sq;
    beginInsertRows(QModelIndex(), row, row + count - 1);

    sq.setName(tr("Sequence %1").arg(mList.size()+1));
    mList.insert(row, sq);

    endInsertRows();


    return true;
}

bool OutSqTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row >= 0 && row < mList.size()) {
      count = ((row + count) > mList.size()) ? mList.size() - row : count;
      beginRemoveRows(QModelIndex(), row, row + count - 1);

      if (row == 0 && count == mList.size()) {
        mList.clear();
      }
      else
      {
        for (int i = 0; i < count; i++) {
          mList.removeAt(row);
        }

      }

      endRemoveRows();
    }

    return true;
}


void OutSqTableModel::onSendSequence(const QModelIndex& index)
{
  std::cout << "Send sequence: " << index.row() << std::endl;
  // if ( checkIndex(aIndex) ) { emit sendSequence(mList.at(row()); }
}


