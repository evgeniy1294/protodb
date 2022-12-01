#pragma once

#include <protodb/creators/IOWidgetCreatorInterface.h>

namespace protodb {

class NetIOWIdgetCreator: public QObject, public IOWidgetCreator
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "ProtoDb.NetIOWidgetCreatorInterface" FILE "NetIOWidgetPluginMetadata.json")
    Q_INTERFACES(protodb::IOWidgetCreator)

public:
    explicit NetIOWIdgetCreator() = default;
    ~NetIOWIdgetCreator() override = default;

    static QString creatorId() { return QString("NetIOWidgetCreator"); }
    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    IOWidget* create() const override;
    IOWidget* create(const nlohmann::json& json) const override;
};

} // namespace protodb
