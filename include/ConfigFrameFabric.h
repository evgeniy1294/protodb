#pragma once

#include <nlohmann/json.hpp>
#include "FabricAbstract.h"

class ConfigFrameCreator;
class ConfigFrame;

class ConfigFrameFabric final: public FabricAbstract
{
    Q_OBJECT

public:

    ConfigFrameFabric(QObject* parent = nullptr);

    const QString& fid() const override;
    bool isCompatible(CreatorAbstract* creator) const override;
    ConfigFrame* create(const QString& cid, const nlohmann::json& config) const;
};


