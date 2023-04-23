#include "protodb/factories/IOWidgetFactory.h"

#include <protodb/creators/IOWidgetCreatorInterface.h>
#include <protodb/factories/GlobalFactoryStorage.h>

#include <nlohmann/json.hpp>

using namespace protodb;

IOWidgetFactory::IOWidgetFactory(QObject* parent)
    : FactoryAbstract(parent)
{

}

QList<QSharedPointer<IOWidgetCreator> > IOWidgetFactory::getAllCreators() const
{
    QList<QSharedPointer<IOWidgetCreator> > ret;

    for (auto& it: m_creators) {
        auto ptr = qSharedPointerCast<IOWidgetCreator>(it);
        if (ptr) {
            ret.append(ptr);
        }
    }

    return ret;
}

QSharedPointer<IOWidgetCreator> IOWidgetFactory::getCreator(const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<IOWidgetCreator>(m_creators[cid]);
    }

    return nullptr;
}

QSharedPointer<IOWidgetCreator> IOWidgetFactory::operator[](const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<IOWidgetCreator>(m_creators[cid]);
    }

    return nullptr;
}

const QString& IOWidgetFactory::getDefaultCreator() const
{
    return m_default_cid;
}

void IOWidgetFactory::setDefaultCreator(const QString& cid)
{
    if (m_creators.contains(cid))
        m_default_cid = cid;
}

IOWidget* IOWidgetFactory::createIOWidget() const
{
    return createIOWidget(m_default_cid);
}

IOWidget* IOWidgetFactory::createIOWidget(const nlohmann::json& json) const
{
    return createIOWidget(m_default_cid, json);
}

IOWidget* IOWidgetFactory::createIOWidget(const QString& cid) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< IOWidgetCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->create();
    }

    return nullptr;
}

IOWidget* IOWidgetFactory::createIOWidget(const QString& cid, const nlohmann::json& json) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< IOWidgetCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->create(json);
    }

    return nullptr;
}

QPointer<IOWidgetFactory> IOWidgetFactory::globalInstance()
{
    return qobject_cast< IOWidgetFactory * >( GlobalFactoryStorage::getFactory( fid() ) );
}

QString IOWidgetFactory::fid()
{
    return QString("IOWidgetFactory");
}

bool IOWidgetFactory::addCreator(const QSharedPointer<IOWidgetCreator>& creator)
{
    if (!creator || containsCreator(creator->cid())) {
        return false;
    }

    m_creators[ creator->cid() ] = creator;

    return true;
}


