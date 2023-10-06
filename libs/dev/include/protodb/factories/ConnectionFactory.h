#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/FactoryAbstract.h>

#include <QString>
#include <QSharedPointer>

#include <nlohmann/json_fwd.hpp>

namespace protodb
{

class ConnectionCreator;
class Connection;

class PROTODB_DEV_EXPORT ConnectionFactory final: public FactoryAbstract
{
    Q_OBJECT

public:
    explicit ConnectionFactory(QObject* parent = nullptr);
    ~ConnectionFactory() = default;

    static QPointer< ConnectionFactory > globalInstance();
    static QString fid();

    // ---------------- [ Creators ] --------------- //
    bool addCreator(const QSharedPointer< ConnectionCreator >& creator);

    QList< QSharedPointer< ConnectionCreator > > getAllCreators() const;
    QSharedPointer< ConnectionCreator > getCreator( const QString& cid ) const;
    QSharedPointer< ConnectionCreator > operator[]( const QString& cid ) const;

    // ------------ [ Create new ] --------- //
    Connection* createConnection(QObject* parent = nullptr) const;
    Connection* createConnection(const nlohmann::json& json, QObject* parent = nullptr) const;

    Connection* createConnection(const QString &cid, QObject* parent = nullptr) const;
    Connection* createConnection(const QString &cid, const nlohmann::json& json, QObject* parent = nullptr) const;

private:
    Q_DISABLE_COPY( ConnectionFactory )
};

} // namespace protodb
