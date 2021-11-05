#pragma once

#include <QObject>
#include <QDateTime>

class Logger: public QObject {
    Q_OBJECT

public:
    enum Channels {
        kFirstChannel,
        kSecondChannel,
        kCommentChannel,

        kChannelsNum
    };
    Q_ENUM(Channels)

    struct LogItem
    {
        QDateTime  timestamp;
        Channels   channel;
        QByteArray message;
    };

public:
    Logger(QObject* parent = nullptr);

    void print(Channels channel, const QByteArray& msg);
    void comment(const QByteArray& msg);
    void comment(const char* str);
    void clear();

    bool empty() const;
    int count() const;
    const LogItem& back() const;
    const LogItem& at(int i) const;

signals:
    void appended();
    void cleared();

private:
    QList<LogItem> m_log;
};
