#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/CreatorAbstract.h>
#include <nlohmann/json_fwd.hpp>

class QIODevice;

namespace protodb
{

class Connection;

class PROTODB_DEV_EXPORT ConnectionCreator: public CreatorAbstract
{
public:
    virtual ~ConnectionCreator() override = default;

    virtual Connection* create(QObject* parent = nullptr) const = 0;
    virtual Connection* create(const nlohmann::json&, QObject* parent = nullptr) const = 0;
};

} // namespace protodb

Q_DECLARE_INTERFACE(protodb::ConnectionCreator, "ProtoDb.ConnectionCreatorInterface")
