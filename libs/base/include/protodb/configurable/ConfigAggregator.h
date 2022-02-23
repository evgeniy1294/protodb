#pragma once

#include <protodb/configurable/Configurable.h>
/*
class ConfigAggregator final:
    public QObject,
    public Configurable
{
    Q_OBJECT

public:
// ----------- [Aggregator interface] ------------
    // Attach Configurable object to aggregator
    bool attachObject(const QString& prefix, Configurable* cfg);

    // Detach Configurable object from aggregator
    void detachObject(Configurable* cfg);
    void detachObject(const QString& prefix);

    bool acceptObject(const QString& uid);
    bool acceptObjectConfig(const QString& uid);
    bool acceptObjectState (const QString& uid);

    void setObjectConfig(const QString& prefix, const nlohmann::json& json);
    void modifyObjectConfig(const QString& prefix, const nlohmann::json& json);
    void getObjectConfig(const QString& prefix, nlohmann::json& json) const;
    void getDefaultObjectConfig(const QString& prefix, nlohmann::json& json) const;

    void setObjectState(const QString& prefix, const nlohmann::json& json);
    void modifyObjectState(const QString& prefix, const nlohmann::json& json);
    void getObjectState(const QString& prefix, nlohmann::json& json) const;
    void getDefaultObjectState(const QString& prefix, nlohmann::json& json) const;

    int size() const;
    bool contains(const QString& prefix);
    bool isEmpty() const;
    void clear();


// ----------- [Configurable interface] ----------
    void setConfig(const nlohmann::json& json) override;
    void config(nlohmann::json& json) const override;
    void defaultConfig(nlohmann::json& json) const override;

    void setState(const nlohmann::json& json) override;
    void state(nlohmann::json& json) const override;
    void defaultState(nlohmann::json& json) const override;

    bool writeConfig( const QString &path, const QString &extension = "json" ) const override;
    bool readConfig( const QString &path, const QString &extension = "json" ) override;

    bool writeState( const QString &path, const QString &extension = "json") const override;
    bool readState( const QString &path, const QString &extension = "json") override;

signals:
    void sObjectAdded( const QString& prefix );
    void sObjectAboutToBeRemoved( const QString& prefix );
    void sObjectRemoved( const QString& prefix );
    void sAboutToClear();
    void sCleared();

    void sObjectConfigChanged ( const QString& prefix );
    void sObjectStateChanged  ( const QString& prefix );

private:
    struct Object {
        QString prefix;
        int priority;
        Configurable* ptr;
        nlohmann::json config;
        nlohmann::json state;
    };
};
*/
