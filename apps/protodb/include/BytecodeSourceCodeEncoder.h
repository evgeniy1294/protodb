#pragma once

#include <protodb/BytecodeEncoderAbstract.h>
#include <protodb/ConfigWidgetAbstract.h>

#include <QDialog>
#include <QString>

#include <nlohmann/json_fwd.hpp>

class QComboBox;
class QCheckBox;
class QSpinBox;
class QLineEdit;

namespace protodb {

class BytecodeSourceCodeEncoder : public BytecodeEncoderAbstract
{
    Q_OBJECT

public:
    enum class PrimitiveDataType
    {
        Char = 0,
        UnsignedChar,
        Short,
        UnsignedShort,
        Integer,
        UnsignedInteger,
        Float,
        Double,
        _Count,
    };

public:
    static const QString DefaultVariableName;
    static inline constexpr PrimitiveDataType DefaultDataType = PrimitiveDataType::UnsignedChar;
    static inline constexpr int DefaultElementsPerLine = 4;
    static inline constexpr bool DefaultUnsignedAsHexadecimal = true;

public:
    BytecodeSourceCodeEncoder(QObject* parent = nullptr);
   ~BytecodeSourceCodeEncoder() override = default;

    QString encodeData(const QByteArray& data) const override;

    void setConfig(const nlohmann::json& cfg) override;
    void config(nlohmann::json& cfg) const override;

    void setVariableName(const QString& name);
    QString variableName() const;

    void setDataType(BytecodeSourceCodeEncoder::PrimitiveDataType type);
    BytecodeSourceCodeEncoder::PrimitiveDataType dataType() const;

    void setElementsPerLine(int value);
    int elementsPerLine() const;

    void setUnsignedAsHexadecimal(bool set = true);
    bool unsignedAsHexadecimal() const;

    const char* const* dataTypeNames() const;
    int dataTypesCount() const;

private:
    QString printFormatted(const QByteArray& data, int offset, int dataSize) const;

private:
    QString m_variable_name;
    PrimitiveDataType m_data_type;
    int m_elements_per_line;
    bool m_unsigned_as_hex;
};

inline QString BytecodeSourceCodeEncoder::variableName() const { return m_variable_name; }
inline BytecodeSourceCodeEncoder::PrimitiveDataType BytecodeSourceCodeEncoder::dataType() const { return m_data_type; }
inline int BytecodeSourceCodeEncoder::elementsPerLine() const { return m_elements_per_line; }
inline bool BytecodeSourceCodeEncoder::unsignedAsHexadecimal() const { return m_unsigned_as_hex; }

class BytecodeSourceCodeEncoderConfigDialog final: public ConfigWidgetAbstract
{
    Q_OBJECT

public:
    BytecodeSourceCodeEncoderConfigDialog(QWidget* parent = nullptr);
   ~BytecodeSourceCodeEncoderConfigDialog() = default;

    void setConfig(const nlohmann::json& cfg) override;
    void config(nlohmann::json& cfg) const override;

private:
    void createGui();
    void connectSignals();

private:
    QComboBox* m_data_type;
    QCheckBox* m_unsigned_as_hex;
    QSpinBox*  m_elements_per_line;
    QLineEdit* m_variable_name;
};

} // namespace protodb

