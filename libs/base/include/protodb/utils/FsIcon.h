#pragma once

#include <QtCore>

class FsIcon final: public QString {
public:
    FsIcon(const QString& str): QString(str) {}
    FsIcon(const char* str): QString(str) {}
    FsIcon(): QString() {}

    using QString::operator=;
};

Q_DECLARE_METATYPE(FsIcon);
