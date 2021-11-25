#pragma once

#include <QtCore>

enum LogChannel {
    kFirstLogChannel,
    kSecondLogChannel,
    kCommentLogChannel,

    kLogChannelsNum
};

enum ByteFormat {
    kHexFormat,
    kAsciiFormat,
};

struct LogEvent
{
    QDateTime  timestamp;
    LogChannel channel;
    QByteArray message;
};
