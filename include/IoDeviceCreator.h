#pragma once

#include <nlohmann/json.hpp>
#include <QIODevice>
#include "CreatorAbstract.h"

class IoDeviceCreator: public CreatorAbstract
{
public:
    virtual QIODevice* create(const nlohmann::json&) const = 0;
};

