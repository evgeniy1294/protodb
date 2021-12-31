#include "FactoryAbstract.h"
#include "CreatorAbstract.h"
#include "GlobalFactoryStorage.h"

FactoryAbstract::FactoryAbstract(QObject* parent)
    : QObject(parent)
{
}

FactoryAbstract::~FactoryAbstract()
{
}

QPointer<FactoryAbstract> FactoryAbstract::globalInstance(const QString& fid)
{
    return GlobalFactoryStorage::getFactory(fid);
}

QString FactoryAbstract::fid() const
{
    return QString();
}

bool FactoryAbstract::addCreator(const QSharedPointer<CreatorAbstract>& creator)
{
    if ( m_creators.contains(creator->cid()) ) {
        return false;
    }

    m_creators[ creator->cid() ] = creator;
    emit sCreatorAdded( creator->cid() );

    return true;
}

void FactoryAbstract::removeCreator(const QString& cid)
{
    if ( m_creators.contains(cid) ) {
        m_creators.remove(cid);
        emit sCreatorRemoved(cid);
    }
}

void FactoryAbstract::clear()
{
    for (auto& it: m_creators) {
        auto cid = it->cid();

        m_creators.remove(cid);
        emit sCreatorRemoved(cid);
    }
}

bool FactoryAbstract::containsCreator(const QString& cid) const
{
    return m_creators.contains(cid);
}

QList<QSharedPointer<CreatorAbstract> > FactoryAbstract::getAllCreators() const
{
    return m_creators.values();
}

QSharedPointer<CreatorAbstract> FactoryAbstract::getCreator(const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return m_creators[ cid ];
    }

    return nullptr;
}

QSharedPointer<CreatorAbstract> FactoryAbstract::operator[](const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return m_creators[ cid ];
    }

    return nullptr;
}

QSet<QString> FactoryAbstract::getAllCid() const
{

}

