#pragma once

#include <protodb/utils/JsonBaseUtils.h>

#include <QObject>
#include <QStringList>

class QIODevice;

class Sequence;
class Logger;
class LogPrinter;
class SequenceModel;
class ScriptInterface;

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

    QStringList supportedSyntaxes() const;
public slots:
    void start(const nlohmann::json& attr);
    void stop();

signals:
    void sStarted();
    void sStopted();

private:
    MainClass();
   ~MainClass();

    void init_factories();
    void init_logger();
    void init_syntaxes();

    void config_logger(const nlohmann::json& json);

private:
    SequenceModel* m_incoming_sequences;
    SequenceModel* m_outgoing_sequences;
    QStringList m_supported_syntaxes;
    QList<ScriptInterface*> m_script_interfaces;

    Logger* m_logger;
    LogPrinter* m_log_printer;
    QIODevice* m_io;
};
