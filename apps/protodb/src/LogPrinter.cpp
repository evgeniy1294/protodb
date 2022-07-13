#include "LogPrinter.h"
#include "LogFormatter.h"

LogPrinter::LogPrinter(QObject *parent)
    : QObject(parent)
    , m_fmt(nullptr)
{

}

void LogPrinter::setLogFile(const QString &path)
{
    if ( QFile::exists(path) ) {
        bool opened = m_log_file.isOpen();
        m_log_file.close();

        m_log_file.setFileName(path);
        if (opened) {
            m_log_file.open(QFile::WriteOnly);
        }
    }
}

QString LogPrinter::logFile() const
{
    return m_log_file.fileName();
}

void LogPrinter::setFormatter(LogFormatter *fmt)
{
    if (fmt != m_fmt && fmt != nullptr)
        m_fmt = fmt;
}

LogFormatter *LogPrinter::formatter() const
{
    return m_fmt;
}

bool LogPrinter::setEnabled(bool enabled)
{
    if (enabled) {
        m_log_file.open(QFile::WriteOnly);
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

void LogPrinter::print(Logger::Event event)
{
    if (m_log_file.isOpen()) {
        QString text = m_fmt != nullptr ? m_fmt->format(event) : LogFormatter::defaultFormat(event);
        m_log_file.write(text.toLatin1());
    }
}


