#pragma once

#include <nlohmann/json.hpp>
#include "ConfigFrame.h"
#include "CreatorAbstract.h"

class ConfigFrameCreator: public CreatorAbstract
{
public:
    virtual ConfigFrame* create(const nlohmann::json& config) const = 0;
};


