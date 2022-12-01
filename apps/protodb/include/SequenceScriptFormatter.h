#pragma once

#include "SequenceFormatter.h"

#include <QSharedPointer>

namespace protodb {

class ScriptInterface;

class SequenceScriptFormatter: public SequenceFormatter
{
public:
    SequenceScriptFormatter( QSharedPointer<ScriptInterface>& interface )
        : m_interface(interface) { }

   ~SequenceScriptFormatter() = default;

    QString name() const override;
    QByteArray format(const QString& code) override;

private:
    QSharedPointer<ScriptInterface> m_interface;
};

} // namespace protodb
