#include "Seance.h"

using namespace protodb;

Seance::Seance(QObject* parent)
    : QObject(parent)
    , m_active(false)
    , m_description(QString()){ }

QString Seance::seanceDescription() const
{
    return m_description;
}

void Seance::setConfig(const nlohmann::json& cfg) { m_cfg = cfg; }
void Seance::config(nlohmann::json& cfg) const { cfg = m_cfg; }

bool Seance::isActive() const { return m_active; }
bool Seance::setActive(bool active) { m_active = true; return true; }
void Seance::setDisactive() { m_active = false; }

qint64 Seance::bytesAvailable(int channel) const { return -1; }
qint64 Seance::bytesToWrite(int channel) const { return -1; }

QString Seance::lastError() const { return QString(); }
