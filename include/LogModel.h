#pragma once

#include <QAbstractTableModel>
#include "logging.h"
#include "Configurable.h"

class LogFormatter;
class LogDecorator;

class LogModel: public QAbstractTableModel
{
    Q_OBJECT

public:
    enum ColumnNames {
        kColumnTimestamp = 0,
        kColumnChannel   = 1,
        kColumnMsg       = 2,

        kColumnCount
    };

public:
    LogModel(QObject* parent = nullptr);

    // ---------[ MODEL INTERFACE ]----------- //
    int rowCount(const QModelIndex& aParent = QModelIndex()) const override;
    int columnCount(const QModelIndex& aParent = QModelIndex()) const override;
    QVariant data(const QModelIndex& aIndex, int aRole = Qt::DisplayRole) const override;
    QVariant headerData(int aSection, Qt::Orientation aOrientation, int aRole) const override;
    bool setData(const QModelIndex &aIndex, const QVariant &aValue, int aRole = Qt::EditRole) override;

    void setFormatter(LogFormatter* formatter);
    LogFormatter* formatter() const;

    void setDecorator(LogDecorator* decorator);
    LogDecorator* decorator() const;

    void log(const LogEvent& event);
    void log(LogChannel ch, const QByteArray& data);
    void comment(const QByteArray& text);

signals:
    void sPrint(const QString& text);

public slots:
    void clear();
    void reload();

private:
    LogFormatter* m_formatter;
    LogDecorator* m_decorator;
    QList<LogEvent> m_log;
};


