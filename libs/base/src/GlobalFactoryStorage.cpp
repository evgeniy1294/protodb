#include "factories/GlobalFactoryStorage.h"
#include "factories/FactoryAbstract.h"

using namespace protodb;

bool GlobalFactoryStorage::addFactory(const QString& name, const QPointer<FactoryAbstract>& factory)
{
    if( ! factory || storage().contains( name ) )
        return false;

    storage()[ name ] = factory;

    QObject::connect( factory, &FactoryAbstract::destroyed,
        [ name ]()
        {
          storage().remove( name );
        }
    );
    return true;
}

QPointer<FactoryAbstract> GlobalFactoryStorage::getFactory(const QString& name) noexcept
{
    return storage().value(name, QPointer<FactoryAbstract>());
}

bool GlobalFactoryStorage::contains(const QString& name) noexcept
{
    return storage().contains(name);
}

QStringList GlobalFactoryStorage::names() noexcept
{
    return storage().keys();
}

QList<QPointer<FactoryAbstract> > GlobalFactoryStorage::factories() noexcept
{
    return storage().values();
}

QMap<QString, QPointer<FactoryAbstract> >& GlobalFactoryStorage::storage()
{
    static QMap<QString, QPointer< FactoryAbstract > > map;
    return map;
}
