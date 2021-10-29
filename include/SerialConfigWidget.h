#pragma once

#include <QWidget>

class QComboBox;
class QPushButton;
class QLineEdit;

class SerialConfigWidget: public QFrame
{
  Q_OBJECT

public:
    explicit SerialConfigWidget(QWidget *parent = nullptr);
    ~SerialConfigWidget() = default;

private:
    void createGui();

private:
    QComboBox*   m_baudrate;
    QComboBox*   m_flow_ctrl;
    QComboBox*   m_data_bits;
    QComboBox*   m_parity;
    QComboBox*   m_stop_bits;
    QComboBox*   m_open_mode;
    QComboBox*   m_device;
    QPushButton* mLogfileBtn;
    QLineEdit*   mLogfileLe;
};
