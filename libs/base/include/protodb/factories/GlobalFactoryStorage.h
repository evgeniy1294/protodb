#pragma once

#include <QtCore>

namespace protodb
{

class FactoryAbstract;

class GlobalFactoryStorage {
public:
    static bool addFactory( const QString &name, const QPointer< FactoryAbstract > &factory );
    static QPointer< FactoryAbstract > getFactory( const QString &name ) noexcept;
    static bool contains( const QString &name ) noexcept;

    static QStringList names() noexcept;
    static QList< QPointer< FactoryAbstract > > factories() noexcept;

    template< typename T >
    static QStringList namesByType() noexcept;

    template< typename T >
    static QPointer< T > factoriesByType() noexcept;

private:
    GlobalFactoryStorage() = delete;
    static QMap< QString, QPointer< FactoryAbstract > >& storage();
};

template< typename T >
QStringList GlobalFactoryStorage::namesByType( ) noexcept
{
    QStringList ret;

    const auto &m = storage();
    for( auto it = m.cbegin(); it != m.cend(); ++it )
    {
        auto factory = qobject_cast<T>( it.value() );
        if( factory )
            ret.append( it.key() );
    }

    return ret;
}

template< typename T >
QPointer< T > GlobalFactoryStorage::factoriesByType( ) noexcept
{
    QPointer< T > ret;

    const auto &m = storage();
    for( auto it = m.cbegin(); it != m.cend(); ++it )
    {
        auto factory = qobject_cast<T>( it.value() );
        if( factory )
            ret.append( it.value() );
    }

    return ret;
}

} // namespace protodb
