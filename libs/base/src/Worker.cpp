#include "Worker.h"
#include "SequenceModel.h"
#include "Logger.h"

#include <protodb/shared_types/LogTypes.h>

Worker::Worker(QObject *parent)
    : QObject(parent)
    , m_incoming_sequences(new SequenceModel(this))
    , m_outgoing_sequences(new SequenceModel(this))
{
    m_incoming_sequences->setIncomingMode();
    m_outgoing_sequences->setOutgoingMode();
}

SequenceModel* Worker::incomingSequences()
{
    return m_incoming_sequences;
}

SequenceModel* Worker::outgoingSequences()
{
    return m_outgoing_sequences;
}

Logger *Worker::logModel()
{
    return m_log_model;
}

void Worker::onStart()
{
    emit sStart();
}

void Worker::onStop()
{
    emit sStop();
}

void Worker::onReceived(const QDateTime& timestamp, const QByteArray& data)
{

}

void Worker::onTransmitted(const QDateTime& timestamp, const QByteArray& data, const QUuid& uuid)
{

}

void Worker::onError(const QString &)
{
    emit sStop();
}
