#pragma once

#include <protodb/sessions/SessionManager.h>

namespace protodb {

class ProtodbSessionManager: public SessionManager
{
    Q_OBJECT

public:
    static ProtodbSessionManager& instance();

private:
    ProtodbSessionManager(QObject* parent = nullptr);
   ~ProtodbSessionManager() = default;

    void connect_signals();

    bool load_session(const QString& path_to_folder) override;
    bool save_session(const QString& path_to_folder) override;
};

} // namespace protodb
