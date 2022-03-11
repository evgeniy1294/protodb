#include "protodb/configurable/ConfigAggregator.h"

ConfigAggregator::ConfigAggregator(QObject* parent)
    : QObject(parent)
    , Configurable()
{

}

QString ConfigAggregator::name() const
{
    return m_name;
}

void ConfigAggregator::setName(const QString& name)
{
    if ( name == m_name )
        return;

    m_name = name;
    emit sNameChanged(m_name);
}

bool ConfigAggregator::attachObject(const QString& prefix, Configurable* cfg, int priority)
{
    if ( prefix.isEmpty() || cfg == nullptr )
        return false;

    bool ret = false;

    int pos = position(prefix);
    if (pos == -1) {
        Section section;
            section.prefix = prefix;
            section.obj = cfg;
            section.priority = priority;

        for (int i = 0; i < m_sections.size(); i++) {
            if ( m_sections[i].priority > priority ) {
                m_sections.insert(i, section);
                break;
            }
        }

        ret = true;
        emit sSectionAdded(prefix);
    }
    else {
        if ( m_sections[pos].obj != nullptr )
            return false;

        m_sections[pos].obj = cfg;

        if ( m_sections[pos].priority != priority ) {
            auto section = m_sections[pos];

            emit sSectionAboutToBeRemoved(prefix);
                m_sections.removeAt(pos);
            emit sSectionRemoved(prefix);

            for (int i = 0; i < m_sections.size(); i++) {
                if ( m_sections[i].priority > priority ) {
                    m_sections.insert(i, section);
                    break;
                }
            }

            emit sSectionAdded(prefix);
        }

        ret = true;
    }

    return ret;
}

void ConfigAggregator::detachObject(Configurable* cfg)
{
    int pos = position(cfg);
    if ( pos != -1 ) {
        auto prefix = m_sections[pos].prefix;

        emit sSectionAboutToBeRemoved(prefix);
            m_sections.removeAt(pos);
        emit sSectionRemoved(prefix);
    }
}

void ConfigAggregator::detachObject(const QString& prefix)
{
    int pos = position(prefix);
    if ( pos != -1 ) {
        emit sSectionAboutToBeRemoved( prefix );
            m_sections.removeAt( pos );
        emit sSectionRemoved( prefix );
    }
}

bool ConfigAggregator::acceptObject(const QString& prefix)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];
    if (section.obj == nullptr)
        return false;

    section.obj->config(section.config);
    section.obj->state(section.state);

    return true;
}

bool ConfigAggregator::acceptObjectConfig(const QString& prefix)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];
    if (section.obj == nullptr)
        return false;

    section.obj->config(section.config);

    return true;
}

bool ConfigAggregator::acceptObjectState(const QString& prefix)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];
    if (section.obj == nullptr)
        return false;

    section.obj->state(section.state);

    return true;
}

bool ConfigAggregator::updateSection(const QString &prefix)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];
    if (section.obj == nullptr)
        return false;

    section.obj->state(section.config);
    section.obj->state(section.state);

    return true;
}

bool ConfigAggregator::updateSectionConfig(const QString &prefix)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];
    if (section.obj == nullptr)
        return false;

    section.obj->config(section.config);

    return true;
}

bool ConfigAggregator::updateSectionState(const QString &prefix)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];
    if (section.obj == nullptr)
        return false;

    section.obj->state(section.state);

    return true;
}

bool ConfigAggregator::setSectionConfig(const QString &prefix, const nlohmann::json &json, bool accept)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];
        section.config = json;

    if (accept && section.obj != nullptr) {
        section.obj->setConfig(json);
        section.obj->config(section.config);
    }

    return true;
}

bool ConfigAggregator::modifySectionConfig(const QString &prefix, const nlohmann::json &json, bool accept)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];

    for( const auto& it : json.items() )
        section.config[ it.key() ] = it.value();

    if (accept && section.obj != nullptr) {
        section.obj->setConfig(json);
        section.obj->config(section.config);
    }

    return true;
}

bool ConfigAggregator::getSectionConfig(const QString &prefix, nlohmann::json &json) const
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    json = m_sections[pos].config;

    return true;
}

bool ConfigAggregator::getDefaultSectionConfig(const QString &prefix, nlohmann::json &json) const
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];

    if ( section.obj != nullptr ) {
        section.obj->defaultConfig(json);
    }
    else {
        json = section.config;
    }

    return true;
}

bool ConfigAggregator::setSectionState(const QString &prefix, const nlohmann::json &json, bool accept)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];
        section.state = json;

    if (accept && section.obj != nullptr) {
        section.obj->setState(json);
        section.obj->state(section.state);
    }

    return true;
}

bool ConfigAggregator::modifySectionState(const QString &prefix, const nlohmann::json &json, bool accept)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];

    for( const auto& it : json.items() )
        section.state[ it.key() ] = it.value();

    if (accept && section.obj != nullptr) {
        section.obj->setState(json);
        section.obj->state(section.state);
    }

    return true;
}

bool ConfigAggregator::getSectionState(const QString &prefix, nlohmann::json &json) const
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    json = m_sections[pos].state;

    return true;
}

bool ConfigAggregator::getDefaultSectionState(const QString &prefix, nlohmann::json &json) const
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto& section = m_sections[pos];

    if ( section.obj != nullptr ) {
        section.obj->defaultState(json);
    }
    else {
        json = section.state;
    }

    return true;
}

QString ConfigAggregator::prefix(Configurable* cfg) const
{
    for( int i = 0; i < m_sections.size(); i++) {
        if ( m_sections[i].obj == cfg ) {
            return m_sections[i].prefix;
        }
    }

    return QString();
}

int ConfigAggregator::size() const
{
    return m_sections.size();
}

int ConfigAggregator::position(const QString& prefix) const
{
    if (prefix.isEmpty())
        return -1;

    int ret = -1;
    for( int i = 0; i < m_sections.size(); i++) {
        if ( m_sections[i].prefix == prefix ) {
            ret = i; break;
        }
    }

    return ret;
}

int ConfigAggregator::position(Configurable* cfg) const
{
    int ret = -1;
    for( int i = 0; i < m_sections.size(); i++) {
        if ( m_sections[i].obj == cfg ) {
            ret = i; break;
        }
    }

    return ret;
}

bool ConfigAggregator::contains(const QString& prefix) const
{
    int idx = -1;
    for( int i = 0; i < m_sections.size(); i++) {
        if ( m_sections[i].prefix == prefix ) {
            idx = i; break;
        }
    }

    return idx != -1;
}

bool ConfigAggregator::contains(Configurable* cfg) const
{
    int idx = -1;
    for( int i = 0; i < m_sections.size(); i++) {
        if ( m_sections[i].obj == cfg ) {
            idx = i; break;
        }
    }

    return idx != -1;
}



bool ConfigAggregator::isEmpty() const
{
    return m_sections.isEmpty();
}

void ConfigAggregator::clear()
{
    emit sAboutToClear();
        m_sections.clear();
    emit sCleared();
}

void ConfigAggregator::setConfig(const nlohmann::json &json)
{
    for ( auto& [key, config]: json.items() ) {
        if ( !config.is_object() ) {
            continue;
        }

        QString prefix( key.c_str() );
        int pos = position(prefix);

        if (pos )
    }
}
