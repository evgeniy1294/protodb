#pragma once

#include <protodb/configurable/Configurable.h>

class ConfigAggregator final:
    public QObject,
    public Configurable
{
    Q_OBJECT

public:
    ConfigAggregator(QObject* parent = nullptr);
   ~ConfigAggregator() = default;

// ----------- [Aggregator interface] ------------
    QString name() const;
    void setName(const QString& name);

    // Attach Configurable object to aggregator
    bool attachObject(const QString& prefix, Configurable* cfg, int priority);

    // Detach Configurable object from aggregator
    void detachObject(Configurable* cfg);
    void detachObject(const QString& prefix);

    // Save config in sections
    bool acceptObject(const QString& prefix);
    bool acceptObjectConfig(const QString& prefix);
    bool acceptObjectState (const QString& prefix);

    void setObjectConfig(const QString& prefix, const nlohmann::json& json);
    void modifyObjectConfig(const QString& prefix, const nlohmann::json& json);
    void getObjectConfig(const QString& prefix, nlohmann::json& json) const;
    void getDefaultObjectConfig(const QString& prefix, nlohmann::json& json) const;

    void setObjectState(const QString& prefix, const nlohmann::json& json);
    void modifyObjectState(const QString& prefix, const nlohmann::json& json);
    void getObjectState(const QString& prefix, nlohmann::json& json) const;
    void getDefaultObjectState(const QString& prefix, nlohmann::json& json) const;

    QString prefix(Configurable* cfg) const;

    int size() const;
    int position(const QString& prefix) const;
    int position(Configurable* cfg) const;
    bool contains(const QString& prefix) const;
    bool contains(Configurable* cfg) const;
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
    void sSectionAdded( const QString& prefix );
    void sSectionAboutToBeRemoved( const QString& prefix );
    void sSectionRemoved( const QString& prefix );
    void sAboutToClear();
    void sCleared();

    void sObjectConfigChanged ( const QString& prefix );
    void sObjectStateChanged  ( const QString& prefix );

    void sNameChanged( const QString& name );

private:
    struct Section {
        Configurable* ptr;
        QString prefix;
        nlohmann::json config;
        nlohmann::json state;
        int priority;
    };

    QList< Section > m_sections;
    QString m_name = tr("Config aggregator");

private:
    //Section& find_or_create(const QString& prefix, int priority);
};

