#include "CrcCalculator.h"
#include "SequenceEditor.h"

#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QApplication>


protodb::CrcCalculator::CrcCalculator(QWidget* parent)
{
    create_gui();
    connect_signals();
}

void protodb::CrcCalculator::create_gui()
{
    setWindowTitle(tr("CRC Calculator"));

    m_width = new QSpinBox();
        m_width->setMinimum(3);
        m_width->setMaximum(64);
        m_width->setValue(32);
        m_width->setPrefix(tr("Width: "));

    m_poly = new QLineEdit();
        m_poly->setPlaceholderText(tr("Poly"));
        m_poly->setToolTip(tr("Polynominal"));

    m_init = new QLineEdit();
        m_init->setPlaceholderText(tr("Seed"));
        m_init->setToolTip(tr("Seed"));

    m_xor_out = new QLineEdit();
        m_xor_out->setPlaceholderText(tr("Xor value"));
        m_xor_out->setToolTip(tr("Xor out"));

    m_result = new QLineEdit();
        m_result->setPlaceholderText(tr("Result"));
        m_result->setToolTip(tr("Result"));
        m_result->setReadOnly(true);

    m_ref_in = new QCheckBox();
        m_ref_in->setText(tr("Reflect In"));

    m_ref_out = new QCheckBox();
        m_ref_out->setText(tr("Reflect Out"));


    m_calc_btn = new QPushButton(tr("Calculate"));

    m_editor = new BytecodeEditor();

    auto m_layout = new QGridLayout();
        m_layout->addWidget(m_width, 0, 0, 1, 1);

        m_layout->addWidget(m_poly, 1, 0, 1, 1);

        m_layout->addWidget(m_init, 2, 0, 1, 1);
        m_layout->addWidget(m_xor_out, 3, 0, 1, 1);
        m_layout->addWidget(m_result, 4, 0, 1, 1);

        m_layout->addWidget(m_ref_in, 0, 1, 1, 1);
        m_layout->addWidget(m_ref_out, 1, 1, 1, 1);

        m_layout->addWidget(m_editor, 5, 0, 1, 2);
        m_layout->addWidget(m_calc_btn, 6, 0, 1, 2, Qt::AlignRight);

    setLayout(m_layout);
    setWindowModality(Qt::NonModal);
    resize(640, 480);
}

void protodb::CrcCalculator::connect_signals()
{
    connect(m_calc_btn, &QPushButton::clicked, this, &CrcCalculator::calculate);
}

void protodb::CrcCalculator::calculate()
{
    auto palette = QApplication::palette();
        palette.setColor(QPalette::Base, QColor(0xffcece));

    bool ok = false;
    bool fail = false;

    m_crc.setWidth(m_width->value());

    auto poly = m_poly->text().toLong(&ok, 16);
    if (ok) {
        m_poly->setPalette(QApplication::palette());
        m_crc.setPoly(poly);
    }
    else {
        m_poly->setPalette(palette);
        fail = true;
    }

    auto seed = m_init->text().toLong(&ok, 16);
    if (ok) {
        m_init->setPalette(QApplication::palette());
        m_crc.setSeed(seed);
    }
    else {
        m_init->setPalette(palette);
        fail = true;
    }

    auto xor_out = m_xor_out->text().toLong(&ok, 16);
    if (ok) {
        m_xor_out->setPalette(QApplication::palette());
        m_crc.setXorOut(xor_out); }
    else {
        m_xor_out->setPalette(palette);
        fail = true;
    }

    m_crc.setReflectIn(m_ref_in->isChecked());
    m_crc.setReflectOut(m_ref_out->isChecked());

    auto bytes = m_editor->currentData();

    if (bytes.isEmpty()) {
        return;
    }

    if (!fail) {
        m_crc.calculate(reinterpret_cast<uint8_t*>(bytes.data()),
                        reinterpret_cast<uint8_t*>(bytes.end()));
        m_result->setText(QString::number(m_crc.finalize(), 16));
    }

    return;
}
