#include "BytecodeValuesEncoder.h"

#include <Okteta/ValueCodec.hpp>

#include <protodb/utils/JsonBaseUtils.h>
#include <nlohmann/json.hpp>

#include <QComboBox>
#include <QVBoxLayout>
#include <QLineEdit>

namespace protodb {

static const char* DefaultSeparator(" ");
static const char* SeparatorConfigKey("Separator");
static const char* ValueConfigKey("ValueCoding");

BytecodeValuesEncoder::BytecodeValuesEncoder(QObject* parent)
    : BytecodeEncoderAbstract(tr("name of the encoding target", "Values"), QStringLiteral("text/plain"), QString(), parent)
    , m_value_coding(Okteta::HexadecimalCoding)
    , m_separation(DefaultSeparator)
{

}

QString BytecodeValuesEncoder::encodeData(const QByteArray& data) const
{
    QString result;

    // encode
    Okteta::ValueCodec* valueCodec = Okteta::ValueCodec::createCodec(m_value_coding);

    // prepare
    QString valueString;
    valueString.resize(valueCodec->encodingWidth());

    for (size_t i = 0; i < data.size(); i++) {
        if (i > 0) {
            result.append(m_separation);
        }

        valueCodec->encode(&valueString, 0, data.at(i));

        result.append(valueString);
    }

    // clean up
    delete valueCodec;

    return result;
}

void BytecodeValuesEncoder::setConfig(const nlohmann::json& cfg)
{
    bool changed = false;

    auto separation  = cfg.value(SeparatorConfigKey, QString(" "));
    if (separation != m_separation) {
        m_separation = separation;
        changed = true;
    }

    auto value_coding = static_cast<Okteta::ValueCoding>(cfg.value(ValueConfigKey, static_cast<int>(Okteta::ValueCoding::HexadecimalCoding)));
    if (value_coding != m_value_coding) {
        m_value_coding = value_coding;
        changed = true;
    }

    if (changed) {
        Q_EMIT configChanged();
    }
}

void BytecodeValuesEncoder::config(nlohmann::json& cfg) const
{
    cfg[SeparatorConfigKey] = m_separation;
    cfg[ValueConfigKey] = static_cast<int>(m_value_coding);
}

void BytecodeValuesEncoder::setSeparation(const QString& s)
{
    if (m_separation != s) {
        m_separation = s;
        Q_EMIT configChanged();
    }
}

QString BytecodeValuesEncoder::separaion() const
{
    return m_separation;
}

void BytecodeValuesEncoder::setValueCoding(Okteta::ValueCoding valueCoding)
{
    if (m_value_coding != valueCoding) {
        m_value_coding = valueCoding;
        Q_EMIT configChanged();
    }
}

Okteta::ValueCoding BytecodeValuesEncoder::valueCoding() const
{
    return m_value_coding;
}

BytecodeValuesEncoderConfigWidget::BytecodeValuesEncoderConfigWidget(QWidget* parent)
    : ConfigWidgetAbstract(parent)
{
    createGui();
    connectSignals();
}

void BytecodeValuesEncoderConfigWidget::createGui()
{
    m_separation = new QLineEdit(DefaultSeparator);
    m_coding = new QComboBox();
        m_coding->addItem(tr("Hexadecimal"), static_cast<int>(Okteta::HexadecimalCoding));
        m_coding->addItem(tr("Decimal"), static_cast<int>(Okteta::DecimalCoding));
        m_coding->addItem(tr("Octal"), static_cast<int>(Okteta::OctalCoding));
        m_coding->addItem(tr("Binary"), static_cast<int>(Okteta::BinaryCoding));

    auto m_layout = new QVBoxLayout();
    m_layout->addWidget(m_coding);
    m_layout->addWidget(m_separation);

    setLayout(m_layout);
}

void BytecodeValuesEncoderConfigWidget::connectSignals()
{
    connect(m_separation, &QLineEdit::editingFinished, this, [this]() {
        Q_EMIT sConfigChanged();
    });

    connect(m_coding, &QComboBox::currentTextChanged, this, [this]() {
        Q_EMIT sConfigChanged();
    });
}

void BytecodeValuesEncoderConfigWidget::setConfig(const nlohmann::json& cfg)
{
    bool changed = false;

    auto separation  = cfg.value(SeparatorConfigKey, QString(" "));
    if (separation != m_separation->text()) {
        m_separation->setText(separation);
        changed = true;
    }

    auto value_coding = static_cast<Okteta::ValueCoding>(cfg.value(ValueConfigKey, static_cast<int>(Okteta::ValueCoding::HexadecimalCoding)));
    if (value_coding != m_coding->currentData().toInt()) {
        m_coding->setCurrentText("");
        changed = true;
    }

    if (changed) {
        Q_EMIT sConfigChanged();
    }
}

void BytecodeValuesEncoderConfigWidget::config(nlohmann::json& cfg) const
{
    cfg[SeparatorConfigKey] = m_separation->text();
    cfg[ValueConfigKey] = m_coding->currentData().toInt();
}

} // namespace protodb
