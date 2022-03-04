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
