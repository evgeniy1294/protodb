#pragma once

#include <nlohmann/json.hpp>
#include "CreatorAbstract.h"

class IOWidget;

class IOWidgetCreatorInterface: public CreatorAbstract
{
public:
    virtual ~IOWidgetCreatorInterface() override = default;

    virtual IOWidget* create() const = 0;
    virtual IOWidget* create(const nlohmann::json&) const = 0;
};

Q_DECLARE_INTERFACE(IOWidgetCreatorInterface, "ProtoDb.IOWidgetCreatorInterface")
