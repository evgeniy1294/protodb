#pragma once

#include <QtCore>

enum LogChannel {
    kFirstLogChannel,
    kSecondLogChannel,
    kCommentLogChannel,

    kLogChannelsNum
};


struct LogItem
{
    QDateTime  timestamp;
    LogChannel channel;
    QByteArray message;
};
