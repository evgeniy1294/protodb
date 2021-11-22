#include "FabricAbstract.h"
#include "CreatorAbstract.h"

FabricAbstract::FabricAbstract(QObject* parent)
    : QObject(parent)
{
}

const QString& FabricAbstract::fid() const
{
    static QString dummyFid;
    return dummyFid;
}

bool FabricAbstract::isCompatible(CreatorAbstract* creator) const
{
    return !m_creators.contains(creator->cid());
}

bool FabricAbstract::addCreator(CreatorAbstract* creator)
{
    bool ret = false;

    if (isCompatible(creator)) {
        m_creators[creator->cid()] = creator;
        ret = true;
    }

    return ret;
}

void FabricAbstract::removeCreator(CreatorAbstract* creator)
{
    m_creators.remove(creator->cid());
}

void FabricAbstract::removeCreator(const QString& cid)
{
    m_creators.remove(cid);
}

const QList<QString> FabricAbstract::creators() const
{
    return m_creators.keys();
}
