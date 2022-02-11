#pragma once

#include <QIODevice>
#include <protodb/plugins/IODeviceCreatorInterface.h>

class SerialIODeviceCreator: public QObject, public IODeviceCreatorInterface
{
    Q_OBJECT
    Q_INTERFACES(IODeviceCreatorInterface)
    Q_PLUGIN_METADATA(IID "ProtoDb.SerialIODeviceCreator" FILE "SerialIODevicePluginMetadata.json")

public:
    explicit SerialIODeviceCreator() = default;
    ~SerialIODeviceCreator() override = default;

    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    QIODevice* create() const override;
    QIODevice* create(const nlohmann::json& json) const override;
};
