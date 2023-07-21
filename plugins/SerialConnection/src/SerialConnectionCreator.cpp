#include "protodb/SerialConnectionCreator.h"
#include "protodb/SerialConnection.h"

#include <protodb/utils/JsonBaseUtils.h>

#include <nlohmann/json.hpp>

#include <QSerialPort>

using namespace protodb;

QString SerialConnectionCreator::cid() const
{
    return creatorId();
}

QString SerialConnectionCreator::name() const
{
    return QString(QObject::tr("SerialConnection Creator"));
}

QString SerialConnectionCreator::description() const
{
    return QString(QObject::tr("Creator for serial io seance"));
}

QString SerialConnectionCreator::iconName() const {
    return QString();
}

Connection* SerialConnectionCreator::create(QObject* parent) const
{
    return new SerialConnection(parent);
}

Connection* SerialConnectionCreator::create(const nlohmann::json& cfg, QObject* parent) const
{
    if (cfg.is_null())
        return create(parent);

    auto ret = new SerialConnection(parent);
        ret->setConfig(cfg);

    return ret;
}
