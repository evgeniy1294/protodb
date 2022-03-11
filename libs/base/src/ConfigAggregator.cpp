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
            section.ptr = cfg;
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
        if ( m_sections[pos].ptr != nullptr )
            return false;

        m_sections[pos].ptr = cfg;

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

    auto section = m_sections[pos];
    if (section.ptr == nullptr)
        return false;

    section.ptr->config(section.config);
    section.ptr->state(section.state);

    return true;
}

bool ConfigAggregator::acceptObjectConfig(const QString& prefix)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto section = m_sections[pos];
    if (section.ptr == nullptr)
        return false;

    section.ptr->config(section.config);

    return true;
}

bool ConfigAggregator::acceptObjectState(const QString& prefix)
{
    int pos = position(prefix);
    if ( pos == -1 )
        return false;

    auto section = m_sections[pos];
    if (section.ptr == nullptr)
        return false;

    section.ptr->state(section.state);

    return true;
}

void ConfigAggregator::setObjectConfig(const QString& prefix, const nlohmann::json& json)
{
    int pos = position(prefix);
    auto& section = m_sections[pos];

    if (section.ptr != nullptr) {
        section.ptr->setConfig(json);
    }
}

QString ConfigAggregator::prefix(Configurable* cfg) const
{
    for( int i = 0; i < m_sections.size(); i++) {
        if ( m_sections[i].ptr == cfg ) {
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
        if ( m_sections[i].ptr == cfg ) {
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
        if ( m_sections[i].ptr == cfg ) {
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
