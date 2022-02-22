#pragma once

#include <protodb/configurable/Configurable.h>

class JsonKeeper final:
    public QObject,
    public Configurable
{
    Q_OBJECT

public:
    JsonKeeper( const QString& gfid = QString(), const QString& cid = QString() )
        : m_gfid(gfid)
        , m_cid(cid) { }
   ~JsonKeeper() = default;

    void setConfig(const nlohmann::json& json) override {
        m_config = json;
    }

    void config(nlohmann::json& json) const override {
        json = m_config;
    }

    void setState(const nlohmann::json& json) override {
        m_state = json;
    }

    void state(nlohmann::json& json) const override {
        json = m_state;
    }

private:
    QString m_gfid;
    QString m_cid;
    nlohmann::json m_config;
    nlohmann::json m_state;
};
