#include "CrcCalculator.h"
#include "BytecodeEditor.h"
#include "FilteredComboBox.h"

#include <protodb/SpoilerWidget.h>

#include <QSpinBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QApplication>

protodb::ChecksumCalculator::ChecksumCalculator(QWidget* parent)
{
    create_gui();
    connect_signals();
}

void protodb::ChecksumCalculator::setModel(CrcModel& model)
{
    if (m_crc.setModel(model)) {
        m_width->setValue(model.width);
        m_poly->setText(QString("0x%1").arg(QString::number(model.poly, 16)));
        m_init->setText(QString("0x%1").arg(QString::number(model.seed, 16)));
        m_xor_out->setText(QString("0x%1").arg(QString::number(model.xor_out, 16)));
        m_ref_in->setChecked(model.ref_in);
        m_ref_out->setChecked(model.ref_out);
    }
}

void protodb::ChecksumCalculator::setModel(QString& model)
{
    if (m_crc.setModel(model)) {
        auto model = m_crc.model();

        m_width->setValue(model.width);
        m_poly->setText(QString("0x%1").arg(QString::number(model.poly, 16)));
        m_init->setText(QString("0x%1").arg(QString::number(model.seed, 16)));
        m_xor_out->setText(QString("0x%1").arg(QString::number(model.xor_out, 16)));
        m_ref_in->setChecked(model.ref_in);
        m_ref_out->setChecked(model.ref_out);
    }
}

protodb::CrcModel protodb::ChecksumCalculator::currentModel() const
{
    return m_crc.model();
}

void protodb::ChecksumCalculator::setData(const QByteArray& data, bool calculate)
{
    m_editor->setData(data);
    if (calculate) {
        this->calculate();
    }
}

void protodb::ChecksumCalculator::create_gui()
{
    setWindowTitle(tr("CRC Calculator"));

    m_set_model_btn = new QPushButton(tr("Set"));

    m_model_sel = new FilteredComboBox();
        m_model_sel->addItems(CrcLogic::standartModels());
        m_model_sel->setCurrentText("");

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

    auto m_spoiler_layout = new QGridLayout();
        m_spoiler_layout->addWidget(new QLabel(tr("Width: ")));
        m_spoiler_layout->addWidget(m_width, 0, 1, 1, 1);
        m_spoiler_layout->addWidget(new QLabel(tr("Polynomial: ")), 1, 0, 1, 1);
        m_spoiler_layout->addWidget(m_poly, 1, 1, 1, 1);
        m_spoiler_layout->addWidget(new QLabel(tr("Seed: ")), 2, 0, 1, 1);
        m_spoiler_layout->addWidget(m_init, 2, 1, 1, 1);
        m_spoiler_layout->addWidget(new QLabel(tr("XOR out: ")), 3, 0, 1, 1);
        m_spoiler_layout->addWidget(m_xor_out, 3, 1, 1, 1);
        m_spoiler_layout->addWidget(m_ref_in, 0, 2, 1, 1);
        m_spoiler_layout->addWidget(m_ref_out, 1, 2, 1, 1);
        m_spoiler_layout->addWidget(new QLabel(), 4, 2, 1, 1);
        m_spoiler_layout->setContentsMargins(0, 0, 0, 0);

    m_spoiler = new SpoilerWidget(tr("Parameters:"));
        m_spoiler->setContentLayout(m_spoiler_layout);
        m_spoiler->setAnimationDuration(100);

    m_editor = new BytecodeEditor();


    auto m_layout = new QGridLayout();
        m_layout->addWidget(new QLabel(tr("CRC Models:")), 0, 0, 1, 1, Qt::AlignLeft);
        m_layout->addWidget(m_model_sel, 0, 1, 1, 1);
        m_layout->addWidget(m_set_model_btn, 0, 2, 1, 1, Qt::AlignRight);
        m_layout->addWidget(m_spoiler, 1, 0, 1, 3);
        m_layout->addWidget(m_editor, 2, 0, 1, 3);
        m_layout->addWidget(m_result, 3, 0, 1, 3);
        m_layout->addWidget(m_calc_btn, 4, 0, 1, 3, Qt::AlignRight);
        m_layout->setColumnStretch(1, 1);
        m_layout->setColumnMinimumWidth(0, m_spoiler_layout->columnMinimumWidth(0));

    setLayout(m_layout);
    setWindowModality(Qt::NonModal);
    resize(640, 480);
}

void protodb::ChecksumCalculator::connect_signals()
{
    connect(m_calc_btn, &QPushButton::clicked, this, &ChecksumCalculator::calculate);
    connect(m_set_model_btn, &QPushButton::clicked, this, [this]() {
        auto name = m_model_sel->currentText();
        if (CrcLogic::standartModels().contains(name)) {
            setModel(name);
        }
    });
}

void protodb::ChecksumCalculator::calculate()
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
        m_result->setText(QString("0x%1").arg(QString::number(m_crc.finalize(), 16)));
    }

    return;
}
