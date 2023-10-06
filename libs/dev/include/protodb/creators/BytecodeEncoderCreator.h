#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/CreatorAbstract.h>

#include <nlohmann/json_fwd.hpp>

namespace protodb {

class BytecodeEncoderAbstract;
class ConfigWidgetAbstract;

class PROTODB_DEV_EXPORT BytecodeEncoderCreator: public CreatorAbstract
{
public:
    virtual ~BytecodeEncoderCreator() = default;

    virtual BytecodeEncoderAbstract* createEncoder(QObject* parent = nullptr) const = 0;
    virtual BytecodeEncoderAbstract* createEncoder(const nlohmann::json&, QObject* parent = nullptr) const = 0;

    virtual ConfigWidgetAbstract* createConfigWidget(QWidget* parent = nullptr) const = 0;
    virtual ConfigWidgetAbstract* createConfigWidget(const nlohmann::json&, QWidget* parent = nullptr) const = 0;
};


} // namespace protodb

Q_DECLARE_INTERFACE(protodb::BytecodeEncoderCreator, "ProtoDb.ByteArrayEncoderInterface")
