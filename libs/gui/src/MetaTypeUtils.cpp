#include "utils/MetaTypeUtils.h"

#include <protodb/utils/FsIcon.h>

#include <QIcon>

void registerCustomConverters()
{
    QMetaType::registerConverter<FsIcon, QIcon>([](const FsIcon& icon) -> QIcon {
       return QIcon(icon);
    });
}
