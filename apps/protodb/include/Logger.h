#pragma once

#include <protodb/configurable/Configurable.h>

#include <QAbstractTableModel>
#include <QDateTime>

namespace protodb {

class Logger: public QAbstractTableModel
{
    Q_OBJECT
    Q_DISABLE_COPY(Logger)

public:
    enum ColumnNames {
        ColumnTimestamp = 0,
        ColumnChannel   = 1,
        ColumnMsg       = 2,

        kColumnCount
    };

    enum Channel {
        ChannelFirst = 0,
        ChannelSecond,
        ChannelComment,
        ChannelError,

        kLogChannelsNum
    };

    struct Event
    {
        QDateTime timestamp;
        Channel channel;
        QByteArray message;
    };

    static constexpr int EventRole = Qt::UserRole;
public:
    Logger(QObject* parent = nullptr);

    void setChannelEnabled (Channel channel);
    void setChannelDisabled(Channel channel);

    void toJson(nlohmann::json& json) const;
    void fromJson(const nlohmann::json& json);

    // ---------[ MODEL INTERFACE ]----------- //
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

signals:
    void sEventOccuaried(Logger::Event event);

public slots:
    void clear();
    void reload();

    void log(Logger::Channel ch, const QByteArray& data, const QDateTime& timestamp = QDateTime::currentDateTime());
    void comment(const QByteArray& text);
    void comment(const QString& text);
    void error(const QByteArray& text);
    void error(const QString& text);

private:
    void log(const Event& event);

private:
    QList<Event> m_log;
    QMap<Channel, bool> m_flags;
};

} // namespace protodb

Q_DECLARE_METATYPE(protodb::Logger::Event)
