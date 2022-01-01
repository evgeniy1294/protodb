#pragma once

#include <nlohmann/json.hpp>
#include "CreatorAbstract.h"

class QIODevice;

class IODeviceCreatorInterface: public CreatorAbstract
{
public:
    virtual ~IODeviceCreatorInterface() override = default;

    virtual QIODevice* create() const = 0;
    virtual QIODevice* create(const nlohmann::json&) const = 0;
};

Q_DECLARE_INTERFACE(IODeviceCreatorInterface, "ProtoDb.IODeviceCreatorInterface")
