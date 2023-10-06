#pragma once

#include "BytecodeValuesEncoder.h"

#include <protodb/creators/BytecodeEncoderCreator.h>

namespace protodb {

class BytecodeValuesEncoderCreator final: public BytecodeEncoderCreator
{
public:
    explicit BytecodeValuesEncoderCreator() = default;
    ~BytecodeValuesEncoderCreator() override = default;

    static QString creatorId() { return QString("ByteArrayValuesEncoderCreator"); }

    QString cid() const override { return creatorId(); };
    QString name() const override { return QObject::tr("Values"); };
    QString description() const override { return QObject::tr("Encode bytes as values"); };

    BytecodeEncoderAbstract* createEncoder(QObject* parent = nullptr) const override {
        return new BytecodeValuesEncoder();
    }

    BytecodeEncoderAbstract* createEncoder(const nlohmann::json& cfg, QObject* parent = nullptr) const override {
        auto ret = new BytecodeValuesEncoder();
        ret->setConfig(cfg);

        return ret;
    }


    ConfigWidgetAbstract* createConfigWidget(QWidget* parent = nullptr) const override {
        return nullptr;
    }

    ConfigWidgetAbstract* createConfigWidget(const nlohmann::json&, QWidget* parent = nullptr) const override {
        return nullptr;
    };
};


} // namespace protodb
