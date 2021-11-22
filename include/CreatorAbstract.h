#pragma once

#include <QtCore>

class CreatorAbstract
{
public:
    virtual const QString& cid() {
        static QString dummyCid;
        return dummyCid;
    }
};
