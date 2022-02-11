#pragma once

#include <nlohmann/json.hpp>
#include <protodb/factories/FactoryAbstract.h>

class IOWidgetCreatorInterface;
class IOWidget;

class IOWidgetFactory final: public FactoryAbstract
{
    Q_OBJECT

public:
    explicit IOWidgetFactory(QObject* parent = nullptr);
    ~IOWidgetFactory() = default;

    static QPointer< IOWidgetFactory > globalInstance();
    static QString fid();

    // ---------------- [ Creators ] --------------- //
    bool addCreator(const QSharedPointer< IOWidgetCreatorInterface >& creator);

    QList< QSharedPointer< IOWidgetCreatorInterface > > getAllCreators() const;
    QSharedPointer< IOWidgetCreatorInterface > getCreator( const QString& cid ) const;
    QSharedPointer< IOWidgetCreatorInterface > operator[]( const QString& cid ) const;

    // ------------ [ Default creator ] ------------ //
    const QString &getDefaultCreator() const;
    void setDefaultCreator( const QString &cid );

    // ------------ [ Create new ephemers ] --------- //
    IOWidget* createIOWidget() const;
    IOWidget* createIOWidget(const nlohmann::json& json) const;

    IOWidget* createIOWidget(const QString &cid) const;
    IOWidget* createIOWidget(const QString &cid, const nlohmann::json& json) const;

private:
    Q_DISABLE_COPY( IOWidgetFactory )
    QString m_default_cid;
};