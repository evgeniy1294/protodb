#include "protodb/factories/ConnectionFactory.h"

#include <protodb/factories/GlobalFactoryStorage.h>
#include <protodb/creators/ConnectionCreator.h>

#include <nlohmann/json.hpp>

using namespace protodb;

ConnectionFactory::ConnectionFactory(QObject* parent)
    : FactoryAbstract(parent)
{

}

bool ConnectionFactory::addCreator(const QSharedPointer<ConnectionCreator>& creator)
{
    if (!creator || containsCreator(creator->cid())) {
        return false;
    }

    m_creators[ creator->cid() ] = creator;

    return true;
}

QList<QSharedPointer<ConnectionCreator> > ConnectionFactory::getAllCreators() const
{
    QList<QSharedPointer<ConnectionCreator> > ret;

    for (auto& it: m_creators) {
        auto ptr = qSharedPointerCast<ConnectionCreator>(it);
        if (ptr) {
            ret.append(ptr);
        }
    }

    return ret;
}

QSharedPointer<ConnectionCreator> ConnectionFactory::getCreator(const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<ConnectionCreator>(m_creators[cid]);
    }

    return nullptr;
}

QSharedPointer<ConnectionCreator> ConnectionFactory::operator[](const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<ConnectionCreator>(m_creators[cid]);
    }

    return nullptr;
}

const QString& ConnectionFactory::getDefaultCreator() const
{
    return m_default_cid;
}

void ConnectionFactory::setDefaultCreator(const QString& cid)
{
    if (m_creators.contains(cid))
        m_default_cid = cid;
}

Connection* ConnectionFactory::createConnection(QObject* parent) const
{
    return createConnection(m_default_cid, parent);
}

Connection* ConnectionFactory::createConnection(const nlohmann::json& json, QObject* parent) const
{
    return createConnection(m_default_cid, json, parent);
}

Connection* ConnectionFactory::createConnection(const QString& cid, QObject* parent) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< ConnectionCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->create(parent);
    }

    return nullptr;
}

Connection* ConnectionFactory::createConnection(const QString& cid, const nlohmann::json& json, QObject* parent) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< ConnectionCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->create(json, parent);
    }

    return nullptr;
}

QPointer<ConnectionFactory> ConnectionFactory::globalInstance()
{
    return qobject_cast< ConnectionFactory * >( GlobalFactoryStorage::getFactory( fid() ) );
}

QString ConnectionFactory::fid()
{
    return QString("ConnectionFactory");
}

