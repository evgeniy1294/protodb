#pragma once

#include <QDialog>

#include <protodb/utils/crc_logic.h>

class QCheckBox;
class QLineEdit;
class QSpinBox;
class QPlainTextEdit;
class QPushButton;

namespace protodb {

class BytecodeEditor;
class FilteredComboBox;

class ChecksumCalculator: public QWidget
{
    Q_OBJECT

public:
    ChecksumCalculator(QWidget* parent = nullptr);
   ~ChecksumCalculator() = default;

    void setModel(CrcModel& model);
    void setModel(QString& name);

    void setData(const QByteArray& data, bool calculate = false);

private:
    void create_gui();
    void connect_signals();

private slots:
    void calculate();

private:
    CrcLogic m_crc;

    BytecodeEditor* m_editor;

    FilteredComboBox* m_model_sel;
    QSpinBox*  m_width;
    QLineEdit* m_poly;
    QLineEdit* m_init;
    QLineEdit* m_xor_out;
    QLineEdit* m_result;
    QCheckBox* m_ref_in;
    QCheckBox* m_ref_out;
    QPushButton* m_calc_btn;
    QPushButton* m_set_model_btn;
};

} // namespace protodb
