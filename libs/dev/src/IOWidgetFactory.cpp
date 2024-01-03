#include "protodb/factories/ConnectionWidgetFactory.h"

#include <protodb/creators/ConnectionWidgetCreator.h>
#include <protodb/factories/GlobalFactoryStorage.h>

using namespace protodb;

ConnectionWidgetFactory::ConnectionWidgetFactory(QObject* parent)
    : FactoryAbstract(parent)
{

}

QList<QSharedPointer<ConnectionWidgetCreator> > ConnectionWidgetFactory::getAllCreators() const
{
    QList<QSharedPointer<ConnectionWidgetCreator> > ret;

    for (auto& it: m_creators) {
        auto ptr = qSharedPointerCast<ConnectionWidgetCreator>(it);
        if (ptr) {
            ret.append(ptr);
        }
    }

    return ret;
}

QSharedPointer<ConnectionWidgetCreator> ConnectionWidgetFactory::getCreator(const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<ConnectionWidgetCreator>(m_creators[cid]);
    }

    return nullptr;
}

QSharedPointer<ConnectionWidgetCreator> ConnectionWidgetFactory::operator[](const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<ConnectionWidgetCreator>(m_creators[cid]);
    }

    return nullptr;
}

ConnectionConfigWidget* ConnectionWidgetFactory::createConfigWidget() const
{
    return createConfigWidget(getDefaultCreator());
}

ConnectionConfigWidget* ConnectionWidgetFactory::createConfigWidget(const nlohmann::json& json) const
{
    return createConfigWidget(getDefaultCreator(), json);
}

ConnectionConfigWidget* ConnectionWidgetFactory::createConfigWidget(const QString& cid) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< ConnectionWidgetCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->createConfigWidget();
    }

    return nullptr;
}

ConnectionConfigWidget* ConnectionWidgetFactory::createConfigWidget(const QString& cid, const nlohmann::json& json) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< ConnectionWidgetCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->createConfigWidget(json);
    }

    return nullptr;
}

ConnectionAdvanceControlWidget* ConnectionWidgetFactory::createAdvanceControlWidget() const
{
    return createAdvanceControlWidget(getDefaultCreator());
}

ConnectionAdvanceControlWidget* ConnectionWidgetFactory::createAdvanceControlWidget(const QString& cid) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< ConnectionWidgetCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->createAdvanceControlWidget();
    }

    return nullptr;
}

QPointer<ConnectionWidgetFactory> ConnectionWidgetFactory::globalInstance()
{
    return qobject_cast< ConnectionWidgetFactory * >( GlobalFactoryStorage::getFactory( fid() ) );
}

QString ConnectionWidgetFactory::fid()
{
    return QString("IOWidgetFactory");
}

bool ConnectionWidgetFactory::addCreator(const QSharedPointer<ConnectionWidgetCreator>& creator)
{
    if (!creator || containsCreator(creator->cid())) {
        return false;
    }

    m_creators[ creator->cid() ] = creator;

    return true;
}


