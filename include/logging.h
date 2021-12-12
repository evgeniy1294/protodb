#pragma once

#include <QtCore>

enum LogChannel {
    /// Data channels
    kFirstLogChannel,
    kSecondLogChannel,

    /// Info channels
    kCommentLogChannel,
    kErrorLogChannel,

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
