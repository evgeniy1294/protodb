#include "protodb/SerialControlWidget.h"

#include <protodb/SerialConnection.h>

#include <QLabel>
#include <QCheckBox>
#include <QGridLayout>
#include <QEvent>

#include <QTimer>

using namespace protodb;

SerialControlWidget::SerialControlWidget(QWidget* parent)
    : ConnectionAdvanceControlWidget(parent)
    , m_conn(nullptr)
{
    m_poll_tmr = new QTimer(this);
        m_poll_tmr->setTimerType(Qt::TimerType::CoarseTimer);
        m_poll_tmr->setInterval(kPollIntervalMs);
        m_poll_tmr->setSingleShot(false);

    create_gui();
    connect_signals();
}

void SerialControlWidget::setConnection(QSharedPointer<Connection> conn)
{
    setSerialConnection(qSharedPointerCast<SerialConnection>(conn));
}

void SerialControlWidget::setSerialConnection(QSharedPointer<SerialConnection> conn)
{

    if (conn) {
        m_conn = conn;
            m_rts->setChecked(m_conn->isRequestToSend());
            m_dtr->setChecked(m_conn->isDataTerminalReady());
            updatePinoutState();

            m_poll_tmr->start();
    }
}

QString SerialControlWidget::deviceCID() const
{
    return QString("SerialConnectionCreator");
}

void SerialControlWidget::create_gui()
{
    static const QString style_red = "QCheckBox::indicator { width: 16px; height: 16px; }"
                                     "QCheckBox::indicator:checked {"
                                        "image: url(:/icons/lamp_on_red.svg);"
                                     "}"
                                     "QCheckBox::indicator:unchecked"
                                     "{"
                                        "image: url(:/icons/lamp_off.svg);"
                                     "}";

    static const QString style_green = "QCheckBox::indicator { width: 16px; height: 16px; }"
                                       "QCheckBox::indicator:checked {"
                                           "image: url(:/icons/lamp_on_green.svg);"
                                       "}"
                                       "QCheckBox::indicator:unchecked"
                                       "{"
                                           "image: url(:/icons/lamp_off.svg);"
                                       "}";

    m_rts = new QCheckBox("RTS");
        m_rts->setToolTip("Request to Send");
        m_rts->setStyleSheet(style_green);

    m_dtr = new QCheckBox("DTR");
        m_dtr->setToolTip("Data Terminal Ready");
        m_dtr->setStyleSheet(style_green);

    m_cts = new QCheckBox("CTS");
        m_cts->setToolTip("Clear to Send");
        m_cts->installEventFilter(this);
        m_cts->setStyleSheet(style_red);

    m_dsr = new QCheckBox("DSR");
        m_dsr->setToolTip("Data Set Ready");
        m_dsr->installEventFilter(this);
        m_dsr->setStyleSheet(style_red);
        m_dsr->setChecked(true);

    m_dcd = new QCheckBox("DCD");
        m_dcd->setToolTip("Carrier Detect");
        m_dcd->installEventFilter(this);
        m_dcd->setStyleSheet(style_red);

    m_ri  = new QCheckBox("RNG");
        m_ri->setToolTip("Ring Indicator");
        m_ri->installEventFilter(this);
        m_ri->setCheckable(false);
        m_ri->setStyleSheet(style_red);

    m_std  = new QCheckBox("STD");
        m_std->setToolTip("Secondary Transmitted Data");
        m_std->installEventFilter(this);
        m_std->setStyleSheet(style_red);

    m_srd  = new QCheckBox("SRD");
        m_srd->setToolTip("Secondary Received Data");
        m_srd->installEventFilter(this);
        m_srd->setStyleSheet(style_red);

    auto m_layout = new QHBoxLayout();
        m_layout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
        m_layout->addWidget(m_rts);
        m_layout->addWidget(m_dtr);
        m_layout->addWidget(m_cts);
        m_layout->addWidget(m_dsr);
        m_layout->addWidget(m_dcd);
        m_layout->addWidget(m_ri);
        m_layout->addWidget(m_std);
        m_layout->addWidget(m_srd);
        m_layout->setStretch(0, 1);
        m_layout->setStretch(1, 1);
        m_layout->setStretch(2, 1);
        m_layout->setStretch(3, 1);
        m_layout->setStretch(4, 1);
        m_layout->setStretch(5, 1);
        m_layout->setStretch(6, 1);

    setLayout(m_layout);
}

void SerialControlWidget::connect_signals()
{
    connect(m_poll_tmr, &QTimer::timeout, this, &SerialControlWidget::updatePinoutState);

    connect(m_rts, &QCheckBox::clicked, this, [this]() {
        if (m_conn) {
            m_conn->setRequestToSend(m_rts->isChecked());
            m_rts->setChecked(m_conn->isRequestToSend());
        }
    });

    connect(m_dtr, &QCheckBox::clicked, this, [this]() {
        if (m_conn) {
            m_conn->setDataTerminalReady(m_dtr->isChecked());
            m_dtr->setChecked(m_conn->isDataTerminalReady());
        }
    });
}

void SerialControlWidget::updatePinoutState()
{
    if (m_conn) {
        auto pinout = m_conn->pinoutSignals();
            m_cts->setChecked((pinout & QSerialPort::ClearToSendSignal) != 0);
            m_dsr->setChecked((pinout & QSerialPort::DataSetReadySignal) != 0);
            m_dcd->setChecked((pinout & QSerialPort::DataCarrierDetectSignal) != 0);
            m_std->setChecked((pinout & QSerialPort::SecondaryTransmittedDataSignal) != 0);
            m_srd->setChecked((pinout & QSerialPort::SecondaryReceivedDataSignal) != 0);
            m_ri ->setChecked((pinout & QSerialPort::RingIndicatorSignal) != 0);
    }
}

bool SerialControlWidget::eventFilter(QObject* target, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        return true;
    }

    return ConnectionAdvanceControlWidget::eventFilter(target, event);
}
