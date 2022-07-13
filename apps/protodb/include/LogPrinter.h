#pragma once

#include "Logger.h"

#include <QObject>

class LogFormatter;

class LogPrinter: public QObject
{
    Q_OBJECT

public:
    LogPrinter(QObject* parent = nullptr);
   ~LogPrinter() = default;

    void setLogFile(const QString& path);
    QString logFile() const;

    void setFormatter(LogFormatter* fmt);
    LogFormatter* formatter() const;

    bool setEnabled(bool enabled = true);
    bool enabled() const;

    bool setDisabled(bool disabled = true);
    bool disabled();

public slots:
    void print(Logger::Event event);

private:
    QFile m_log_file;
    LogFormatter* m_fmt;
};
