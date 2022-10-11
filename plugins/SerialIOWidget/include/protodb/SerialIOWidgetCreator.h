#pragma once

#include <protodb/creators/IOWidgetCreatorInterface.h>

class SerialIOWIdgetCreator: public QObject, public IOWidgetCreator
{
    Q_OBJECT
    Q_INTERFACES(IOWidgetCreator)
    Q_PLUGIN_METADATA(IID "ProtoDb.SerialIOWidgetCreator" FILE "SerialIOWidgetPluginMetadata.json")

public:
    explicit SerialIOWIdgetCreator() = default;
    ~SerialIOWIdgetCreator() override = default;

    static QString creatorId() { return QString("SerialIOWidgetCreator"); }
    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    IOWidget* create() const override;
    IOWidget* create(const nlohmann::json& json) const override;
};
