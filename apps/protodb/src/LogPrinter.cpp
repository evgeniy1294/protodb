#include "LogPrinter.h"
#include "LogFormatter.h"

#include <QTimer>

using namespace protodb;

LogPrinter::LogPrinter(QObject *parent)
    : QObject(parent)
    , m_fmt(new LogFormatter)
    , m_append_flag(false)
    , m_fmt_flags(LogFormatter::AllEnabled)
{

}

LogPrinter::~LogPrinter()
{
    delete m_fmt;
}

void LogPrinter::setLogFile(const QString &path)
{
    bool opened = m_log_file.isOpen();
    m_log_file.close();

    m_log_file.setFileName(path);
    if (opened) {
        m_log_file.open(QFile::WriteOnly | (m_append_flag ? QFile::Append : QFile::NotOpen));
    }
}

QString LogPrinter::logFile() const
{
    return m_log_file.fileName();
}

void LogPrinter::setAppendFile(bool append)
{
    m_append_flag = append;
}

bool LogPrinter::appendFile() const
{
    return m_append_flag;
}

bool LogPrinter::setEnabled(bool enabled)
{
    if (enabled) {
        m_log_file.open(QFile::WriteOnly | (m_append_flag ? QFile::Append : QFile::NotOpen));
    }
    else {
        m_log_file.close();
    }

    return m_log_file.isOpen();
}

bool LogPrinter::enabled() const
{
    return m_log_file.isOpen();
}

bool LogPrinter::setDisabled(bool disabled)
{
    if (disabled) {
        QTimer::singleShot(5, this, [this]() {
            m_log_file.close();
        });
    }
    else {
        m_log_file.open(QFile::WriteOnly);
    }

    return !m_log_file.isOpen();
}

bool LogPrinter::disabled()
{
    return !m_log_file.isOpen();
}

void LogPrinter::setTimestampEnabled(bool enabled)
{
    if (enabled) {
        m_fmt_flags |= LogFormatter::TimeStampEnabled;
    }
    else {
        m_fmt_flags &= ~LogFormatter::TimeStampEnabled;
    }
}

bool LogPrinter::timestampEnabled() const
{
    return (m_fmt_flags & LogFormatter::TimeStampEnabled) != 0;
}

void LogPrinter::setChannelNameEnabled(bool enabled)
{
    if (enabled) {
        m_fmt_flags |= LogFormatter::ChannelNameEnabled;
    }
    else {
        m_fmt_flags &= ~LogFormatter::ChannelNameEnabled;
    }
}

bool LogPrinter::channelNameEnabled() const
{
    return (m_fmt_flags & LogFormatter::ChannelNameEnabled) != 0;
}

LogFormatter* LogPrinter::formatter() const
{
    return m_fmt;
}

void LogPrinter::print(const Logger::Event& event)
{
    if (m_log_file.isOpen()) {
        QString text = m_fmt != nullptr ? m_fmt->format(event) : LogFormatter::defaultFormat(event);
        m_log_file.write(text.toLatin1() + "\n\n");
    }
}



