#pragma once

#include "BytecodeSourceCodeEncoder.h"

#include <protodb/creators/BytecodeEncoderCreator.h>

namespace protodb {

class BytecodeSourceCodeEncoderCreator final: public BytecodeEncoderCreator
{
public:
    explicit BytecodeSourceCodeEncoderCreator() = default;
    ~BytecodeSourceCodeEncoderCreator() override = default;

    static QString creatorId() { return QString("ByteArraySourceCodeEncoderCreator"); }

    QString cid() const override { return creatorId(); };
    QString name() const override { return QObject::tr("C-array"); };
    QString description() const override { return QObject::tr("Encode bytes as C-array"); };

    BytecodeEncoderAbstract* createEncoder(QObject* parent = nullptr) const override {
        return new BytecodeSourceCodeEncoder();
    }

    BytecodeEncoderAbstract* createEncoder(const nlohmann::json& cfg, QObject* parent = nullptr) const override {
        auto ret = new BytecodeSourceCodeEncoder();
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

