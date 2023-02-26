#include "Delimeter.h"

#include <QTimer>

using namespace protodb;

Delimeter::Delimeter()
    : m_guard_timer(new QTimer(this))
    , m_d1(-1)
    , m_d2(-1)
    , m_bypass(false)
{
    m_guard_timer->setTimerType(Qt::TimerType::PreciseTimer);
    m_guard_timer->setSingleShot(true);

    connect(m_guard_timer, &QTimer::timeout, this, [this]() {
        // emit dataCaptured(m_buffer);
    });
}

void Delimeter::setD1(int d1)
{
    m_d1 = d1;
}

int Delimeter::d1() const
{
    return m_d1;
}

void Delimeter::setD2(int d2)
{
    m_d2 = d2;
}

int Delimeter::d2() const
{
    return m_d2;
}

void Delimeter::setBypassEnabled(bool en)
{
    m_bypass = en;
}

bool Delimeter::bypassEnabled() const
{
    return m_bypass;
}

void Delimeter::setGuardTime(int time)
{
    m_guard_timer->setInterval(time);
}

int Delimeter::guardTime() const
{
    return m_guard_timer->interval();
}

void Delimeter::pushData(QByteArray data)
{
    m_buffer.append(data);

    //if (m_d1 != )
}
