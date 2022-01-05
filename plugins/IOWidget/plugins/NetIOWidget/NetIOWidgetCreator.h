#pragma once

#include <IOWidgetCreatorInterface.h>

class NetIOWIdgetCreator: public QObject, public IOWidgetCreatorInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ProtoDb.IOWidgetCreatorInterface")
    Q_INTERFACES(IOWidgetCreatorInterface)

public:
    explicit NetIOWIdgetCreator() = default;
    ~NetIOWIdgetCreator() override = default;

    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    IOWidget* create() const override;
    IOWidget* create(const nlohmann::json& json) const override;
};
