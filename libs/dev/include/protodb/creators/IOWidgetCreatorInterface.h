#pragma once

#include <protodb/factories/CreatorAbstract.h>

#include <QtPlugin>
#include <nlohmann/json.hpp>

class IOWidget;

class IOWidgetCreator: public CreatorAbstract
{
public:
    virtual ~IOWidgetCreator() override = default;

    virtual IOWidget* create() const = 0;
    virtual IOWidget* create(const nlohmann::json&) const = 0;
};

Q_DECLARE_INTERFACE(IOWidgetCreator, "ProtoDb.IOWidgetCreatorInterface")
