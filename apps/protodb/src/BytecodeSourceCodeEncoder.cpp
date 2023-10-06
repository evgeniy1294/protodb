#include "BytecodeSourceCodeEncoder.h"
#include <protodb/utils/JsonBaseUtils.h>

// Qt
#include <QJsonObject>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>

// nlohmann
#include <nlohmann/json.hpp>

// Std
#include <algorithm>
#include <iterator>
#include <cstring>

namespace protodb {

const QString BytecodeSourceCodeEncoder::DefaultVariableName(QStringLiteral("array"));
static const char* VariableNameConfigKey("VariableName");
static const char* DataTypeConfigKey("DataType");
static const char* ElementsPerLineConfigKey("ElementsPerLine");
static const char* UnsignedAsHexadecimalConfigKey("UnsignedAsHexadecimal");

static constexpr  const char* PrimitiveDataTypeName[] = {
    "char",
    "unsigned char",
    "short",
    "unsigned short",
    "int",
    "unsigned int",
    "float",
    "double"
};

static constexpr int SizeOfPrimitiveDataType[] =
{
    sizeof(char),
    sizeof(unsigned char),
    sizeof(short),
    sizeof(unsigned short),
    sizeof(int),
    sizeof(unsigned int),
    sizeof(float),
    sizeof(double)
};

static constexpr int NoOfPrimitiveDataTypes = 8;

inline QString decimalFormattedNumberPlaceHolder() { return QStringLiteral("%1"); }
inline QString hexadecimalFormattedNumberPlaceHolder() { return QStringLiteral("0x%1"); }
const char* const* BytecodeSourceCodeEncoder::dataTypeNames() const { return PrimitiveDataTypeName; }
int BytecodeSourceCodeEncoder::dataTypesCount() const { return NoOfPrimitiveDataTypes; }

void BytecodeSourceCodeEncoder::setVariableName(const QString& name)
{
    if (m_variable_name != name) {
        m_variable_name = name;
        Q_EMIT configChanged();
    }
}

void BytecodeSourceCodeEncoder::setDataType(BytecodeSourceCodeEncoder::PrimitiveDataType type)
{
    if (m_data_type != type) {
        m_data_type = type;
        Q_EMIT configChanged();
    }
}

void BytecodeSourceCodeEncoder::setElementsPerLine(int value)
{
    if (m_elements_per_line != value) {
        m_elements_per_line = value;
        Q_EMIT configChanged();
    }
}

void BytecodeSourceCodeEncoder::setUnsignedAsHexadecimal(bool set)
{
    if (m_unsigned_as_hex != set) {
        m_unsigned_as_hex = set;
        Q_EMIT configChanged();
    }
}

BytecodeSourceCodeEncoder::BytecodeSourceCodeEncoder(QObject* parent)
    : BytecodeEncoderAbstract(tr("name of the encoding target", "C-Array"), QStringLiteral("text/plain"), QString(), parent)
    , m_data_type(DefaultDataType)
    , m_elements_per_line(DefaultElementsPerLine)
    , m_unsigned_as_hex(DefaultUnsignedAsHexadecimal)
    , m_variable_name("array")
{

}

QString BytecodeSourceCodeEncoder::encodeData(const QByteArray& data) const
{
    QString result;

    const int size = data.size();
    const int dataTypeSize = SizeOfPrimitiveDataType[static_cast<int>(m_data_type)];
    const int sizeOfArray = (size + dataTypeSize - 1) / dataTypeSize;

    result.append(QLatin1String("const "));
    result.append(QLatin1String(PrimitiveDataTypeName[static_cast<int>(m_data_type)]));
    result.append(QLatin1Char(' '));
    result.append(m_variable_name);
    result.append(QLatin1String("[%1] = \n{\n").arg(QString::number(sizeOfArray)));

    int elementAddedOnLine = 0;
    for (int i = 0; i < data.size(); i += dataTypeSize) {
        if (elementAddedOnLine == 0) {
            result.append(QLatin1String("   ")); // just 3, one space before every datum
        }
        result.append(QLatin1Char(' '));
        result.append(printFormatted(data, i, data.size() - i));
        if ((i + dataTypeSize) < data.size()) {
            result.append(QLatin1Char(','));
        }

        if (++elementAddedOnLine >= m_elements_per_line) {
            result.append(QLatin1Char('\n'));
            elementAddedOnLine = 0;
        }
    }

    if (elementAddedOnLine > 0) {
        result.append(QLatin1Char('\n'));
    }

    result.append(QLatin1String("};"));
    result.append(QLatin1Char('\n'));

    return result;
}

void BytecodeSourceCodeEncoder::setConfig(const nlohmann::json& cfg)
{
    bool changed = false;

    auto variable_name = cfg.value(VariableNameConfigKey, DefaultVariableName);
    if (m_variable_name != variable_name) {
        m_variable_name = variable_name;
        changed = true;
    }

    auto data_type = static_cast<PrimitiveDataType>( cfg.value(DataTypeConfigKey,
        static_cast<int>(DefaultDataType))
    );
    if (m_data_type != data_type) {
        m_data_type = data_type;
        changed = true;
    }

    auto elements_per_line = cfg.value(ElementsPerLineConfigKey, DefaultElementsPerLine);
    if (m_elements_per_line != elements_per_line) {
        m_elements_per_line = elements_per_line;
        changed = true;
    }

    auto unsigned_as_hex = cfg.value(UnsignedAsHexadecimalConfigKey, DefaultUnsignedAsHexadecimal);
    if (m_unsigned_as_hex != unsigned_as_hex) {
        m_unsigned_as_hex = unsigned_as_hex;
        changed = true;
    }

    if (changed) {
        Q_EMIT configChanged();
    }
}

void BytecodeSourceCodeEncoder::config(nlohmann::json& cfg) const
{
    cfg[VariableNameConfigKey] = m_variable_name;
    cfg[DataTypeConfigKey] =  static_cast<int>(m_data_type);
    cfg[ElementsPerLineConfigKey] = m_elements_per_line;
    cfg[UnsignedAsHexadecimalConfigKey] = m_unsigned_as_hex;
}

QString BytecodeSourceCodeEncoder::printFormatted(const QByteArray& bytes, int offset, int dataSize) const
{
    QString result;
    switch (m_data_type)
    {
        case BytecodeSourceCodeEncoder::PrimitiveDataType::Char:
        {
            constexpr int fieldWidth = 4;

            char e = bytes.at(offset);
            result = decimalFormattedNumberPlaceHolder().arg((int)e, fieldWidth);

        } break;

        case BytecodeSourceCodeEncoder::PrimitiveDataType::UnsignedChar:
        {
            unsigned char e = bytes.at(offset);
            const int base = m_unsigned_as_hex ? 16 : 10;
            const int fieldWidth = m_unsigned_as_hex ? 2 : 3;
            const QString formattedNumberPlaceHolder = m_unsigned_as_hex ?
                                                       hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
            const QChar stuffChar = QLatin1Char(m_unsigned_as_hex ? '0' : ' ');
            result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        } break;

        case BytecodeSourceCodeEncoder::PrimitiveDataType::Short:
        {
            constexpr int fieldWidth = 6;

            short e = 0;
            std::memcpy(&e, bytes.data(), std::min<int>(uint(sizeof(e)), dataSize));

            result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);

        } break;

        case BytecodeSourceCodeEncoder::PrimitiveDataType::UnsignedShort:
        {
            unsigned short e = 0;
            std::memcpy(&e, bytes.data(), std::min<int>(uint(sizeof(e)), dataSize));

            const int base = m_unsigned_as_hex ? 16 : 10;
            const int fieldWidth = m_unsigned_as_hex ? 4 : 5;
            const QString formattedNumberPlaceHolder = m_unsigned_as_hex ?
                                                       hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
            const QChar stuffChar = QLatin1Char(m_unsigned_as_hex ? '0' : ' ');
            result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        } break;

        case BytecodeSourceCodeEncoder::PrimitiveDataType::Integer:
        {
            constexpr int fieldWidth = 11;

            int e = 0;
            std::memcpy(&e, bytes.data(), std::min<int>(uint(sizeof(e)), dataSize));
            result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        } break;

        case BytecodeSourceCodeEncoder::PrimitiveDataType::UnsignedInteger:
        {
            unsigned int e = 0;

            std::memcpy(&e, bytes.data(), std::min<int>(uint(sizeof(e)), dataSize));
            const int base = m_unsigned_as_hex ? 16 : 10;
            const int fieldWidth = m_unsigned_as_hex ? 8 : 10;
            const QString formattedNumberPlaceHolder = m_unsigned_as_hex ?
                                                       hexadecimalFormattedNumberPlaceHolder() : decimalFormattedNumberPlaceHolder();
            const QChar stuffChar = QLatin1Char(m_unsigned_as_hex ? '0' : ' ');
            result = formattedNumberPlaceHolder.arg(e, fieldWidth, base, stuffChar);
        } break;

        case BytecodeSourceCodeEncoder::PrimitiveDataType::Float:
        {
            constexpr int fieldWidth = 13;

            float e = 0;
            std::memcpy(&e, bytes.data(), std::min<int>(uint(sizeof(e)), dataSize));
            result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        } break;

        case BytecodeSourceCodeEncoder::PrimitiveDataType::Double:
        {
            constexpr int fieldWidth = 13;

            double e = 0;
            std::memcpy(&e, bytes.data(), std::min<int>(uint(sizeof(e)), dataSize));
            result = decimalFormattedNumberPlaceHolder().arg(e, fieldWidth);
        } break;

        case BytecodeSourceCodeEncoder::PrimitiveDataType::_Count: {
        // dummy entry to avoid compiler warning -Wswitch, can this be avoided?

        }  break;
    }

        return result;
}

BytecodeSourceCodeEncoderConfigDialog::BytecodeSourceCodeEncoderConfigDialog(QWidget* parent)
    : ConfigWidgetAbstract(parent)
{
    createGui();
    connectSignals();
}

void BytecodeSourceCodeEncoderConfigDialog::setConfig(const nlohmann::json& cfg)
{
    bool changed = false;

    auto variable_name = cfg.value(VariableNameConfigKey, BytecodeSourceCodeEncoder::DefaultVariableName);
    if (m_variable_name->text() != variable_name) {
            m_variable_name->setText(variable_name);
            changed = true;
    }

    auto data_type = cfg.value(DataTypeConfigKey, static_cast<int>(BytecodeSourceCodeEncoder::DefaultDataType));
    if (m_data_type->currentData().toInt() != data_type) {
        for(int i = 0; i < m_data_type->count(); i++) {
            if (m_data_type->itemData(i) == data_type) {
                m_data_type->setCurrentIndex(i);
            }
        }
        changed = true;
    }

    auto elements_per_line = cfg.value(ElementsPerLineConfigKey, BytecodeSourceCodeEncoder::DefaultElementsPerLine);
    if (m_elements_per_line->value() != elements_per_line) {
            m_elements_per_line->setValue(elements_per_line);
            changed = true;
    }

    auto unsigned_as_hex = cfg.value(UnsignedAsHexadecimalConfigKey, BytecodeSourceCodeEncoder::DefaultUnsignedAsHexadecimal);
    if (m_unsigned_as_hex->isChecked() != unsigned_as_hex) {
            m_unsigned_as_hex->setChecked(unsigned_as_hex);
            changed = true;
    }

    if (changed) {
            Q_EMIT sConfigChanged();
    }
}

void BytecodeSourceCodeEncoderConfigDialog::config(nlohmann::json& cfg) const
{
    cfg[VariableNameConfigKey] = m_variable_name->text();
    cfg[DataTypeConfigKey] =  static_cast<int>(m_data_type->currentData().toInt());
    cfg[ElementsPerLineConfigKey] = m_elements_per_line->value();
    cfg[UnsignedAsHexadecimalConfigKey] = m_unsigned_as_hex->isChecked();
}

void BytecodeSourceCodeEncoderConfigDialog::createGui()
{
    m_data_type = new QComboBox();
    for (int i = 0; i < NoOfPrimitiveDataTypes; i++) {
        m_data_type->addItem(PrimitiveDataTypeName[i], i);
    }

    for(int i = 0; i < m_data_type->count(); i++) {
        if (m_data_type->itemData(i).toInt() == static_cast<int>(BytecodeSourceCodeEncoder::DefaultDataType)) {
            m_data_type->setCurrentIndex(i);
        }
    }

    m_unsigned_as_hex = new QCheckBox(tr("Unsigned as hexademical"));
        m_unsigned_as_hex->setChecked(BytecodeSourceCodeEncoder::DefaultUnsignedAsHexadecimal);

    m_elements_per_line = new QSpinBox();
        m_elements_per_line->setMinimum(1);
        m_elements_per_line->setMaximum(256);

    m_variable_name = new QLineEdit(BytecodeSourceCodeEncoder::DefaultVariableName);

    auto m_layout = new QGridLayout();
        m_layout->addWidget(new QLabel(tr("Array name")), 0, 0);
        m_layout->addWidget(m_variable_name, 0, 1);
        m_layout->addWidget(new QLabel(tr("Elements per line")), 1, 0);
        m_layout->addWidget(m_elements_per_line, 1, 1);
        m_layout->addWidget(new QLabel(tr("Data type")), 2, 0);
        m_layout->addWidget(m_data_type, 2, 1);
        m_layout->addWidget(new QLabel(), 3, 0, 1, 2);
        m_layout->addWidget(m_unsigned_as_hex, 4, 0, 1, 2);

    setLayout(m_layout);
}

void BytecodeSourceCodeEncoderConfigDialog::connectSignals()
{
    connect(m_variable_name, &QLineEdit::editingFinished, this, [this]() {
        Q_EMIT sConfigChanged();
    });

    connect(m_elements_per_line, QOverload<int>::of(&QSpinBox::valueChanged), this, [this]() {
        Q_EMIT sConfigChanged();
    });

    connect(m_unsigned_as_hex, &QCheckBox::stateChanged, this, [this]() {
        Q_EMIT sConfigChanged();
    });

    connect(m_data_type, &QComboBox::currentTextChanged, this, [this]() {
        Q_EMIT sConfigChanged();
    });
}

} // namespace protodb

