#pragma once

#include "Sequence.h"

#include <protodb/utils/JsonBaseUtils.h>

#include <QObject>
#include <QStringList>
#include <QSharedPointer>
#include <QMap>

class QIODevice;

namespace protodb {

class Logger;
class LogPrinter;
class Sequence;
class SequenceModel;
class ScriptMultiInterface;

class MainClass final: public QObject
{
    Q_OBJECT

public:
    static MainClass& instance();

    void init();
    bool isStarted() const;

    void setSeanceConfigs(const nlohmann::json& cfg);
    void seanceConfigs(nlohmann::json& cfg) const;

    SequenceModel* incomingSequences() const;
    SequenceModel* outgoingSequences() const;
    Logger* logger() const;

public slots:
    void start();
    void stop();

    void sendBytes(QByteArray& bytes);

signals:
    void sStarted();
    void sStopted();
    void sStartFailed();

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
    bool try_create_connection(const QString& cid, const nlohmann::json& json);

    bool send_sequence(QSharedPointer<const Sequence>& sequence);

private:
    SequenceModel* m_incoming_sequences;
    SequenceModel* m_outgoing_sequences;
    ScriptMultiInterface* m_script_multi_interface;

    QMap<int, QUuid> m_repeat_timers;
    QMap<int, QUuid> m_singleshot_timers;

    Logger* m_logger;
    LogPrinter* m_log_printer;
    QIODevice* m_io;

    nlohmann::json m_seance_cfg;
};

} // namespace protodb
