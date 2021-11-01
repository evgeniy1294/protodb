#include <QFont>
#include <QBrush>
#include <QPointer>
#include <QDebug>

#include "SequenceTableModel.h"
#include <iostream>

SequenceTableModel::SequenceTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_mode(DisplayMode::kOutgoingDisplayMode)
{

}


int SequenceTableModel::rowCount(const QModelIndex& parent) const
{
    return m_list.size();
}

int SequenceTableModel::columnCount(const QModelIndex& parent) const
{
    return kColumnCount;
}


QVariant SequenceTableModel::data(const QModelIndex& index, int role) const
{
    auto row = index.row();
    auto col = index.column();


    if (checkIndex(index)) {
        auto& sq = m_list.at(row);

        if (role == Qt::DisplayRole) {
            switch (col) {
                case kColumnName:
                    return sq.name();

                case kColumnBindedName:
                    return sq.bindedName();

                case kColumnPeriod: {
                    quint32 period  = sq.period();
                    QString special = (m_mode == kIncomingDisplayMode) ?
                                            tr("No Delay") : tr("No Repeat");

                    return (sq.period() == 0) ? special : QString("%1ms").arg(period);
                }

                case kColumnDescription:
                    return sq.description();

                case kColumnDsl:
                    return sq.dslString();

                case kColumnActiveFlag:
                    return sq.active();
            }
        }

        if (role == Qt::EditRole) {
            switch (col) {
                case kColumnName:
                    return sq.name();

                case kColumnBindedName:
                    return sq.bindedName();

                case kColumnPeriod:
                    return sq.period();

                case kColumnDescription:
                    return sq.description();

                case kColumnDsl:
                    return sq.dslString();

                case kColumnActiveFlag:
                    return sq.active();
            }
        }
    }


   return QVariant();
}

QVariant SequenceTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case kColumnName: return QString(tr("Name"));
                case kColumnBindedName: return (m_mode == kIncomingDisplayMode) ? QString(tr("Triggered")) : QString();
                case kColumnPeriod: return (m_mode == kIncomingDisplayMode) ? QString(tr("Delay")) : QString(tr("Repeat"));
                case kColumnDescription: return QString(tr("Description"));
                case kColumnDsl: return QString(tr("Sequence"));
                case kColumnActiveFlag: return (m_mode == kIncomingDisplayMode) ? QString(tr("Use")): QString("");
                default: break;
            }
        }
        else {
            return QString::number(section+1);
        }

    }

    return QVariant();
}


bool SequenceTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    bool ret = false;

    int row = index.row();
    int col = index.column();

    if ( checkIndex(index) )
    {
        ret = true;
        auto& sq = m_list[row];

        if (role == Qt::EditRole)
        {
            switch (col) {
                case kColumnName:
                    sq.setName(value.toString());
                    break;

                case kColumnBindedName:
                    sq.setBindedName(value.toString());
                    break;

                case kColumnPeriod:
                    sq.setPeriod(value.toInt());
                    break;
                case kColumnDescription:
                    sq.setDescription(value.toString());
                    break;
                case kColumnDsl:
                    sq.setDslString(value.toString());
                    break;
                case kColumnActiveFlag:
                    sq.setActive( value.toBool() );
                default:
                    break;
            }

            emit dataChanged(index, index);
        }

        if (role == kClickRole) {
            if (col == kColumnActiveFlag) {
                sq.setActive( !sq.active() );
            }
            emit dataChanged(index, index);
        }
    }

    return ret;
}

Qt::ItemFlags SequenceTableModel::flags(const QModelIndex& index) const
{
    int row[[maybe_unused]] = index.row();
    int col = index.column();

    Qt::ItemFlags flags = QAbstractTableModel::flags(index);

    if (col != kColumnName) {
        flags |= Qt::ItemIsEditable;
    }

    return flags;
}

bool SequenceTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    Sequence sq;
    beginInsertRows(QModelIndex(), row, row + count - 1);

    sq.setName(tr("Sequence %1").arg(m_list.size()+1));
    m_list.insert(row, sq);

    endInsertRows();


    return true;
}

bool SequenceTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (row >= 0 && row < m_list.size()) {
      count = ((row + count) > m_list.size()) ? m_list.size() - row : count;
      beginRemoveRows(QModelIndex(), row, row + count - 1);

      if (row == 0 && count == m_list.size()) {
        m_list.clear();
      }
      else
      {
        for (int i = 0; i < count; i++) {
          m_list.removeAt(row);
        }

      }

      endRemoveRows();
    }

    return true;
}



