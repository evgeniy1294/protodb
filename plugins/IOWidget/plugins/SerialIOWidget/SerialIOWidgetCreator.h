#pragma once

#include <IOWidgetCreatorInterface.h>
#include "SerialIOWidget.h"

class SerialIOWIdgetCreator: public QObject, public IOWidgetCreatorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ProtoDb.IOWidgetCreatorInterface")
    Q_INTERFACES(IOWidgetCreatorInterface)

public:
    explicit SerialIOWIdgetCreator() = default;
    ~SerialIOWIdgetCreator() override = default;

    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    IOWidget* create() const override;
    IOWidget* create(const nlohmann::json& json) const override;
};
