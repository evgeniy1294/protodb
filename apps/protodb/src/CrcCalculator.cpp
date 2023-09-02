#include "CrcCalculator.h"
#include "BytecodeEditor.h"
#include "FilteredComboBox.h"

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

    m_set_model_btn = new QPushButton(tr("Set"));

    m_model_sel = new FilteredComboBox();
        m_model_sel->addItems(CrcLogic::standartModels());

    m_width = new QSpinBox();
        m_width->setMinimum(3);
        m_width->setMaximum(64);
        m_width->setValue(32);

    m_poly = new QLineEdit();
        m_poly->setPlaceholderText(tr("Poly"));
        m_poly->setToolTip(tr("Polynominal"));

    m_init = new QLineEdit();
        m_init->setPlaceholderText(tr("Seed"));
        m_init->setToolTip(tr("Seed"));

    m_xor_out = new QLineEdit();
        m_xor_out->setPlaceholderText(tr("XOR out"));
        m_xor_out->setToolTip(tr("XOR out"));

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
        m_layout->addWidget(new QLabel(tr("Models:")), 0, 0, 1, 1);
        m_layout->addWidget(m_model_sel, 0, 1, 1, 1);
        m_layout->addWidget(m_set_model_btn, 0, 2, 1, 1);

        m_layout->addWidget(new QLabel(tr("Width:")), 1, 0, 1, 1);
        m_layout->addWidget(m_width, 1, 1, 1, 1);

        m_layout->addWidget(new QLabel(tr("Polynomial:")), 2, 0, 1, 1);
        m_layout->addWidget(m_poly, 2, 1, 1, 1);

        m_layout->addWidget(new QLabel(tr("Seed:")), 3, 0, 1, 1);
        m_layout->addWidget(m_init, 3, 1, 1, 1);
        m_layout->addWidget(new QLabel(tr("XOR out:")), 4, 0, 1, 1);
        m_layout->addWidget(m_xor_out, 4, 1, 1, 1);

        m_layout->addWidget(m_ref_in, 1, 2, 1, 1);
        m_layout->addWidget(m_ref_out, 2, 2, 1, 1);

        m_layout->addWidget(new QLabel(), 5, 2, 1, 1);

        m_layout->addWidget(m_editor, 6, 0, 1, 3);
        m_layout->addWidget(m_result, 7, 0, 1, 3);
        m_layout->addWidget(m_calc_btn, 8, 0, 1, 3, Qt::AlignRight);

    setLayout(m_layout);
    setWindowModality(Qt::NonModal);
    resize(640, 480);
}

void protodb::CrcCalculator::connect_signals()
{
    connect(m_calc_btn, &QPushButton::clicked, this, &CrcCalculator::calculate);
    connect(m_set_model_btn, &QPushButton::clicked, this, [this]() {
        auto name = m_model_sel->currentText();
        if (CrcLogic::standartModels().contains(name)) {
            m_crc.setModel(name);

            auto model = m_crc.model();
            m_width->setValue(model.width);
            m_poly->setText(QString("0x%1").arg(QString::number(model.poly, 16)));
            m_init->setText(QString("0x%1").arg(QString::number(model.seed, 16)));
            m_xor_out->setText(QString("0x%1").arg(QString::number(model.xor_out, 16)));
            m_ref_in->setChecked(model.ref_in);
            m_ref_out->setChecked(model.ref_out);
        }
    });
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
