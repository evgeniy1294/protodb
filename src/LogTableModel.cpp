#include <iostream>
#include <QColor>

#include "Logger.h"
#include "LogItemFormatter.h"
#include "LogTableModel.h"

LogTableModel::LogTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , m_formatter(nullptr)
    , m_logger(nullptr)
{
     setFormatter( new LogItemFormatter() );
}

int LogTableModel::rowCount(const QModelIndex& aParent) const
{
    return (m_logger != nullptr) ? m_logger->count() : 0;
}

int LogTableModel::columnCount(const QModelIndex& aParent) const
{
    return kColumnCount;
}

QVariant LogTableModel::data(const QModelIndex& index, int role) const
{
    auto row = index.row();
    auto col = index.column();

    if (m_logger == nullptr)
        return QVariant();

    if (!checkIndex(index))
        return QVariant();

    if (role == Qt::DisplayRole) {
        auto& item = m_logger->at(row);

        switch (col) {
            case kColumnTimestamp:
                return m_formatter->timestamp(item);

            case kColumnChannel: {
                return m_formatter->channel(item);
            }

            case kColumnMsg:
                return m_formatter->message(item);

            default:
                break;
        }
    }

    if (role == Qt::ForegroundRole) {
        return (col == kColumnMsg) ? m_formatter->color(m_logger->at(row)) :
                                     m_formatter->attributeColor();
    }

    if (role == Qt::TextAlignmentRole) {
        return (col == kColumnMsg) ? 0x81 : Qt::AlignCenter;
    }

    return QVariant();
}

QVariant LogTableModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
{
    if (aRole == Qt::DisplayRole) {

        if (aOrientation == Qt::Horizontal) {
            switch (aSection) {
                case kColumnTimestamp: return QString(tr(""));
                case kColumnChannel: return QString(tr(""));
                case kColumnMsg: return QString(tr("Bytes"));
                default: break;
            }
        }
        else {
            return QString::number(aSection+1);
        }

    }

    return QVariant();
}

bool LogTableModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    return false;
}

bool LogTableModel::insertRows(int row, int count, const QModelIndex& parent)
{
    return false;
}

bool LogTableModel::removeRows(int row, int count, const QModelIndex& parent)
{
    return false;
}

void LogTableModel::setLogger(Logger* logger)
{
    if (m_logger != logger) {
        if (m_logger != nullptr) {
            disconnect(m_logger);
        }

        m_logger = logger;

        connect(m_logger, &Logger::appended, this, [this]() {
            auto row = rowCount() - 1;

            beginInsertRows(QModelIndex(), row, row);
            endInsertRows();
        });

        connect(m_logger, &Logger::cleared, this, [this]() {
            QModelIndex index = createIndex(0, 0);

            beginRemoveRows(index, 0, 0);
            endRemoveRows();
            emit dataChanged(index, index);
        });
    }
}

void LogTableModel::setFormatter(LogItemFormatter* a_decorator)
{
    if (m_formatter != a_decorator) {

        if (m_formatter != nullptr) {
            disconnect(m_formatter);
            delete m_formatter;
        }

        m_formatter = a_decorator;
        m_formatter->setParent(this);
        connect(m_formatter, &LogItemFormatter::changed, this, [this]() {
            emit dataChanged(index(0, 0), index(rowCount() - 1, columnCount() - 1));
        });
    }
}

Logger* LogTableModel::logger() const
{
    return m_logger;
}

LogItemFormatter* LogTableModel::formatter() const
{
    return m_formatter;
}
