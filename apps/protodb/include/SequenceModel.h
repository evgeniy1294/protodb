#pragma once

#include <protodb/Sequence.h>

#include <protodb/utils/JsonBaseUtils.h>

#include <QAbstractTableModel>
#include <QVariant>
#include <QList>


class SequenceModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnNames {
        kColumnName        = 0,
        kColumnBindedName  = 1,
        kColumnPeriod      = 2,
        kColumnDescription = 3,
        kColumnDsl         = 4,
        kColumnActiveFlag  = 5,

        kColumnCount
    };

    enum Role {
        kClickRole = Qt::UserRole,
    };

public:
    SequenceModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &aValue, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    void setIncomingMode();
    void setOutgoingMode();
    bool isModeIncoming() const;

    void toJson(nlohmann::json& json);
    void fromJson(const nlohmann::json& json);

private:
    QList<Sequence> m_sequences;
    bool m_mode;
};



