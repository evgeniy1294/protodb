#include "IoDeviceCreator.h"
#include "IoDeviceFabric.h"

IoDeviceFabric::IoDeviceFabric(QObject* parent)
    : FabricAbstract(parent) {  }

const QString& IoDeviceFabric::fid() const
{
    static QString m_fid = "io_fabric";
    return m_fid;
}

bool IoDeviceFabric::isCompatible(CreatorAbstract* creator) const
{
    return dynamic_cast<IoDeviceCreator*>(creator) ?
                FabricAbstract::isCompatible(creator) : false;
}

QIODevice* IoDeviceFabric::create(const QString& cid, const nlohmann::json& config) const
{
    QIODevice* frame = nullptr;

    if (m_creators.contains(cid)) {
        auto c = static_cast<IoDeviceCreator*>(m_creators[cid]);
        frame = c->create(config);
    }

    return frame;
}
