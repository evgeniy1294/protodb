#pragma once

#include <protodb/creators/IODeviceCreatorInterface.h>

class QIODevice;

namespace protodb {

class NetIODeviceCreator: public QObject, public SeanceCreator
{
    Q_OBJECT
    Q_INTERFACES(protodb::SeanceCreator)
    Q_PLUGIN_METADATA(IID "protodb.NetworkSeanceCreator" FILE "NetworkSeancePluginMetadata.json")

public:
    explicit NetIODeviceCreator() = default;
    ~NetIODeviceCreator() override = default;

    static QString creatorId() { return QString("NetworkSeanceCreator"); }
    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    Seance* create(QObject* parent = nullptr) const override;
    Seance* create(const nlohmann::json& cfg, QObject* parent = nullptr) const override;
};

} // namespace protodb
