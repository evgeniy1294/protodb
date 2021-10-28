#include <QColor>
#include "log_model.h"

LogModel::LogModel(QObject* parent)
    : QAbstractTableModel(parent)
{

}

int LogModel::rowCount(const QModelIndex& aParent) const
{
    return mLog.count();
}

int LogModel::columnCount(const QModelIndex& aParent) const
{
    return kColumnCount;
}

QVariant LogModel::data(const QModelIndex& aIndex, int aRole) const
{
    auto row = aIndex.row();
    auto col = aIndex.column();


    if (checkIndex(aIndex)) {
        auto& event = mLog.at(row);

        if (aRole == Qt::DisplayRole) {
            switch (col) {
                case kColumnTimestamp:
                    return event.timestamp.time().toString();

                case kColumnChannel:
                    return (event.channel == 0) ? QString(">>>") : QString("<<<");

                case kColumnBytes:
                    return event.bytes.toHex(' ');

                case kColumnUser:
                    return event.userData;
                default:
                    break;
            }
        }

        if (aRole == Qt::ForegroundRole) {
            switch (col) {
                case kColumnTimestamp:
                    return QColor(Qt::darkGreen);

                case kColumnChannel:
                    return QColor(Qt::darkGreen);

                case kColumnBytes:
                    return (event.channel == 0) ? QColor(Qt::red) : QColor(Qt::blue);

                case kColumnUser:
                    return QColor(Qt::darkYellow);
                default:
                    break;
            }
        }

        if (aRole == Qt::TextAlignmentRole) {
            return (col == kColumnBytes) ? 0x81 : Qt::AlignCenter;
        }
    }


   return QVariant();
}

QVariant LogModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
{
    if (aRole == Qt::DisplayRole) {

        if (aOrientation == Qt::Horizontal) {
            switch (aSection) {
                case kColumnTimestamp: return QString(tr(""));
                case kColumnChannel: return QString(tr(""));
                case kColumnBytes: return QString(tr("Bytes"));
                case kColumnUser: return QString(tr("User Comment"));
                default: break;
            }
        }
        else {
            return QString::number(aSection+1);
        }

    }

    return QVariant();
}

bool LogModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    return false;
}
/*
Qt::ItemFlags LogModel::flags(const QModelIndex& aIndex) const
{
    return QAbstractTableModel::flags(aIndex);
}*/

bool LogModel::insertRows(int row, int count, const QModelIndex& parent)
{
    return false;
}

bool LogModel::removeRows(int row, int count, const QModelIndex& parent)
{
    return false;
}

void LogModel::append(const Event& event)
{
    beginInsertRows(QModelIndex(), mLog.size(), 1);
        mLog.append(event);
    endInsertRows();
}

void LogModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, mLog.size() - 1);
        mLog.clear();
    endRemoveRows();
}
