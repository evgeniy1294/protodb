#include "Worker.h"
#include "SequenceTableModel.h"
#include "LogTableModel.h"



Worker::Worker(QObject *parent)
    : QObject(parent)
{

}

QAbstractTableModel *Worker::incomingSequences()
{
    return m_inc_sequences;
}

QAbstractTableModel *Worker::outgoingSequences()
{
    return m_out_sequences;
}

QAbstractTableModel *Worker::log()
{
    return m_log;
}

LogStyle Worker::logStyle()
{
    return {};
}

void Worker::setLogStyle(const LogStyle &style)
{

}

LogConfigs Worker::logConfigs()
{

}

void Worker::setLogConfigs(const LogConfigs &configs)
{

}

void Worker::onReceived(const QDateTime &, const QByteArray &)
{

}

void Worker::onTransmitted(const QDateTime &, const QUuid &)
{

}
