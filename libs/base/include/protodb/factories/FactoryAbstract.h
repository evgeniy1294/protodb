#pragma once

#include <protodb/export/base_cfg.hpp>

#include <QObject>
#include <QSharedPointer>
#include <QPointer>
#include <QString>
#include <QList>
#include <QHash>

namespace protodb {

class CreatorAbstract;

class PROTODB_BASE_EXPORT FactoryAbstract: public QObject
{
    Q_OBJECT

public:
    static QPointer< FactoryAbstract > globalInstance( const QString& fid );

    // ----------[Fabric interface]------------ //
    bool addCreator(const QSharedPointer< CreatorAbstract >& creator);

    void removeCreator(const QString& cid);
    void clear();

    bool containsCreator(const QString& cid) const;

    QList< QSharedPointer< CreatorAbstract > > getAllCreators() const;
    QSharedPointer< CreatorAbstract > getCreator(const QString& cid) const;
    QSharedPointer< CreatorAbstract > operator[](const QString& cid) const;

    QStringList getAllCid() const;

    // ------------ [ Default creator ] ------------ //
    const QString &getDefaultCreator() const;
    void setDefaultCreator( const QString &cid );

signals:
    void sCreatorAdded( QString cid );
    void sCreatorRemoved( QString cid );

protected:
    FactoryAbstract(QObject* parent = nullptr);
    virtual ~FactoryAbstract();

    QHash <QString, QSharedPointer< CreatorAbstract > > m_creators;

private:
    Q_DISABLE_COPY(FactoryAbstract)
    QString m_default_cid;
};

} // namespace protodb
