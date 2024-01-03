#pragma once

#include <protodb/ConnectionAdvanceControlWidget.h>

class QLabel;
class QCheckBox;

class QTimer;

namespace protodb {

class SerialConnection;

class SerialControlWidget final: public ConnectionAdvanceControlWidget
{
    static constexpr int kPollIntervalMs = 10;

public:
    explicit SerialControlWidget(QWidget* parent = nullptr);
    ~SerialControlWidget() = default;

    void setConnection(QSharedPointer<Connection> conn) override;
    void setSerialConnection(QSharedPointer<SerialConnection> conn);

    QString deviceCID() const override;
private:
    void create_gui();
    void connect_signals();

    void updatePinoutState();

    bool eventFilter(QObject *target, QEvent *event) override;

private:
    QSharedPointer<SerialConnection> m_conn;
    QTimer* m_poll_tmr;

    QCheckBox* m_rts;
    QCheckBox* m_dtr;
    QCheckBox* m_cts;
    QCheckBox* m_dsr;
    QCheckBox* m_dcd;
    QCheckBox* m_ri;
    QCheckBox* m_std;
    QCheckBox* m_srd;
};

} // namespace protodb
