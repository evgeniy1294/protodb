#include "ConfigFrame.h"
#include "ConfigFrameCreator.h"
#include "ConfigFrameFabric.h"

ConfigFrameFabric::ConfigFrameFabric(QObject* parent)
    : FabricAbstract(parent) {  }

const QString& ConfigFrameFabric::fid() const
{
    static QString m_fid = "config_frame_fabric";
    return m_fid;
}

bool ConfigFrameFabric::isCompatible(CreatorAbstract* creator) const
{
    return dynamic_cast<ConfigFrameCreator*>(creator) ?
                FabricAbstract::isCompatible(creator) : false;
}

ConfigFrame* ConfigFrameFabric::create(const QString& cid, const nlohmann::json& config) const
{
    ConfigFrame* frame = nullptr;

    if (m_creators.contains(cid)) {
        auto c = static_cast<ConfigFrameCreator*>(m_creators[cid]);
        frame = c->create(config);
    }

    return frame;
}
