#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/FactoryAbstract.h>

#include <nlohmann/json.hpp>

namespace protodb
{

class IOWidgetCreator;
class IOWidget;

class PROTODB_DEV_EXPORT IOWidgetFactory final: public FactoryAbstract
{
    Q_OBJECT

public:
    explicit IOWidgetFactory(QObject* parent = nullptr);
    ~IOWidgetFactory() = default;

    static QPointer< IOWidgetFactory > globalInstance();
    static QString fid();

    // ---------------- [ Creators ] --------------- //
    bool addCreator(const QSharedPointer< IOWidgetCreator >& creator);

    QList< QSharedPointer< IOWidgetCreator > > getAllCreators() const;
    QSharedPointer< IOWidgetCreator > getCreator( const QString& cid ) const;
    QSharedPointer< IOWidgetCreator > operator[]( const QString& cid ) const;

    // ------------ [ Default creator ] ------------ //
    const QString &getDefaultCreator() const;
    void setDefaultCreator( const QString &cid );

    // ------------ [ Create new widget ] --------- //
    IOWidget* createIOWidget() const;
    IOWidget* createIOWidget(const nlohmann::json& json) const;

    IOWidget* createIOWidget(const QString &cid) const;
    IOWidget* createIOWidget(const QString &cid, const nlohmann::json& json) const;

private:
    Q_DISABLE_COPY( IOWidgetFactory )
    QString m_default_cid;
};

} // namespace protodb
