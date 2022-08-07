#include "protodb/factories/ScriptInterfaceFactory.h"

#include <protodb/creators/ScriptInterfaceCreator.h>
#include <protodb/factories/GlobalFactoryStorage.h>

ScriptInterfaceFactory::ScriptInterfaceFactory(QObject* parent)
    : FactoryAbstract(parent)
{

}

QList<QSharedPointer<ScriptInterfaceCreator> > ScriptInterfaceFactory::getAllCreators() const
{
    QList<QSharedPointer<ScriptInterfaceCreator> > ret;

    for (auto& it: m_creators) {
        auto ptr = qSharedPointerCast<ScriptInterfaceCreator>(it);
        if (ptr) {
            ret.append(ptr);
        }
    }

    return ret;
}

QSharedPointer<ScriptInterfaceCreator> ScriptInterfaceFactory::getCreator(const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<ScriptInterfaceCreator>(m_creators[cid]);
    }

    return nullptr;
}

QSharedPointer<ScriptInterfaceCreator> ScriptInterfaceFactory::operator[](const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<ScriptInterfaceCreator>(m_creators[cid]);
    }

    return nullptr;
}

const QString& ScriptInterfaceFactory::getDefaultCreator() const
{
    return m_default_cid;
}

void ScriptInterfaceFactory::setDefaultCreator(const QString& cid)
{
    if (m_creators.contains(cid))
        m_default_cid = cid;
}

ScriptInterface* ScriptInterfaceFactory::createScriptInterface() const
{
    return createScriptInterface(m_default_cid);
}

ScriptInterface* ScriptInterfaceFactory::createScriptInterface(const nlohmann::json& json) const
{
    return createScriptInterface(m_default_cid, json);
}

ScriptInterface* ScriptInterfaceFactory::createScriptInterface(const QString& cid) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< ScriptInterfaceCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->create();
    }

    return nullptr;
}

ScriptInterface* ScriptInterfaceFactory::createScriptInterface(const QString& cid, const nlohmann::json& json) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< ScriptInterfaceCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->create(json);
    }

    return nullptr;
}

QPointer<ScriptInterfaceFactory> ScriptInterfaceFactory::globalInstance()
{
    return qobject_cast< ScriptInterfaceFactory * >( GlobalFactoryStorage::getFactory( fid() ) );
}

QString ScriptInterfaceFactory::fid()
{
    return QString("IOWidgetFactory");
}

bool ScriptInterfaceFactory::addCreator(const QSharedPointer<ScriptInterfaceCreator>& creator)
{
    if (!creator || containsCreator(creator->cid())) {
        return false;
    }

    m_creators[ creator->cid() ] = creator;

    return true;
}



