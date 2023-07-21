#include "protodb/NetworkConnectionCreator.h"
#include "protodb/NetworkConnection.h"
#include <protodb/utils/JsonBaseUtils.h>
#include <nlohmann/json.hpp>

using namespace protodb;

QString NetworkConnectionCreator::cid() const
{
    return creatorId();
}

QString NetworkConnectionCreator::name() const
{
    return QString(QObject::tr("NetworkConnection Creator"));
}

QString NetworkConnectionCreator::description() const
{
    return QString(QObject::tr("Creator for NetworkConnection"));
}

QString NetworkConnectionCreator::iconName() const {
    return QString();
}

Connection* NetworkConnectionCreator::create(QObject* parent) const
{
    auto connection = new NetworkConnection(parent);
    return connection;
}

Connection* NetworkConnectionCreator::create(const nlohmann::json& cfg, QObject* parent) const
{
    auto connection = new NetworkConnection(parent);
    if (!cfg.is_null())
        connection->setConfig(cfg);

    return connection;
}
