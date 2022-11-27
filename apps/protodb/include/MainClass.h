#pragma once

#include "Sequence.h"
#include <protodb/utils/JsonBaseUtils.h>

#include <QObject>
#include <QStringList>
#include <QSharedPointer>
#include <QMap>

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

private slots:
    void sequenceActivated(QUuid uid);
    void sequenceDisactivated(QUuid uid);

    void timerEvent(QTimerEvent *event) override;
    void readyRead();

private:
    MainClass();
   ~MainClass();

    void init_factories();
    void init_logger();
    void init_syntaxes();
    void connect_signals();

    void config_logger(const nlohmann::json& json);
    bool try_create_connection(const nlohmann::json& json);

    bool send_sequence(QSharedPointer<Sequence>& sequence);

private:
    SequenceModel* m_incoming_sequences;
    SequenceModel* m_outgoing_sequences;
    QStringList m_supported_syntaxes;
    QList<ScriptInterface*> m_script_interfaces;

    QMap<int, QUuid> m_repeat_timers;
    QMap<int, QUuid> m_singleshot_timers;

    Logger* m_logger;
    LogPrinter* m_log_printer;
    QIODevice* m_io;
};
