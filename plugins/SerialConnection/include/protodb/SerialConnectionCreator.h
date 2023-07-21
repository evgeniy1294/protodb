#pragma once

#include <protodb/creators/ConnectionCreator.h>

namespace protodb {

class Connection;

class SerialConnectionCreator: public QObject, public ConnectionCreator
{
    Q_OBJECT
    Q_INTERFACES(protodb::ConnectionCreator)
    Q_PLUGIN_METADATA(IID "protodb.SerialConnection" FILE "SerialConnectionPluginMetadata.json")

public:
    explicit SerialConnectionCreator() = default;
    ~SerialConnectionCreator() override = default;

    static QString creatorId() { return QString("SerialConnectionCreator"); }
    QString cid() const override;
    QString name() const override;
    QString description() const override;

    QString iconName() const override;

    Connection* create(QObject* parent = nullptr) const override;
    Connection* create(const nlohmann::json& cfg, QObject* parent = nullptr) const override;
};

} // namespace protodb
