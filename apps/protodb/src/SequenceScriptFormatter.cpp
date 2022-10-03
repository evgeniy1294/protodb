#include "SequenceScriptFormatter.h"

#include <protodb/ScriptInterface.h>

QString SequenceScriptFormatter::name() const
{
    return m_interface->syntaxId();
}

QByteArray SequenceScriptFormatter::format(const QString& code)
{
    return m_interface->compileCode(code);
}
