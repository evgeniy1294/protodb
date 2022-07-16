#include "LogPrinter.h"
#include "LogFormatter.h"

LogPrinter::LogPrinter(QObject *parent)
    : QObject(parent)
    , m_fmt(new LogFormatter)
    , m_append_flag(false)
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
        m_log_file.close();
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

void LogPrinter::print(const Logger::Event& event)
{
    if (m_log_file.isOpen()) {
        QString text = m_fmt != nullptr ? m_fmt->format(event) : LogFormatter::defaultFormat(event);
        m_log_file.write(text.toLatin1() + "\n\n");
    }
}



