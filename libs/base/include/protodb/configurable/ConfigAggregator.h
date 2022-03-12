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
    void detachObject(Configurable* cfg, bool remove = true);
    void detachObject(const QString& prefix, bool remove = true);

    // Get configs and states from section and set it to object
    bool acceptObject(const QString& prefix);
    bool acceptObjectConfig(const QString& prefix);
    bool acceptObjectState (const QString& prefix);

    // Get configs and states from objects and set it to section
    bool updateSection(const QString& prefix);
    bool updateSectionConfig(const QString& prefix);
    bool updateSectionState (const QString& prefix);

    // Section control api
    void removeSection(const QString& prefix);

    bool setSectionConfig(const QString& prefix, const nlohmann::json& json, bool accept = false);
    bool modifySectionConfig(const QString& prefix, const nlohmann::json& json, bool accept = false);
    bool getSectionConfig(const QString& prefix, nlohmann::json& json) const;
    bool getDefaultSectionConfig(const QString& prefix, nlohmann::json& json) const;

    bool setSectionState(const QString& prefix, const nlohmann::json& json, bool accept = false);
    bool modifySectionState(const QString& prefix, const nlohmann::json& json, bool accept = false);
    bool getSectionState(const QString& prefix, nlohmann::json& json) const;
    bool getDefaultSectionState(const QString& prefix, nlohmann::json& json) const;

    // Container api
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

signals:
    void sSectionAdded( const QString& prefix );
    void sSectionAboutToBeRemoved( const QString& prefix );
    void sSectionRemoved( const QString& prefix );
    void sAboutToClear();
    void sCleared();

    void sNameChanged( const QString& name );

private:
    struct Section {
        Configurable* obj;
        QString prefix;
        nlohmann::json config;
        nlohmann::json state;
        int priority;
    };

    QList< Section > m_sections;
    QString m_name = tr("Config aggregator");

private:
    Section& find_or_create(const QString& prefix, int priority = 50);
};

