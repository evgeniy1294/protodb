#include "Worker.h"
#include "SequenceTableModel.h"



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



void Worker::onReceived(const QDateTime &, const QByteArray &)
{

}

void Worker::onTransmitted(const QDateTime &, const QUuid &)
{

}
