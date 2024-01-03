#pragma once

#include <protodb/export/dev_cfg.hpp>

#include <protodb/factories/FactoryAbstract.h>

#include <nlohmann/json_fwd.hpp>

namespace protodb
{

class ConnectionWidgetCreator;
class ConnectionConfigWidget;
class ConnectionAdvanceControlWidget;

class PROTODB_DEV_EXPORT ConnectionWidgetFactory final: public FactoryAbstract
{
    Q_OBJECT

public:
    explicit ConnectionWidgetFactory(QObject* parent = nullptr);
    ~ConnectionWidgetFactory() = default;

    static QPointer< ConnectionWidgetFactory > globalInstance();
    static QString fid();

    // ---------------- [ Creators ] --------------- //
    bool addCreator(const QSharedPointer< ConnectionWidgetCreator >& creator);

    QList< QSharedPointer< ConnectionWidgetCreator > > getAllCreators() const;
    QSharedPointer< ConnectionWidgetCreator > getCreator( const QString& cid ) const;
    QSharedPointer< ConnectionWidgetCreator > operator[]( const QString& cid ) const;

    // ------------ [ Create new widget ] --------- //
    ConnectionConfigWidget* createConfigWidget() const;
    ConnectionConfigWidget* createConfigWidget(const nlohmann::json& json) const;

    ConnectionConfigWidget* createConfigWidget(const QString &cid) const;
    ConnectionConfigWidget* createConfigWidget(const QString &cid, const nlohmann::json& json) const;

    ConnectionAdvanceControlWidget* createAdvanceControlWidget() const;
    ConnectionAdvanceControlWidget* createAdvanceControlWidget(const QString &cid) const;

private:
    Q_DISABLE_COPY( ConnectionWidgetFactory )
};

} // namespace protodb
