#pragma once

#include <protodb/utils/JsonBaseUtils.h>
#include <QObject>

class QIODevice;

class Logger;
class LogFormatter;
class LogPrinter;
class SequenceModel;

class MainClass final: public QObject
{
    Q_OBJECT

public:
    static MainClass& instance();

    void init();
    bool isStarted() const;

    SequenceModel* incomingSequences() const;
    SequenceModel* outgoingSequences() const;
    Logger* logger() const;
    LogFormatter* logFormatter() const;

public slots:
    void start(const nlohmann::json& attr);
    void stop();

signals:
    void sStarted();
    void sStopted();

private:
    MainClass();
   ~MainClass();

private:
    SequenceModel* m_incoming_sequences;
    SequenceModel* m_outgoing_sequences;
    Logger* m_logger;
    LogFormatter* m_log_formatter;
    LogPrinter* m_log_printer;
    QIODevice* m_io;
};
