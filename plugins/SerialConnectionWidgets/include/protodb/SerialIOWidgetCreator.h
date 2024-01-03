#pragma once

#include <protodb/creators/ConnectionWidgetCreator.h>

namespace protodb {

class SerialConnectionWidgetCreator: public QObject, public ConnectionWidgetCreator
{
    Q_OBJECT
    Q_INTERFACES(protodb::ConnectionWidgetCreator)
    Q_PLUGIN_METADATA(IID "protodb.SerialConnectionWidgetCreator" FILE "SerialConnectionWidgetPluginMetadata.json")

public:
    explicit SerialConnectionWidgetCreator() = default;
    ~SerialConnectionWidgetCreator() override = default;

    static QString creatorId() { return QString("SerialConnectionCreator"); }
    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    ConnectionConfigWidget* createConfigWidget() const override;
    ConnectionConfigWidget* createConfigWidget(const nlohmann::json& json) const override;

    ConnectionAdvanceControlWidget* createAdvanceControlWidget() const override;
};

} // namespace protodb
