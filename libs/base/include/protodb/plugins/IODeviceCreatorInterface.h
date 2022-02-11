#pragma once

#include "factories/CreatorAbstract.h"
#include <nlohmann/json.hpp>

class QIODevice;

class IODeviceCreatorInterface: public CreatorAbstract
{
public:
    virtual ~IODeviceCreatorInterface() override = default;

    virtual QIODevice* create() const = 0;
    virtual QIODevice* create(const nlohmann::json&) const = 0;
};

Q_DECLARE_INTERFACE(IODeviceCreatorInterface, "ProtoDb.IODeviceCreatorInterface")
