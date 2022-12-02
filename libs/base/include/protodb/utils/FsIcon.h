#pragma once

#include <protodb/export/base_cfg.hpp>

#include <qmetatype.h>
#include <QString>

class PROTODB_BASE_EXPORT FsIcon final: public QString {
public:
    FsIcon(const QString& str): QString(str) {}
    FsIcon(const char* str): QString(str) {}
    FsIcon(): QString() {}

    using QString::operator=;
};

Q_DECLARE_METATYPE(FsIcon);
