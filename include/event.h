#pragma once

#include <QDateTime>
#include <QByteArray>
#include <QStringList>

struct Event {
    int channel;
    QDateTime timestamp;
    QByteArray  bytes;
    QStringList userData;
};
