#include "Logger.h"

#include <protodb/utils/JsonBaseUtils.h>

Logger::Logger(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_flags[ChannelFirst]   = true;
    m_flags[ChannelSecond]  = true;
    m_flags[ChannelComment] = true;
    m_flags[ChannelError]   = true;
}

void Logger::log(const Event &event)
{
    if (m_flags[event.channel]) {
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
            m_log.append(event);
        endInsertRows();

        emit sEventOccuaried(event);
    }
}

void Logger::log(Channel ch, const QByteArray& data, const QDateTime& timestamp)
{
    log( {.timestamp = timestamp, .channel = ch, .message = data} );
}

void Logger::comment(const QByteArray &text)
{
    log( { .timestamp = QDateTime::currentDateTime(), .channel = ChannelComment, .message = text } );
}

void Logger::error(const QByteArray& text)
{
    log( { .timestamp = QDateTime::currentDateTime(), .channel = ChannelError, .message = text } );
}

void Logger::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_log.size() - 1);
        m_log.clear();
    endRemoveRows();
}

void Logger::reload()
{
    beginResetModel();
    endResetModel();
}

int Logger::rowCount(const QModelIndex& aParent) const
{
    return m_log.count();
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

    auto event = m_log.at(row);
    if ( role == Qt::DisplayRole ) {
        switch (col) {
            case ColumnTimestamp:
                return event.timestamp;

            case ColumnChannel: {
                return event.channel;
            }

            case ColumnMsg: {
                return event.message;
            }

            default:
                break;
        }
    }

    if ( role == EventRole ) {
        return QVariant::fromValue(event);
    }

    return QVariant();
}

QVariant Logger::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {

        if (orientation == Qt::Horizontal) {
            switch (section) {
                case ColumnTimestamp: return QString(tr(""));
                case ColumnChannel: return QString(tr(""));
                case ColumnMsg: return QString(tr("Bytes"));
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
    m_flags[channel] = true;
}

void Logger::setChannelDisabled(Channel channel)
{
    m_flags[channel] = false;
}

void Logger::toJson(nlohmann::json& json) const
{
    json = nlohmann::json::array();

    for (const auto& event: qAsConst(m_log)) {
        nlohmann::json fields;
            fields["timestamp"] = event.timestamp.toString("dd.MM.yyyy hh:mm:ss.zzz");
            fields["channel"]   = static_cast<int>(event.channel);
            fields["data"]      = event.message;

        json.push_back(fields);
    }
}

void Logger::fromJson(const nlohmann::json& json)
{
    QList<Event> imported;
    if ( !json.is_array() )
        return;

    for (const auto& it: json) {
        if (!it.is_object())
            continue;

        Event e;
            e.timestamp = QDateTime::fromString( it.value("timestamp", QString()), QString("dd.MM.yyyy hh:mm:ss.zzz") );
            e.channel   = static_cast<Channel>( it.value("channel", static_cast<int>(Channel::ChannelError)) );
            e.message   = it.value("data", QByteArray());

        imported.append(e);
    }

    if (imported.size() > 0) {
        std::sort(imported.begin(), imported.end(), [](Event& a, Event& b) {
            return a.timestamp < b.timestamp;
        });

        beginResetModel();
            m_log.clear();
            m_log.append(imported);
        endResetModel();
    }
}
