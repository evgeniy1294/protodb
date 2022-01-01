#pragma once

#include <QtCore>

#ifndef MODULE_VERSION_MAJ
#define MODULE_VERSION_MAJ 0
#endif

#ifndef MODULE_VERSION_MIN
#define MODULE_VERSION_MIN 0
#endif

#ifndef MODULE_VERSION_MIC
#define MODULE_VERSION_MIC 0
#endif

class CreatorAbstract
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

Q_DECLARE_INTERFACE(CreatorAbstract, "ProtoDb.CreatorAbstract")
