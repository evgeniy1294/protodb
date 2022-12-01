#include "SequenceScriptFormatter.h"

#include <protodb/ScriptInterface.h>

using namespace protodb;

QString SequenceScriptFormatter::name() const
{
    return m_interface->syntaxId();
}

QByteArray SequenceScriptFormatter::format(const QString& code)
{
    return m_interface->compileCode(code);
}
