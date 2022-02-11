#pragma once

#include <QObject>

class QAbstractTableModel;
class SequenceModel;
class LogModel;


class Worker: public QObject
{
    Q_OBJECT

public:
    Worker(QObject* parent = nullptr);
   ~Worker() = default;

    SequenceModel* incomingSequences();
    SequenceModel* outgoingSequences();
    LogModel*      logModel();

public slots:
    void onStart();
    void onStop();
    void onReceived(const QDateTime& timestamp, const QByteArray& data);
    void onTransmitted(const QDateTime& timestamp, const QByteArray& data, const QUuid& uuid);
    void onError(const QString&);

signals:
    void sStart(); // Передавать QIoDevice или конфиг?
    void sStop();
    void sTransmit(const QUuid&, const QByteArray&);

private:
    SequenceModel* m_incoming_sequences;
    SequenceModel* m_outgoing_sequences;
    LogModel* m_log_model;
};
