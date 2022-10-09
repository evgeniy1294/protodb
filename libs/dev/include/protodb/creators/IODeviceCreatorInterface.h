#pragma once

#include <protodb/factories/CreatorAbstract.h>
#include <nlohmann/json.hpp>

class QIODevice;

class IODeviceCreator: public CreatorAbstract
{
public:
    virtual ~IODeviceCreator() override = default;

    virtual QIODevice* create() const = 0;
    virtual QIODevice* create(const nlohmann::json&) const = 0;
};

Q_DECLARE_INTERFACE(IODeviceCreator, "ProtoDb.IODeviceCreatorInterface")
