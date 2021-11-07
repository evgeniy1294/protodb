#pragma once

#include <QAbstractTableModel>

class Logger;
class LogItemFormatter;

class LogTableModel: public QAbstractTableModel {
    Q_OBJECT
    friend class Logger;

public:
    enum ColumnNames {
        kColumnTimestamp = 0,
        kColumnChannel   = 1,
        kColumnMsg       = 2,

        kColumnCount
    };

public:
    LogTableModel(QObject* parent = nullptr);

    // ---------[ MODEL INTERFACE ]----------- //
    int rowCount(const QModelIndex& aParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& aParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const override;
    QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole) const override;
    bool setData(const QModelIndex &aIndex, const QVariant &aValue, int aRole = Qt::EditRole) override;

    void setLogger(Logger* logger);
    void setFormatter(LogItemFormatter* decorator);

    Logger* logger() const;
    LogItemFormatter* formatter() const;

private:
    Logger* m_logger;
    LogItemFormatter* m_formatter;
};

