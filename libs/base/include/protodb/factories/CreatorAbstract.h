#pragma once

#include <protodb/export/base_cfg.hpp>

#include <QString>
#include <QVersionNumber>
#include <QtPlugin>

#ifndef MODULE_VERSION_MAJ
    #define MODULE_VERSION_MAJ 0
#endif

#ifndef MODULE_VERSION_MIN
    #define MODULE_VERSION_MIN 0
#endif

#ifndef MODULE_VERSION_MIC
    #define MODULE_VERSION_MIC 1
#endif

namespace protodb
{

class PROTODB_BASE_EXPORT CreatorAbstract
{
public:
    virtual ~CreatorAbstract() = default;

    virtual QString cid() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;

    virtual QString iconName() const {
        return QString();
    }

    virtual QVersionNumber version() const {
        return QVersionNumber(MODULE_VERSION_MAJ, MODULE_VERSION_MIN, MODULE_VERSION_MIC);
    }
};

} // namespace protodb

Q_DECLARE_INTERFACE(protodb::CreatorAbstract, "ProtoDb.CreatorAbstract")
