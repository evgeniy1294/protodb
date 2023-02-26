#pragma once

#include <QObject>
#include <QDateTime>

class QTimer;

namespace protodb {

class Delimeter: public QObject {
    Q_OBJECT

public:
    Delimeter();
   ~Delimeter() = default;

    void setD1(int d1);
    int d1() const;

    void setD2(int d2);
    int d2() const;

    void setBypassEnabled(bool en);
    bool bypassEnabled() const;

    void setGuardTime(int time);
    int guardTime() const;

    QByteArray buffer() const;

    void pushData(QByteArray data);

signals:
    void dataCaptured(QByteArray data, QDateTime time);

private:
    int m_d1;
    int m_d2;
    bool m_bypass;
    QTimer* m_guard_timer;

    QByteArray m_buffer;
};

} // namespace protodb
