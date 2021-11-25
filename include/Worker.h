#pragma once

#include <QObject>

class QAbstractTableModel;
class SequenceModel;


class Worker: public QObject {
    Q_OBJECT

public:
    Worker(QObject* parent = nullptr);
   ~Worker() = default;

    QAbstractTableModel* incomingSequences();
    QAbstractTableModel* outgoingSequences();


public slots:
    void onReceived(const QDateTime&, const QByteArray&);
    void onTransmitted(const QDateTime&, const QUuid&);

signals:
    void print(const QString&);
    void transmit(const QUuid&, const QByteArray&);

private:
    SequenceModel* m_inc_sequences;
    SequenceModel* m_out_sequences;
    //LogTableModel* m_log;
};
