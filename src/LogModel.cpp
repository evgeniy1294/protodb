#include "LogFormatter.h"
#include "LogDecorator.h"
#include "LogModel.h"

LogModel::LogModel(QObject *parent)
    : QAbstractTableModel(parent)
    , m_decorator(nullptr)
    , m_formatter(nullptr)
{
    setDecorator( new LogDecorator() );
    setFormatter( new LogFormatter() );
}

void LogModel::setFormatter(LogFormatter* formatter)
{
    if (m_formatter != formatter) {

        if (m_formatter != nullptr) {
            disconnect(m_formatter);
            delete m_formatter;
        }

        m_formatter = formatter;
        m_formatter->setParent(this);
        connect(m_formatter, &LogFormatter::sConfigChanged, this, &LogModel::reload);
    }
}

LogFormatter *LogModel::formatter() const
{
    return m_formatter;
}

void LogModel::setDecorator(LogDecorator *decorator)
{
    if (m_decorator != decorator) {

        if (m_decorator != nullptr) {
            disconnect(m_decorator);
            delete m_decorator;
        }

        m_decorator = decorator;
        m_decorator->setParent(this);
        connect(m_decorator, &LogDecorator::sConfigChanged, this, &LogModel::reload);
    }
}

LogDecorator *LogModel::decorator() const
{
    return m_decorator;
}

void LogModel::log(const LogEvent &event)
{
    auto row = rowCount() - 1;

    beginInsertRows(QModelIndex(), row, row); // row, rowCount() ???
        m_log.append(event);
    endInsertRows();
}

void LogModel::log(LogChannel ch, const QByteArray &data)
{
    LogEvent event = {.timestamp = QDateTime::currentDateTime(), .channel = ch, .message = data};
    log(event);
}

void LogModel::comment(const QByteArray &text)
{
    LogEvent event = {.timestamp = QDateTime::currentDateTime(), .channel = kCommentLogChannel, .message = text};
    log(event);
}

void LogModel::error(const QByteArray& text)
{
    LogEvent event = {.timestamp = QDateTime::currentDateTime(), .channel = kErrorLogChannel, .message = text};
    log(event);
}

void LogModel::clear()
{
    beginResetModel();
        m_log.clear();
    endResetModel();
}

void LogModel::reload()
{
    beginResetModel();
    endResetModel();
}

int LogModel::rowCount(const QModelIndex& aParent) const
{
    return m_log.count();
}

int LogModel::columnCount(const QModelIndex& aParent) const
{
    return kColumnCount;
}

QVariant LogModel::data(const QModelIndex& index, int role) const
{
    auto row = index.row();
    auto col = index.column();

    if (!checkIndex(index))
        return QVariant();

    if (role == Qt::DisplayRole) {
        auto& item = m_log.at(row);

        switch (col) {
            case kColumnTimestamp:
                return m_formatter->timestamp(item);

            case kColumnChannel: {
                return m_formatter->channelName(item);
            }

            case kColumnMsg:
                return m_formatter->data(item, kHexFormat);

            default:
                break;
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

QVariant LogModel::headerData(int aSection, Qt::Orientation aOrientation, int aRole) const
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

bool LogModel::setData(const QModelIndex& aIndex, const QVariant& aValue, int aRole)
{
    return false;
}

