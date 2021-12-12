#include "Worker.h"
#include "SequenceModel.h"
#include "LogModel.h"
#include "LogFormatter.h"
#include "LogDecorator.h"

Worker::Worker(QObject *parent)
    : QObject(parent)
    , m_incoming_sequences(new SequenceModel(this))
    , m_outgoing_sequences(new SequenceModel(this))
    , m_log_model(new LogModel(this))
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

LogModel *Worker::logModel()
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
    LogEvent event = {.timestamp = timestamp, .channel = kFirstLogChannel, .message = data};
    m_log_model->log(event);
}

void Worker::onTransmitted(const QDateTime& timestamp, const QByteArray& data, const QUuid& uuid)
{
    LogEvent event = {.timestamp = timestamp, .channel = kSecondLogChannel, .message = data};
    m_log_model->log(event);
}

void Worker::onError(const QString &)
{
    emit sStop();
}

void Worker::fromJson(const nlohmann::json &json)
{
    m_log_model->decorator()->fromJson(json);
    m_log_model->formatter()->fromJson(json);
}

void Worker::toJson(nlohmann::json &json) const
{
    m_log_model->decorator()->toJson(json);
    m_log_model->formatter()->toJson(json);
}

const QString& Worker::jsonPrefix() const
{
    static const QString m_prefix("Configurations");
    return m_prefix;
}
