#pragma once

#include "BytecodeCharsEncoder.h"

#include <protodb/creators/BytecodeEncoderCreator.h>

namespace protodb {

class BytecodeCharsEncoderCreator final: public BytecodeEncoderCreator
{
public:
    explicit BytecodeCharsEncoderCreator() = default;
   ~BytecodeCharsEncoderCreator() override = default;

    static QString creatorId() { return QString("ByteArrayCharsEncoderCreator"); }

    QString cid() const override { return creatorId(); };
    QString name() const override { return QObject::tr("Chars"); };
    QString description() const override { return QObject::tr("Encode bytes as chars"); };

    BytecodeEncoderAbstract* createEncoder(QObject* parent = nullptr) const override {
        return new BytecodeCharsEncoder();
    }

    BytecodeEncoderAbstract* createEncoder(const nlohmann::json& cfg, QObject* parent = nullptr) const override {
        auto ret = new BytecodeCharsEncoder();
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
