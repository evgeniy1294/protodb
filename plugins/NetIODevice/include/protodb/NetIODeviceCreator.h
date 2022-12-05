#pragma once

#include <protodb/creators/IODeviceCreatorInterface.h>

#include <QIODevice>

namespace protodb {

class NetIODeviceCreator: public QObject, public IODeviceCreator
{
    Q_OBJECT
    Q_INTERFACES(protodb::IODeviceCreator)
    Q_PLUGIN_METADATA(IID "protodb.NetIODeviceCreator" FILE "NetIODevicePluginMetadata.json")

public:
    explicit NetIODeviceCreator() = default;
    ~NetIODeviceCreator() override = default;

    static QString creatorId() { return QString("NetIODeviceCreator"); }
    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    QIODevice* create() const override;
    QIODevice* create(const nlohmann::json& json) const override;
};

} // namespace protodb
