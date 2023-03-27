#pragma once

#include <QDialog>

#include <protodb/utils/crc_logic.h>

class QLineEdit;
class QCheckBox;
class QSpinBox;
class QPlainTextEdit;
class QPushButton;

namespace protodb {

class CrcCalculator: public QWidget
{
    Q_OBJECT

public:
    CrcCalculator(QWidget* parent = nullptr);
   ~CrcCalculator() = default;

private:
    void create_gui();
    void connect_signals();

private slots:
    void calculate();

private:
    CrcLogic m_crc;
    bool m_fmt;

    QSpinBox*  m_width;
    QLineEdit* m_poly;
    QLineEdit* m_init;
    QLineEdit* m_xor_out;
    QLineEdit* m_result;
    QCheckBox* m_ref_in;
    QCheckBox* m_ref_out;
    QPlainTextEdit* m_bytes;
    QPushButton* m_calc_btn;
    QPushButton* m_fmt_btn;
};

} // namespace protodb
