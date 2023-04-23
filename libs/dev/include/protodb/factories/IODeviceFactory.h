#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/FactoryAbstract.h>

#include <QString>
#include <QSharedPointer>

#include <nlohmann/json_fwd.hpp>

class QIODevice;

namespace protodb
{

class SeanceCreator;

class PROTODB_DEV_EXPORT IODeviceFactory final: public FactoryAbstract
{
    Q_OBJECT

public:
    explicit IODeviceFactory(QObject* parent = nullptr);
    ~IODeviceFactory() = default;

    static QPointer< IODeviceFactory > globalInstance();
    static QString fid();

    // ---------------- [ Creators ] --------------- //
    bool addCreator(const QSharedPointer< SeanceCreator >& creator);

    QList< QSharedPointer< SeanceCreator > > getAllCreators() const;
    QSharedPointer< SeanceCreator > getCreator( const QString& cid ) const;
    QSharedPointer< SeanceCreator > operator[]( const QString& cid ) const;

    // ------------ [ Default creator ] ------------ //
    const QString &getDefaultCreator() const;
    void setDefaultCreator( const QString &cid );

    // ------------ [ Create new device ] --------- //
    QIODevice* createIODevice(QString& desc) const;
    QIODevice* createIODevice(const nlohmann::json& json, QString& desc) const;

    QIODevice* createIODevice(const QString &cid, QString& desc) const;
    QIODevice* createIODevice(const QString &cid, const nlohmann::json& json, QString& desc) const;

private:
    Q_DISABLE_COPY( IODeviceFactory )
    QString m_default_cid;
};

} // namespace protodb
