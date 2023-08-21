#pragma once

#include <QDialog>

#include <protodb/utils/crc_logic.h>

class QLineEdit;
class QCheckBox;
class QSpinBox;
class QPlainTextEdit;
class QPushButton;

namespace protodb {

class BytecodeEditor;

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

    BytecodeEditor* m_editor;

    QSpinBox*  m_width;
    QLineEdit* m_poly;
    QLineEdit* m_init;
    QLineEdit* m_xor_out;
    QLineEdit* m_result;
    QCheckBox* m_ref_in;
    QCheckBox* m_ref_out;
    QPushButton* m_calc_btn;
};

} // namespace protodb
