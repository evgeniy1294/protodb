#include "factories/BytecodeEncoderFactory.h"
#include "creators/BytecodeEncoderCreator.h"

#include <protodb/factories/GlobalFactoryStorage.h>

namespace protodb {

BytecodeEncoderFactory::BytecodeEncoderFactory(QObject* parent)
    : FactoryAbstract(parent)
{

}

QPointer<BytecodeEncoderFactory> BytecodeEncoderFactory::globalInstance()
{
    return qobject_cast< BytecodeEncoderFactory * >( GlobalFactoryStorage::getFactory( fid() ) );
}

QString BytecodeEncoderFactory::fid()
{
    return QString("ByteArrayEncoderFactory");
}

bool BytecodeEncoderFactory::addCreator(const QSharedPointer<BytecodeEncoderCreator>& creator)
{
    if (!creator || containsCreator(creator->cid())) {
        return false;
    }

    m_creators[ creator->cid() ] = creator;

    return true;
}

QList<QSharedPointer<BytecodeEncoderCreator> > BytecodeEncoderFactory::getAllCreators() const
{
    QList<QSharedPointer<BytecodeEncoderCreator> > ret;

    for (auto& it: m_creators) {
        auto ptr = qSharedPointerCast<BytecodeEncoderCreator>(it);
        if (ptr) {
            ret.append(ptr);
        }
    }

    return ret;
}

QSharedPointer<BytecodeEncoderCreator> BytecodeEncoderFactory::getCreator(const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<BytecodeEncoderCreator>(m_creators[cid]);
    }

    return nullptr;
}

QSharedPointer<BytecodeEncoderCreator> BytecodeEncoderFactory::operator[](const QString& cid) const
{
    if (m_creators.contains(cid)) {
        return qSharedPointerCast<BytecodeEncoderCreator>(m_creators[cid]);
    }

    return nullptr;
}

ConfigWidgetAbstract* BytecodeEncoderFactory::createConfigWidget(QWidget* parent) const
{
    return createConfigWidget(getDefaultCreator(), parent);
}

ConfigWidgetAbstract* BytecodeEncoderFactory::createConfigWidget(const nlohmann::json& json, QWidget* parent) const
{
    return createConfigWidget(getDefaultCreator(), json, parent);
}

ConfigWidgetAbstract* BytecodeEncoderFactory::createConfigWidget(const QString& cid, QWidget* parent) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< BytecodeEncoderCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->createConfigWidget(parent);
    }

    return nullptr;
}

ConfigWidgetAbstract* BytecodeEncoderFactory::createConfigWidget(const QString& cid, const nlohmann::json& json, QWidget* parent) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< BytecodeEncoderCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->createConfigWidget(json, parent);
    }

    return nullptr;
}

BytecodeEncoderAbstract* BytecodeEncoderFactory::createEncoder(QObject* parent) const
{
    return createEncoder(getDefaultCreator(), parent);
}

BytecodeEncoderAbstract* BytecodeEncoderFactory::createEncoder(const nlohmann::json& json, QObject* parent) const
{
    return createEncoder(getDefaultCreator(), json, parent);
}

BytecodeEncoderAbstract* BytecodeEncoderFactory::createEncoder(const QString& cid, QObject* parent) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< BytecodeEncoderCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->createEncoder(parent);
    }

    return nullptr;
}

BytecodeEncoderAbstract* BytecodeEncoderFactory::createEncoder(const QString& cid, const nlohmann::json& json, QObject* parent) const
{
    if( m_creators.contains(cid) )
    {
        auto creator = qSharedPointerCast< BytecodeEncoderCreator >( m_creators[ cid ] );
        if( ! creator )
            return nullptr;

        return creator->createEncoder(json, parent);
    }

    return nullptr;
}

} // namespace protodb
