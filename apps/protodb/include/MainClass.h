#pragma once

#include <protodb/utils/JsonBaseUtils.h>
#include <QObject>

class QIODevice;

class Logger;
class SequenceModel;

class MainClass final: public QObject
{
    Q_OBJECT

public:
    static MainClass& instance();

    void init();

    SequenceModel* incomingSequences() const;
    SequenceModel* outgoingSequences() const;
    Logger* logger() const;

    bool isStarted() const;

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
    QIODevice* m_io;
};
