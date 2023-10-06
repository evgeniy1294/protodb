#pragma once

#include <protodb/BytecodeEncoderAbstract.h>
#include <protodb/ConfigWidgetAbstract.h>

#include <Okteta/OktetaCore.hpp>

#include <nlohmann/json_fwd.hpp>

class QComboBox;
class QLineEdit;

namespace protodb {

class BytecodeValuesEncoder final: public BytecodeEncoderAbstract
{
    Q_OBJECT

public:
    BytecodeValuesEncoder(QObject* parent = nullptr);
   ~BytecodeValuesEncoder() override = default;

    QString encodeData(const QByteArray& data) const override;

    void setConfig(const nlohmann::json& cfg) override;
    void config(nlohmann::json& cfg) const override;

    void setSeparation(const QString& s);
    QString separaion() const;

    void setValueCoding(Okteta::ValueCoding valueCoding);
    Okteta::ValueCoding valueCoding() const;

private:
    Okteta::ValueCoding m_value_coding;
    QString m_separation;
};

class BytecodeValuesEncoderConfigWidget final: public ConfigWidgetAbstract
{
    Q_OBJECT

public:
    BytecodeValuesEncoderConfigWidget(QWidget* parent = nullptr);
   ~BytecodeValuesEncoderConfigWidget() = default;

    void setConfig(const nlohmann::json& cfg) override;
    void config(nlohmann::json& cfg) const override;

private:
    void createGui();
    void connectSignals();

private:
    QLineEdit* m_separation;
    QComboBox* m_coding;
};

} // namespace protodb
