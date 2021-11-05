#include "Logger.h"

#include <iostream>

Logger::Logger(QObject* parent)
    : QObject(parent)
{
}

void Logger::print(Channels channel, const QByteArray& msg)
{
    m_log.append({.timestamp = QDateTime::currentDateTime(), .channel = channel, .message = msg});
    emit appended();
}

void Logger::clear()
{
    m_log.clear();
    emit cleared();
}

bool Logger::empty() const
{
    return m_log.empty();
}

int Logger::count() const
{
    return m_log.count();
}

const Logger::LogItem& Logger::back() const
{
    return m_log.back();
}

const Logger::LogItem& Logger::at(int i) const
{
    static const LogItem default_event = {.timestamp = QDateTime::currentDateTime(),
                                        .channel = kCommentChannel,
                                        .message = "Protocol Debugger: Internal Error"};
    if (i < m_log.size()) {
        std::cout << "Log at: " << i << std::endl;
        return m_log.at(i);
    }

    return default_event;
}
