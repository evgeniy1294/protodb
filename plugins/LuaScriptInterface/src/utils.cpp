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

