#include "protodb/LuaInterfaceDetail.h"
#include "protodb/LuaScriptInterface.h"

#include <protodb/utils/crc_logic.h>

#include <QTimer>

static float tableToFloat(const sol::table parent, sol::nested< std::vector<uint8_t> > src)
{
    Q_UNUSED(parent)
    auto ret = std::numeric_limits<float>::quiet_NaN();

    const auto& vec = src.value();
    if (vec.size() >= 4) {
        uint32_t tmp = 0;

        tmp  =  vec[0];
        tmp |= (vec[1] & 0xff) << 8;
        tmp |= (vec[2] & 0xff) << 16;
        tmp |= (vec[3] & 0xff) << 24;

        ret = *reinterpret_cast<float*>(&tmp);
    }

    return ret;
}

static double tableToDouble(const sol::table parent, sol::nested< std::vector<uint8_t> > src)
{
    Q_UNUSED(parent)
    auto ret = std::numeric_limits<double>::quiet_NaN();

    const auto& vec = src.value();
    if (vec.size() >= 8) {
        uint64_t tmp = 0;

        tmp  =  static_cast<uint64_t>(vec[0]);
        tmp |= (static_cast<uint64_t>(vec[1]) & 0xff) << 8;
        tmp |= (static_cast<uint64_t>(vec[2]) & 0xff) << 16;
        tmp |= (static_cast<uint64_t>(vec[3]) & 0xff) << 24;
        tmp |= (static_cast<uint64_t>(vec[4]) & 0xff) << 32;
        tmp |= (static_cast<uint64_t>(vec[5]) & 0xff) << 40;
        tmp |= (static_cast<uint64_t>(vec[6]) & 0xff) << 48;
        tmp |= (static_cast<uint64_t>(vec[7]) & 0xff) << 56;

        ret = *reinterpret_cast<double*>(&tmp);
    }

    return ret;
}

static uint16_t tableToShort(const sol::table parent, sol::nested< std::vector<uint8_t> > src)
{
    Q_UNUSED(parent)
    uint16_t ret = 0;

    const auto& vec = src.value();
    if (vec.size() >= 2) {
        ret  =  vec[0];
        ret |= (vec[1] & 0xff) << 8;
    }

    return ret;
}

static uint32_t tableToWord(const sol::table parent, sol::nested< std::vector<uint8_t> > src)
{
    Q_UNUSED(parent)
    uint32_t ret = 0;

    const auto& vec = src.value();
    if (vec.size() >= 4) {
        ret  =  vec[0];
        ret |= (vec[1] & 0xff) << 8;
        ret |= (vec[2] & 0xff) << 16;
        ret |= (vec[3] & 0xff) << 24;
    }

    return ret;
}

static uint64_t tableToDword(const sol::table parent, sol::nested< std::vector<uint8_t> > src)
{
    Q_UNUSED(parent)
    uint64_t ret = 0;

    const auto& vec = src.value();
    if (vec.size() >= 8) {
        ret  =  static_cast<uint64_t>(vec[0]);
        ret |= (static_cast<uint64_t>(vec[1]) & 0xff) << 8;
        ret |= (static_cast<uint64_t>(vec[2]) & 0xff) << 16;
        ret |= (static_cast<uint64_t>(vec[3]) & 0xff) << 24;
        ret |= (static_cast<uint64_t>(vec[4]) & 0xff) << 32;
        ret |= (static_cast<uint64_t>(vec[5]) & 0xff) << 40;
        ret |= (static_cast<uint64_t>(vec[6]) & 0xff) << 48;
        ret |= (static_cast<uint64_t>(vec[7]) & 0xff) << 56;
    }

    return ret;
}

static auto floatToBytes(const sol::table parent, float val)
{
    Q_UNUSED(parent)
    uint32_t tmp = *reinterpret_cast<uint32_t*>(&val);
        uint8_t b1 = tmp & 0xff;
        uint8_t b2 = (tmp >> 8)  & 0xff;
        uint8_t b3 = (tmp >> 16) & 0xff;
        uint8_t b4 = (tmp >> 24) & 0xff;

    return std::make_tuple(b1, b2, b3, b4);
}

static auto doubleToBytes(const sol::table parent, double val)
{
    Q_UNUSED(parent)
    uint64_t tmp = *reinterpret_cast<uint64_t*>(&val);
        uint8_t b1 = tmp & 0xff;
        uint8_t b2 = (tmp >> 8)  & 0xff;
        uint8_t b3 = (tmp >> 16) & 0xff;
        uint8_t b4 = (tmp >> 24) & 0xff;
        uint8_t b5 = (tmp >> 32) & 0xff;
        uint8_t b6 = (tmp >> 40) & 0xff;
        uint8_t b7 = (tmp >> 48) & 0xff;
        uint8_t b8 = (tmp >> 56) & 0xff;

    return std::make_tuple(b1, b2, b3, b4, b5, b6, b7, b8);
}

static auto shortToBytes(const sol::table parent, uint16_t val)
{
    Q_UNUSED(parent)
    uint8_t b1 = val & 0xff;
    uint8_t b2 = (val >> 8)  & 0xff;

    return std::make_tuple(b1, b2);
}

static auto wordToBytes(const sol::table parent, uint32_t val)
{
    Q_UNUSED(parent)

    uint8_t b1 = val & 0xff;
    uint8_t b2 = (val >> 8)  & 0xff;
    uint8_t b3 = (val >> 16) & 0xff;
    uint8_t b4 = (val >> 24) & 0xff;

    return std::make_tuple(b1, b2, b3, b4);
}

static auto dwordToBytes(const sol::table parent, uint64_t val)
{
    Q_UNUSED(parent)

    uint8_t b1 = val & 0xff;
    uint8_t b2 = (val >> 8)  & 0xff;
    uint8_t b3 = (val >> 16) & 0xff;
    uint8_t b4 = (val >> 24) & 0xff;
    uint8_t b5 = (val >> 32) & 0xff;
    uint8_t b6 = (val >> 40) & 0xff;
    uint8_t b7 = (val >> 48) & 0xff;
    uint8_t b8 = (val >> 56) & 0xff;

    return std::make_tuple(b1, b2, b3, b4, b5, b6, b7, b8);
}

int protodb::LuaInterfacePrivate::exceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
    // L is the lua state, which you can wrap in a state_view if necessary
    // maybe_exception will contain exception, if it exists
    // description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
    std::cout << "An exception occurred in a function, here's what it says ";
    if (maybe_exception) {
        std::cout << "(straight from the exception): ";
        const std::exception& ex = *maybe_exception;
        std::cout << ex.what() << std::endl;
    }
    else {
        std::cout << "(from the description parameter): ";
        std::cout.write(description.data(), static_cast<std::streamsize>(description.size()));
        std::cout << std::endl;
    }

    // you must push 1 element onto the stack to be
    // transported through as the error object in Lua
    // note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
    // so we push a single string (in our case, the description of the error)
    return sol::stack::push(L, description);
}

protodb::LuaInterfacePrivate::LuaInterfacePrivate()
    : q_ptr(nullptr)
{

}

protodb::LuaInterfacePrivate::~LuaInterfacePrivate()
{
    deleteUserObjects();
}

void protodb::LuaInterfacePrivate::setExceptionHandler(sol::state& lua) const
{
    lua.set_exception_handler(exceptionHandler);
}

void protodb::LuaInterfacePrivate::bindSpecial(sol::state& lua)
{
    Q_Q(LuaInterface);

    auto log = lua["log"].get_or_create<sol::table>();
    log.set_function( "print", [q](const sol::table& t, const char* str)
    {
        Q_UNUSED(t)
        emit q->sPrint(QString(str));
    });
    log.set_function( "clear", [q](){ emit q->sLogClear(); });

    auto seance = lua["seance"].get_or_create<sol::table>();

    seance.set_function( "stop", [q]() { emit q->sStopSession(); } );

    seance.set_function("send", [q](const sol::table&, sol::nested< std::vector<uint8_t> > src) {
        const auto& vec = src.value();
            const uint8_t* data = &(*vec.data());
            const uint8_t* end  = &(*vec.end());

        QByteArray bytes((char*)vec.data(), vec.size());
        emit q->sSendBytes(bytes);
    });
}

void protodb::LuaInterfacePrivate::bindUtils(sol::state& lua) {
    auto utils = lua["utils"].get_or_create<sol::table>();
        utils.set_function("tableToFloat", tableToFloat);
        utils.set_function("tableToDouble", tableToDouble);
        utils.set_function("tableToShort", tableToShort);
        utils.set_function("tableToWord", tableToWord);
        utils.set_function("tableToDword", tableToDword);
        utils.set_function("floatToBytes", floatToBytes);
        utils.set_function("doubleToBytes", doubleToBytes);
        utils.set_function("shortToBytes", shortToBytes);
        utils.set_function("wordToBytes", wordToBytes);
        utils.set_function("dwordToBytes", dwordToBytes);
}

void protodb::LuaInterfacePrivate::bindChecksum(sol::state& lua)
{
    sol::usertype<CrcLogic> crc_logic_type = lua.new_usertype<CrcLogic>("Checksum",
        sol::call_constructor,
        sol::factories([this]() -> CrcLogic* {
            auto checksum = new CrcLogic;
                m_checksum_list.append(checksum);
            return checksum;
        }),
        sol::meta_function::garbage_collect,
        sol::destructor([this](CrcLogic* checksum) {
            m_checksum_list.removeAll(checksum);
            delete checksum;
    }));

    crc_logic_type.set_function("setWidth", &CrcLogic::setWidth);
    crc_logic_type.set_function("setPoly", &CrcLogic::setPoly);
    crc_logic_type.set_function("setSeed", &CrcLogic::setSeed);
    crc_logic_type.set_function("setXorOut", &CrcLogic::setXorOut);
    crc_logic_type.set_function("setReflectIn", &CrcLogic::setReflectIn);
    crc_logic_type.set_function("setReflectOut", &CrcLogic::setReflectOut);
    crc_logic_type.set_function("finalize", &CrcLogic::finalize);
    crc_logic_type.set_function("validBytes", &CrcLogic::validBytes);
    crc_logic_type.set_function("calculate", [](CrcLogic& self, sol::nested< std::vector<uint8_t> > src) {
        const auto& vec = src.value();
        const uint8_t* data = &(*vec.data());
        const uint8_t* end  = &(*vec.end());

        self.calculate(data, end);

        return;
    });
}

void protodb::LuaInterfacePrivate::bindQt(sol::state& lua)
{
    Q_Q(LuaInterface);

    sol::usertype<QTimer> qtimer_type = lua.new_usertype<QTimer>("QTimer",
        sol::call_constructor,
        sol::factories( [this]() -> QTimer*{
            Q_Q(LuaInterface);
            auto timer = new QTimer(q);
                m_qtimer_list.append(timer);

            return timer;
        }),
        sol::meta_function::garbage_collect,
        sol::destructor([this](QTimer* timer) {
            timer->stop();
            timer->disconnect();
            timer->deleteLater();
            m_qtimer_list.removeAll(timer);
        })
    );
        qtimer_type.set_function("isActive", &QTimer::isActive);
        qtimer_type.set_function("timerId", &QTimer::timerId);
        qtimer_type.set_function("setInterval", sol::resolve<void(int)>(&QTimer::setInterval));
        qtimer_type.set_function("interval", &QTimer::interval);
        qtimer_type.set_function("remainingTime", &QTimer::remainingTime);
        qtimer_type.set_function("setPreciseTimerType", [](QTimer& self) {
            self.setTimerType(Qt::TimerType::PreciseTimer);
        });
        qtimer_type.set_function("setCoarseTimerType", [](QTimer& self) {
            self.setTimerType(Qt::TimerType::CoarseTimer);
        });
        qtimer_type.set_function("setVeryCoarseTimerType", [](QTimer& self) {
            self.setTimerType(Qt::TimerType::VeryCoarseTimer);
        });
        qtimer_type.set_function("setSingleShot", &QTimer::setSingleShot);
        qtimer_type.set_function("isSingleShot", &QTimer::isSingleShot);
        qtimer_type.set_function("stop", &QTimer::stop);
        qtimer_type.set_function("start", sol::resolve<void()>(&QTimer::start));
        qtimer_type.set_function("startInterval", sol::resolve<void(int)>(&QTimer::start));
        qtimer_type.set_function("connect", [q](QTimer& self, sol::function func) {
            q->connect(&self, &QTimer::timeout, q, func);
            return;
        });
        qtimer_type.set_function("disconnect", [](QTimer& self, sol::function func) {
            self.disconnect();
            return;
        });
}

void protodb::LuaInterfacePrivate::bindAll(sol::state& lua)
{
    bindUtils(lua);
    bindChecksum(lua);
    bindQt(lua);
    bindSpecial(lua);
}

void protodb::LuaInterfacePrivate::deleteUserObjects()
{
    for (int i = 0; i < m_checksum_list.size(); i++) {
        delete m_checksum_list[i];
    }
    m_checksum_list.clear();

    for (int i = 0; i < m_qtimer_list.size(); i++) {
        auto timer = m_qtimer_list[i];
        timer->stop();
        timer->disconnect();
        timer->deleteLater();
    }
    m_qtimer_list.clear();
}
