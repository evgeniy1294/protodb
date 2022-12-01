#pragma once

#include <protodb/factories/FactoryAbstract.h>

#include <QString>
#include <QSharedPointer>

#include <nlohmann/json.hpp>

class QIODevice;

namespace protodb
{

class IODeviceCreator;

class IODeviceFactory final: public FactoryAbstract
{
    Q_OBJECT

public:
    explicit IODeviceFactory(QObject* parent = nullptr);
    ~IODeviceFactory() = default;

    static QPointer< IODeviceFactory > globalInstance();
    static QString fid();

    // ---------------- [ Creators ] --------------- //
    bool addCreator(const QSharedPointer< IODeviceCreator >& creator);

    QList< QSharedPointer< IODeviceCreator > > getAllCreators() const;
    QSharedPointer< IODeviceCreator > getCreator( const QString& cid ) const;
    QSharedPointer< IODeviceCreator > operator[]( const QString& cid ) const;

    // ------------ [ Default creator ] ------------ //
    const QString &getDefaultCreator() const;
    void setDefaultCreator( const QString &cid );

    // ------------ [ Create new ephemers ] --------- //
    QIODevice* createIODevice() const;
    QIODevice* createIODevice(const nlohmann::json& json) const;

    QIODevice* createIODevice(const QString &cid) const;
    QIODevice* createIODevice(const QString &cid, const nlohmann::json& json) const;

private:
    Q_DISABLE_COPY( IODeviceFactory )
    QString m_default_cid;
};

} // namespace protodb
