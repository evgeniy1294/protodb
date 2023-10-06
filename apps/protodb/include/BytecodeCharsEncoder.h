#pragma once

#include <protodb/BytecodeEncoderAbstract.h>
#include <protodb/ConfigWidgetAbstract.h>

#include <Okteta/OktetaCore.hpp>

#include <nlohmann/json_fwd.hpp>

namespace protodb {

class BytecodeCharsEncoder final: public BytecodeEncoderAbstract
{
    Q_OBJECT

public:
    static const QString CodecNameConfigKey;
    static const QString UndefinedCharConfigKey;
    static const QString SubstituteCharConfigKey;

    BytecodeCharsEncoder(QObject* parent = nullptr);
   ~BytecodeCharsEncoder() = default;

    QString encodeData(const QByteArray& data) const override;

    void setConfig(const nlohmann::json& cfg) override;
    void config(nlohmann::json& cfg) const override;

    void setCodecName(const QString& codec);
    QString codecName() const;

    void setUndefinedChar(QChar undefinedChar);
    QChar undefinedChar() const;

    void setSubstituteChar(QChar substituteChar);
    QChar substituteChar() const;

private:
    QString m_codec_name;
    QChar m_undefined_char;
    QChar m_substitute_char;
};

} // namespace protodb
