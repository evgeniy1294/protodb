#pragma once

#include <protodb/plugins/IOWidget.h>

class QLineEdit;
class QComboBox;

class NetIOWidget: public IOWidget
{
  Q_OBJECT

public:
    NetIOWidget(QWidget *parent = nullptr);
    ~NetIOWidget() = default;

    QString name() const override { return QString("Network"); }
private:
    void createGui();
    void connectSignals();

private:
    QLineEdit* m_ip;
    QLineEdit* m_port;
    QComboBox* m_protocol;
};

