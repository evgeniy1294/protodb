#include "Logger.h"

Logger::Logger(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_flags[kChannelFirst]   = true;
    m_flags[kChannelSecond]  = true;
    m_flags[kChannelComment] = true;
    m_flags[kChannelError]   = true;
}

void Logger::log(const Event &event)
{
    if (m_flags[event.channel]) {
        m_log.append(event);
        beginInsertRows(QModelIndex(), rowCount() - 1, rowCount() - 1);
        endInsertRows();
    }
}

Logger &Logger::instance()
{
    static Logger m_instance;
    return m_instance;
}

void Logger::log(Channel ch, const QByteArray& data, const QDateTime& timestamp)
{
    instance().log( {.timestamp = timestamp, .channel = ch, .message = data} );
}

void Logger::comment(const QByteArray &text)
{
    instance().log( { .timestamp = QDateTime::currentDateTime(), .channel = kChannelComment, .message = text } );
}

void Logger::error(const QByteArray& text)
{
    instance().log( { .timestamp = QDateTime::currentDateTime(), .channel = kChannelError, .message = text } );
}

void Logger::clear()
{
    beginResetModel();
        m_log.clear();
    endResetModel();
}

void Logger::reload()
{
    beginResetModel();
    endResetModel();
}

int Logger::rowCount(const QModelIndex& aParent) const
{
    int count = m_log.count();
    return count;
}

int Logger::columnCount(const QModelIndex& aParent) const
{
    return kColumnCount;
}

QVariant Logger::data(const QModelIndex& index, int role) const
{
    auto row = index.row();
    auto col = index.column();

    if (!checkIndex(index))
        return QVariant();

    if ( role == Qt::DisplayRole ) {
        auto& event = m_log.at(row);
        switch (col) {
            case kColumnTimestamp:
                return event.timestamp;

            case kColumnChannel: {
                return event.channel;
            }

            case kColumnMsg: {
                return event.message;
            }

            default:
                break;
        }
    }

    return QVariant();
}

QVariant Logger::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case kColumnTimestamp: return QString(tr(""));
                case kColumnChannel: return QString(tr(""));
                case kColumnMsg: return QString(tr("Bytes"));
                default: break;
            }
        }
        else {
            return QString::number(section+1);
        }

    }

    return QVariant();
}

bool Logger::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    return false;
}


void Logger::setChannelEnabled(Channel channel)
{
    instance().m_flags[channel] = true;
}

void Logger::setChannelDisabled(Channel channel)
{
    instance().m_flags[channel] = false;
}
