#pragma once

#include <QString>

class Sequence;

class SequenceFormatter {
public:
    SequenceFormatter() = default;

    virtual QString name() const = 0;
    virtual QByteArray format(const QString& code) { return QByteArray(); }
};
