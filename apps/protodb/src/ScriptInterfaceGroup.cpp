#include "ScriptInterfaceGroup.h"

#include <QSharedPointer>

using namespace protodb;

ScriptMultiInterface::ScriptMultiInterface(QObject* parent)
    : ScriptInterface(parent)
    , m_interface_id(-1) { }

void ScriptMultiInterface::addScriptInterface(QSharedPointer<ScriptInterface>& interface)
{
    for (int i = 0; i < m_script_interfaces.size(); i++) {
        if (interface->syntaxId() == m_script_interfaces[i]->syntaxId())
            return;
    }

    m_script_interfaces.append(interface);
    connect(interface.data(), &ScriptInterface::sErrorOccuared, this, &ScriptMultiInterface::error_occuared);
    connect(interface.data(), &ScriptInterface::sPrint, this, &ScriptMultiInterface::print);
}

void ScriptMultiInterface::rmScriptInterface(QSharedPointer<ScriptInterface>& interface)
{
    m_script_interfaces.removeOne(interface);
}

QList<QSharedPointer<ScriptInterface> > ScriptMultiInterface::scriptInterfaces() const
{
    return m_script_interfaces;
}

QString ScriptMultiInterface::fileExtention() const
{
    if (m_interface_id != -1 && m_interface_id < m_script_interfaces.size()) {
        return m_script_interfaces[m_interface_id]->syntaxId();
    }

    return QString();
}

QStringList ScriptMultiInterface::fileExtensions() const
{
    QStringList ret;

    for (int i = 0; i < m_script_interfaces.size(); i++) {
        ret.append( m_script_interfaces[i]->syntaxId() );
    }

    return ret;
}

bool ScriptMultiInterface::setScriptFile(const QString& path)
{
    if (path.isEmpty()) {
        m_script_file.clear();
        m_interface_id = -1;
        return true;
    }

    for (int i = 0; i < m_script_interfaces.size(); i++) {
        if (m_script_interfaces[i]->setScriptFile(path)) {
            m_interface_id = i;
            m_script_file = path;

            return true;
        }
    }

    return false;
}

QString ScriptMultiInterface::scriptFile() const
{
    return m_script_file;
}

bool ScriptMultiInterface::isValid() const
{
    if (m_interface_id != -1 && m_interface_id < m_script_interfaces.size()) {
        return m_script_interfaces[m_interface_id]->isValid();
    }

    return false;
}

QString ScriptMultiInterface::syntaxId() const
{
    if (m_interface_id != -1 && m_interface_id < m_script_interfaces.size()) {
        return m_script_interfaces[m_interface_id]->syntaxId();
    }

    return QString();
}

QByteArray ScriptMultiInterface::compileCode(const QString& code) const
{
    if (m_interface_id != -1 && m_interface_id < m_script_interfaces.size()) {
        return m_script_interfaces[m_interface_id]->compileCode(code);
    }

    return QByteArray();
}

bool ScriptMultiInterface::handleDataEvent(Event event, QByteArray& bytes)
{
    if (m_interface_id != -1 && m_interface_id < m_script_interfaces.size()) {
        return m_script_interfaces[m_interface_id]->handleDataEvent(event, bytes);
    }

    return false;
}

void ScriptMultiInterface::error_occuared(QString detail)
{
    emit sErrorOccuared(detail);
}

void ScriptMultiInterface::print(QString text)
{
    emit sPrint(text);
}
