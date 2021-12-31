#pragma once

#include <nlohmann/json.hpp>
#include <QIODevice>
#include "CreatorAbstract.h"

class IODeviceCreatorInterface: public CreatorAbstract
{
public:
    virtual ~IODeviceCreatorInterface() override = default;

    virtual QIODevice* create() const = 0;
    virtual QIODevice* create(const nlohmann::json&) const = 0;
};
