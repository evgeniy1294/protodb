#pragma once

#include <protodb/creators/ConnectionWidgetCreator.h>

namespace protodb {

class NetworkConnectionWidgetCreator final: public QObject, public ConnectionWidgetCreator
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "protodb.NetworkConnectionWidgetCreatorInterface" FILE "NetworkConnectionWidgetPluginMetadata.json")
    Q_INTERFACES(protodb::ConnectionWidgetCreator)

public:
    explicit NetworkConnectionWidgetCreator() = default;
    ~NetworkConnectionWidgetCreator() override = default;

    static QString creatorId() { return QString("NetworkWidgetCreator"); }
    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    ConnectionConfigWidget* createConfigWidget() const override;
    ConnectionConfigWidget* createConfigWidget(const nlohmann::json& json) const override;

    ConnectionAdvanceControlWidget* createAdvanceControlWidget() const override;
};

} // namespace protodb
