#include "protodb/SerialIODeviceCreator.h"
#include "protodb/SerialIoSeance.h"

#include <protodb/utils/JsonBaseUtils.h>

#include <nlohmann/json.hpp>

#include <QSerialPort>

using namespace protodb;

QString SerialIoSeanceCreator::cid() const
{
    return creatorId();
}

QString SerialIoSeanceCreator::name() const
{
    return QString(QObject::tr("SerialIoSeance Creator"));
}

QString SerialIoSeanceCreator::description() const
{
    return QString(QObject::tr("Creator for serial io seance"));
}

QString SerialIoSeanceCreator::iconName() const {
    return QString();
}

Seance* SerialIoSeanceCreator::create(QObject* parent) const
{
    return new SerialIoSeance(parent);
}

Seance* SerialIoSeanceCreator::create(const nlohmann::json& cfg, QObject* parent) const
{
    if (cfg.is_null())
        return create(parent);

    auto ret = new SerialIoSeance(parent);
        ret->setConfig(cfg);

    return ret;
}
