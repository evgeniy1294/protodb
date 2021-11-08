#pragma once

#include <QWidget>
#include <QFrame>

class QCheckBox;
class QLineEdit;
class Logger;

class LogConfigWidget: public QFrame
{
    Q_OBJECT

public:
    LogConfigWidget(QWidget* aParent = nullptr);
    ~LogConfigWidget() = default;

    void setLogger(Logger* logger);

private:
    void createGui();
    void connectSignals();

private:
    QCheckBox* m_append_en;
    QCheckBox* m_timestamp_en;
    QCheckBox* m_ch1_en;
    QCheckBox* m_ch2_en;
    QCheckBox* m_comment_en;
    QLineEdit* m_timestamp_format;
    QLineEdit* m_ch1_name;
    QLineEdit* m_ch2_name;
};
