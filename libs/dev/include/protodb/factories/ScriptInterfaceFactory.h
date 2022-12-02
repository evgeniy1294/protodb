#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/FactoryAbstract.h>

#include <nlohmann/json.hpp>

namespace protodb
{

class ScriptInterfaceCreator;
class ScriptInterface;

class PROTODB_DEV_EXPORT ScriptInterfaceFactory final: public FactoryAbstract
{
    Q_OBJECT

public:
    explicit ScriptInterfaceFactory(QObject* parent = nullptr);
    ~ScriptInterfaceFactory() = default;

    static QPointer< ScriptInterfaceFactory > globalInstance();
    static QString fid();

    // ---------------- [ Creators ] --------------- //
    bool addCreator(const QSharedPointer< ScriptInterfaceCreator >& creator);

    QList< QSharedPointer< ScriptInterfaceCreator > > getAllCreators() const;
    QSharedPointer< ScriptInterfaceCreator > getCreator( const QString& cid ) const;
    QSharedPointer< ScriptInterfaceCreator > operator[]( const QString& cid ) const;

    // ------------ [ Default creator ] ------------ //
    const QString &getDefaultCreator() const;
    void setDefaultCreator( const QString &cid );

    // ------------ [ Create new ] --------- //
    ScriptInterface* createScriptInterface() const;
    ScriptInterface* createScriptInterface(const nlohmann::json& json) const;

    ScriptInterface* createScriptInterface(const QString &cid) const;
    ScriptInterface* createScriptInterface(const QString &cid, const nlohmann::json& json) const;

private:
    Q_DISABLE_COPY( ScriptInterfaceFactory )
    QString m_default_cid;
};

} // namespace protodb
