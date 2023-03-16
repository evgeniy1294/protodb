#pragma once

#include <QString>

namespace protodb {

class SequenceFormatter {
public:
    SequenceFormatter() = default;

    virtual QString name() const = 0;
    virtual QByteArray format(const QString& code) { return QByteArray(); }
};

} // namespace protodb
