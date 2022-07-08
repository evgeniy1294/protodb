#pragma once

#include <protodb/IOWidget.h>

class QComboBox;
class QValidator;

class SerialIOWIdget: public IOWidget
{
  Q_OBJECT

public:
    SerialIOWIdget(QWidget *parent = nullptr);
    ~SerialIOWIdget() = default;

    QString name() const override { return QString("Serial"); }
private:
    void createGui();
    void connectSignals();

private:
    QValidator* m_baudrate_validator;
    QComboBox* m_baudrate;
    QComboBox* m_flow_ctrl;
    QComboBox* m_data_bits;
    QComboBox* m_parity;
    QComboBox* m_stop_bits;
    QComboBox* m_open_mode;
    QComboBox* m_device;
};
