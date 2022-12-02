#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/CreatorAbstract.h>
#include <nlohmann/json.hpp>

class QIODevice;

namespace protodb
{

class PROTODB_DEV_EXPORT IODeviceCreator: public CreatorAbstract
{
public:
    virtual ~IODeviceCreator() override = default;

    virtual QIODevice* create() const = 0;
    virtual QIODevice* create(const nlohmann::json&) const = 0;
};

} // namespace protodb

Q_DECLARE_INTERFACE(protodb::IODeviceCreator, "ProtoDb.IODeviceCreatorInterface")
