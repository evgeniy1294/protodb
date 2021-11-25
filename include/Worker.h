#pragma once

#include <QObject>

class QAbstractTableModel;
class SequenceTableModel;


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
    SequenceTableModel* m_inc_sequences;
    SequenceTableModel* m_out_sequences;
    //LogTableModel* m_log;
};
