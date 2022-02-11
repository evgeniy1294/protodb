#include "Logger.h"

Logger::Logger(QObject *parent)
    : QAbstractTableModel(parent)
    , m_decorator(nullptr)
    , m_formatter(nullptr)
{
    setDecorator( new LogDecorator() );  // Перенести в LogItemDelegate
    setFormatter( new LogFormatter() );  // Перенести в LogItemDelegate

    m_flags[kFirstLogChannel]   = true;
    m_flags[kSecondLogChannel]  = true;
    m_flags[kCommentLogChannel] = true;
    m_flags[kErrorLogChannel]   = true;
}

void Logger::setFormatter(LogFormatter* formatter)
{
    if (m_formatter != formatter) {

        if (m_formatter != nullptr) {
            disconnect(m_formatter);
            delete m_formatter;
        }

        m_formatter = formatter;
        m_formatter->setParent(this);
        connect(m_formatter, &LogFormatter::sConfigChanged, this, &Logger::reload);
    }
}

LogFormatter *Logger::formatter() const
{
    return m_formatter;
}

void Logger::setDecorator(LogDecorator *decorator)
{
    if (m_decorator != decorator) {

        if (m_decorator != nullptr) {
            disconnect(m_decorator);
            delete m_decorator;
        }

        m_decorator = decorator;
        m_decorator->setParent(this);
        connect(m_decorator, &LogDecorator::sConfigChanged, this, &Logger::reload);
    }
}

LogDecorator *Logger::decorator() const
{
    return m_decorator;
}

void Logger::log(const LogEvent &event)
{
    if (m_flags[event.channel]) {
        beginInsertRows(QModelIndex(), rowCount() - 1, rowCount());
            m_log.append(event);
        endInsertRows();
    }
}

void Logger::log(LogChannel ch, const QByteArray &data)
{
    LogEvent event = {.timestamp = QDateTime::currentDateTime(), .channel = ch, .message = data};
    log(event);
}

void Logger::comment(const QByteArray &text)
{
    LogEvent event = {.timestamp = QDateTime::currentDateTime(), .channel = kCommentLogChannel, .message = text};
    log(event);
}

void Logger::error(const QByteArray& text)
{
    LogEvent event = {.timestamp = QDateTime::currentDateTime(), .channel = kErrorLogChannel, .message = text};
    log(event);
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

    if ( role == Qt::DisplayRole || role == HexDisplayRole || role == AsciiDisplayRole ) {
        auto& item = m_log.at(row);
        switch (col) {
            case kColumnTimestamp:
                return m_formatter->timestamp(item);

            case kColumnChannel: {
                return m_formatter->channelName(item);
            }

            case kColumnMsg: {
                auto format = (role == AsciiDisplayRole) ? kAsciiFormat : kHexFormat;
                return m_formatter->data(item, format);
            }

            default:
                break;
        }
    }

    if (role == ChannelRole) {
        auto& item = m_log.at(row);
        return item.channel;
    }

    if (role == EventHexDisplayRole) {
        auto& item = m_log.at(row);

        QString msg = QString("%1 %2 %3").arg(m_formatter->timestamp(item),
                                              m_formatter->channelName(item),
                                              m_formatter->data(item, kHexFormat));
        return msg;
    }

    if (role == EventAsciiDisplayRole) {
        auto& item = m_log.at(row);

        QString msg = QString("%1 %2 %3").arg(m_formatter->timestamp(item),
                                              m_formatter->channelName(item),
                                              m_formatter->data(item, kAsciiFormat));
        return msg;
    }

    if (role == AnalyzeRole) {
        auto& item = m_log.at(row);
        if (item.channel == kFirstLogChannel || item.channel == kSecondLogChannel) {
            return item.message;
        }
    }

    if (role == Qt::ForegroundRole) {
        return (col == kColumnMsg) ? m_decorator->channelColor(m_log.at(row)) :
                                     m_decorator->attributeColor();
    }

    if (role == Qt::FontRole) {
        return (col == kColumnMsg) ? m_decorator->channelFont(m_log.at(row)) :
                                     m_decorator->attributeFont();
    }

    if (role == Qt::TextAlignmentRole) {
        return (col == kColumnMsg) ? 0x81 : Qt::AlignCenter;
    }

    return QVariant();
}

QVariant Logger::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
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

bool Logger::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    return false;
}


void Logger::setChannelEnabled(LogChannel channel)
{
    m_flags[channel] = true;
}

void Logger::setChannelDisabled(LogChannel channel)
{
    m_flags[channel] = false;
}
