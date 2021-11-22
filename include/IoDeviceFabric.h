#pragma once

#include <nlohmann/json.hpp>
#include "FabricAbstract.h"

class IoDeviceCreator;
class QIODevice;

class IoDeviceFabric final: public FabricAbstract
{
    Q_OBJECT

public:

    IoDeviceFabric(QObject* parent = nullptr);

    const QString& fid() const override;
    bool isCompatible(CreatorAbstract* creator) const override;
    QIODevice* create(const QString& cid, const nlohmann::json& config) const;
};

