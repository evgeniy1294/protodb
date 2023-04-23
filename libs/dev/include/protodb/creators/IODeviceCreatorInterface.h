#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/CreatorAbstract.h>
#include <nlohmann/json_fwd.hpp>

class QIODevice;

namespace protodb
{

class Seance;

class PROTODB_DEV_EXPORT SeanceCreator: public CreatorAbstract
{
public:
    virtual ~SeanceCreator() override = default;

    virtual Seance* create(QObject* parent = nullptr) const = 0;
    virtual Seance* create(const nlohmann::json&, QObject* parent = nullptr) const = 0;
};

} // namespace protodb

Q_DECLARE_INTERFACE(protodb::SeanceCreator, "ProtoDb.SeanceCreatorInterface")
