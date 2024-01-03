#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/CreatorAbstract.h>

#include <QtPlugin>
#include <nlohmann/json_fwd.hpp>

namespace protodb {

class ConnectionConfigWidget;
class ConnectionAdvanceControlWidget;

class PROTODB_DEV_EXPORT ConnectionWidgetCreator: public CreatorAbstract
{
public:
    virtual ~ConnectionWidgetCreator() override = default;

    virtual ConnectionConfigWidget* createConfigWidget() const = 0;
    virtual ConnectionConfigWidget* createConfigWidget(const nlohmann::json&) const = 0;

    virtual ConnectionAdvanceControlWidget* createAdvanceControlWidget() const = 0;
};

} // namespace protodb

Q_DECLARE_INTERFACE(protodb::ConnectionWidgetCreator, "ProtoDb.IOWidgetCreatorInterface")
