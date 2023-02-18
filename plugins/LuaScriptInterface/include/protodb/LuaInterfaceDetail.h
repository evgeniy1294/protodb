#pragma once

#include <sol/sol.hpp>

#include <QTimer>
#include <QList>

namespace protodb {

class CrcLogic;
class LuaInterface;

class LuaInterfacePrivate final {
    LuaInterfacePrivate();
   ~LuaInterfacePrivate();

    void setExceptionHandler(sol::state& lua) const;
    void bindSpecial(sol::state& lua);
    void bindUtils(sol::state& lua);
    void bindChecksum(sol::state& lua);
    void bindQt(sol::state& lua);
    void bindAll(sol::state& lua);

    void deleteUserObjects();

private:
    static int exceptionHandler(lua_State* L,
                                sol::optional<const std::exception&> maybe_exception,
                                sol::string_view description);

private:
    Q_DECLARE_PUBLIC(LuaInterface);
    LuaInterface* q_ptr;

    QList<CrcLogic*> m_checksum_list;
    QList<QTimer*> m_qtimer_list;
};

}
