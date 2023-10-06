#include "utils/crc_logic.h"

using namespace protodb;

QMap<QString, CrcModel> CrcLogic::m_models;

static Crc Reflect ( Crc x, std::size_t bits )
{
    Crc reflection = 0;
    constexpr Crc one = 1;

    for ( std::size_t i = 0 ; i < bits ; ++i, x >>= 1 )
    {
      if ( x & one )
        reflection |= ( one << (bits - 1u - i) );
    }

    return reflection;
}


CrcLogic::CrcLogic()
    : m_table_inited(false)
    , m_width(16)
    , m_poly(0x8005)
    , m_seed(0xFFFF)
    , m_xor_out(0x0)
    , m_ref_in(true)
    , m_ref_out(true)
{
    initStandartModels();
}

CrcLogic::CrcLogic(const CrcModel& model)
{
    initStandartModels();
    setModel(model);
}

CrcLogic::CrcLogic(const QString& model)
{
    initStandartModels();
    setModel(model);
}

CrcModel CrcLogic::model() const
{
    return {m_width, m_poly, m_seed, m_xor_out, m_ref_in, m_ref_out};
}

bool CrcLogic::setModel(const CrcModel& model)
{
    bool ret = false;

    if (model.width >= k_min_poly_size && model.width <= k_max_poly_size) {
        m_width       = model.width;
        m_poly        = model.poly;
        m_seed        = model.seed;
        m_xor_out     = model.xor_out;
        m_ref_in      = model.ref_in;
        m_ref_out     = model.ref_out;

        m_crc = m_seed;

        calculateTable();
        ret = true;
    }

    return ret;
}

bool CrcLogic::setModel(const QString& model)
{
    if (m_models.contains(model)) {
        return setModel(m_models[model]);
    }

    return false;
}

QStringList CrcLogic::standartModels()
{
    return m_models.keys();
}

void CrcLogic::setWidth(std::size_t width)
{
    if (width < k_min_poly_size || width > k_max_poly_size)
        return;

    if (m_width != width){
        m_width = width;
        m_crc = m_seed;
        m_table_inited = false;
    }
}

void CrcLogic::setPoly(Crc poly)
{
    if (m_poly != poly) {
        m_poly = poly;
        m_crc = m_seed;
        m_table_inited = false;
    }
}

void CrcLogic::setSeed(Crc seed)
{
    if (m_seed != seed) {
        m_seed = seed;
        m_crc = m_seed;
    }
}

void CrcLogic::setXorOut(Crc value)
{
    m_xor_out = value;
}

void CrcLogic::setReflectIn(bool reflect)
{
    if (m_ref_in != reflect) {
        m_ref_in = reflect;
        m_crc = m_seed;
        m_table_inited = false;
    }
}

void CrcLogic::setReflectOut(bool reflect)
{
    m_ref_out = reflect;
}

Crc CrcLogic::finalize()
{
    if (m_ref_in)
    {
        if (!m_ref_out)
          m_crc = Reflect(m_crc, m_width);
    }
    else
    {
        if (m_ref_out)
          m_crc = Reflect(m_crc, m_width);
    }

    auto tmp = m_crc;
    m_crc = m_seed;

    return (tmp ^ m_xor_out) & m_result_mask;
}

std::size_t CrcLogic::validBytes()
{
    return (m_width / 8u) + ( (m_width % 8u) != 0 ? 1 : 0);
}

void CrcLogic::calculate(const uint8_t *data, const uint8_t *end)
{
    if (!m_table_inited) {
        calculateTable();
    }

    if (m_ref_in)
    {
        while ( data < reinterpret_cast< const std::uint8_t* >( end ) ) {
            m_crc = m_crc ^ ( static_cast < Crc >(*data++) << 0 );

            m_crc = (m_crc >> 4u) ^ m_table[ m_crc & 0b1111u ];
            m_crc = (m_crc >> 4u) ^ m_table[ m_crc & 0b1111u ];
        }
    }
    else
    {
        std::size_t shift_data = (m_width < 8) ? m_width - 4 : m_width - 8u;

        while ( data < reinterpret_cast< const std::uint8_t* >( end ) ) {
            if ( m_width  < 8 ) {
                std::size_t tbl_idx = (m_crc >> shift_data) ^ (*data >> 4);
                m_crc = (m_crc << 4u) ^ m_table[ tbl_idx & 0b1111 ];

                tbl_idx = (m_crc >> shift_data) ^ (*data++);
                m_crc = (m_crc << 4u) ^ m_table[ tbl_idx & 0b1111 ];
            }
            else
            {
                std::size_t shift_idx = m_width - 4;

                m_crc = m_crc ^ ( static_cast < Crc >(*data++) << shift_data );

                m_crc = (m_crc << 4u) ^ m_table[ ( m_crc >> shift_idx ) & 0b1111 ];
                m_crc = (m_crc << 4u) ^ m_table[ ( m_crc >> shift_idx ) & 0b1111 ];
            }
        }
    }

    return;
}


void CrcLogic::calculateTable() {
    Crc ref_poly = Reflect(m_poly, m_width);
    Crc mask = (m_ref_in) ? static_cast<Crc>(1u) : static_cast<Crc>(1u) << ( m_width - 1u );

    m_result_mask = std::numeric_limits<Crc>::max() >> ( sizeof( Crc )*8u - m_width);

    for (std::size_t i = 0u; i < k_table_size; i++) {
        Crc x = 0;

        if (m_ref_in)
        {
            for (std::size_t j = 0x01; j < 0x10; j <<= 1) {
                bool bit = x & mask;
                bit = ( i & j ) ? !bit : bit;
                x = (bit) ? (x >> 1) ^ ref_poly : (x >> 1);
            }
        }
        else
        {
            for (std::size_t j = 0x08; j > 0; j >>= 1) {
                bool bit = x & mask;
                bit = ( i & j ) ? !bit : bit;
                x = (bit) ? (x << 1) ^ (m_poly) : (x << 1);
            }

        }

      m_table[i] = static_cast<Crc>(x & m_result_mask);
    }

    m_table_inited = true;
}

void CrcLogic::initStandartModels()
{
    static bool inited = false;

    if (!inited) {
        m_models["CRC64 WE"]          = CrcModel{64, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, true, true};
        m_models["CRC64 XZ"]          = CrcModel{64, 0x42F0E1EBA9EA3693, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, false, false};
        m_models["CRC64"]             = CrcModel{64, 0x42F0E1EBA9EA3693, 0, 0, false, false};
        m_models["CRC40 GSM"]         = CrcModel{40, 0x4820009, 0, 0xFFFFFFFFFF, false, false};
        m_models["CRC32 XFER"]        = CrcModel{32, 0xAF, 0, 0, false, false};
        m_models["CRC32 JAMCRC"]      = CrcModel{32, 0x04C11DB7, 0xFFFFFFFF, 0, true, true};
        m_models["CRC32 Q"]           = CrcModel{32, 0x814141AB, 0, 0, false, false};
        m_models["CRC32 POSIX"]       = CrcModel{32, 0x04C11DB7, 0, 0xFFFFFFFF, false, false};
        m_models["CRC32 MPEG-2"]      = CrcModel{32, 0x04C11DB7, 0xFFFFFFFF, 0, false, false};
        m_models["CRC32 D"]           = CrcModel{32, 0xA833982B, 0xFFFFFFFF, 0xFFFFFFFF, true, true};
        m_models["CRC32 C"]           = CrcModel{32, 0x1EDC6F41, 0xFFFFFFFF, 0xFFFFFFFF, true, true};
        m_models["CRC32 BZIP2"]       = CrcModel{32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, false, false};
        m_models["CRC32 ZLIB"]        = CrcModel{32, 0x04C11DB7, 0xFFFFFFFF, 0xFFFFFFFF, true, true};
        m_models["CRC31 PHILIPS"]     = CrcModel{31, 0x04C11DB7, 0x7FFFFFFF, 0x7FFFFFFF, false, false};
        m_models["CRC24 FLEXRAY-A"]   = CrcModel{24, 0x5D6DCB, 0xFEDCBA, 0, false, false};
        m_models["CRC24 FLEXRAY-B"]   = CrcModel{24, 0x5D6DCB, 0xABCDEF, 0, false, false};
        m_models["CRC24"]             = CrcModel{24, 0x864CFB, 0xB704CE, 0, false, false};
        m_models["CRC16 XMODEM"]      = CrcModel{16, 0x1021, 0, 0, false, false};
        m_models["CRC16 X-25"]        = CrcModel{16, 0x1021, 0xFFFF, 0xFFFF, true, true};
        m_models["CRC16 MODBUS"]      = CrcModel{16, 0x8005, 0xFFFF, 0, true, true};
        m_models["CRC16 KERMIT"]      = CrcModel{16, 0x1021, 0, 0, true, true};
        m_models["CRC-A"]             = CrcModel{16, 0x1021, 0xC6C6, 0, true, true};
        m_models["CRC16 USB"]         = CrcModel{16, 0x8005, 0xFFFF, 0xFFFF, true, true};
        m_models["CRC16 TMS37157"]    = CrcModel{16, 0x1021, 0x89EC, 0, true, true};
        m_models["CRC16 TELEDISK"]    = CrcModel{16, 0xA097, 0, 0, false, false};
        m_models["CRC16 T10-DIF"]     = CrcModel{16, 0x8BB7, 0, 0, false, false};
        m_models["CRC16 RIELLO"]      = CrcModel{16, 0x1021, 0xB2AA, 0, true, true};
        m_models["CRC16 MCRF4XX"]     = CrcModel{16, 0x1021, 0xFFFF, 0, true, true};
        m_models["CRC16 MAXIM"]       = CrcModel{16, 0x8005, 0, 0xFFFF, true, true};
        m_models["CRC16 GENIBUS"]     = CrcModel{16, 0x1021, 0xFFFF, 0xFFFF, false, false};
        m_models["CRC16 EN-13757"]    = CrcModel{16, 0x3D65, 0, 0xFFFF, false, false};
        m_models["CRC16 DNP"]         = CrcModel{16, 0x3D65, 0, 0xFFFF, true, true};
        m_models["CRC16 DECT-X"]      = CrcModel{16, 0x0589, 0, 0, false, false};
        m_models["CRC16 DECT-R"]      = CrcModel{16, 0x0589, 0, 0x1, false, false};
        m_models["CRC16 DDS-110"]     = CrcModel{16, 0x8005, 0x800D, 0, false, false};
        m_models["CRC16 CDMA2000"]    = CrcModel{16, 0xC867, 0xFFFF, 0, false, false};
        m_models["CRC16 CCITT-FALSE"] = CrcModel{16, 0x1021, 0xFFFF, 0, false, false};
        m_models["CRC16 BUYPASS"]     = CrcModel{16, 0x8005, 0, 0, false, false};
        m_models["CRC16 AUG-CCITT"]   = CrcModel{16, 0x1021, 0x1D0F, 0, false, false};
        m_models["CRC16 ARC"]         = CrcModel{16, 0x8005, 0, 0, true, true};
        m_models["CRC15 MPT1327"]     = CrcModel{15, 0x6815, 0, 0x1, false, false};
        m_models["CRC15"]             = CrcModel{15, 0x4599, 0, 0, false, false};
        m_models["CRC14 DARC"]        = CrcModel{14, 0x805, 0, 0, true, true};
        m_models["CRC13 BBC"]         = CrcModel{13, 0x1CF5, 0, 0, false, false};
        m_models["CRC12 DECT"]        = CrcModel{12, 0x80F, 0, 0, false, false};
        m_models["CRC12 CDMA2000"]    = CrcModel{12, 0xF13, 0xFFF, 0, false, false};
        m_models["CRC12 3GPP"]        = CrcModel{12, 0x80F, 0, 0, false, true};
        m_models["CRC11"]             = CrcModel{11, 0x385, 0x1A, 0, false, false};
        m_models["CRC10 CDMA2000"]    = CrcModel{10, 0x3D9, 0x3FF, 0, false, false};
        m_models["CRC10"]             = CrcModel{10, 0x233, 0, 0, false, false};
        m_models["CRC8 WCDMA"]        = CrcModel{8, 0x9B, 0, 0, true, true};
        m_models["CRC8 ROHC"]         = CrcModel{8, 0x7, 0xFF, 0, true, true};
        m_models["CRC8 MAXIM"]        = CrcModel{8, 0x31, 0, 0, true, true};
        m_models["CRC8 ITU"]          = CrcModel{8, 0x7, 0, 0x55, false, false};
        m_models["CRC8 I-CODE"]       = CrcModel{8, 0x1D, 0xFD, 0, false, false};
        m_models["CRC8 EBU"]          = CrcModel{8, 0x1D, 0xFF, 0, true, true};
        m_models["CRC8 DVB-S2"]       = CrcModel{8, 0xD5, 0, 0, false, false};
        m_models["CRC8 DARC"]         = CrcModel{8, 0x39, 0, 0, true, true};
        m_models["CRC8 CDMA2000"]     = CrcModel{8, 0x9B, 0xFF, 0, false, false};
        m_models["CRC8"]              = CrcModel{8, 0x7, 0, 0, false, false};
        m_models["CRC7 ROHC"]         = CrcModel{7, 0x4F, 0x7F, 0, true, true};
        m_models["CRC7"]              = CrcModel{7, 0x9, 0, 0, false, false};
        m_models["CRC6 ITU"]          = CrcModel{6, 0x3, 0, 0, true, true};
        m_models["CRC6 DARC"]         = CrcModel{6, 0x19, 0, 0, true, true};
        m_models["CRC6 CDMA2000-A"]   = CrcModel{6, 0x27, 0x3F, 0, false, false};
        m_models["CRC6 CDMA2000-B"]   = CrcModel{6, 0x7, 0x3F, 0, false, false};
        m_models["CRC5 USB"]          = CrcModel{5, 0x5, 0x1F, 0x1F, true, true};
        m_models["CRC5 ITU"]          = CrcModel{5, 0x15, 0, 0, true, true};
        m_models["CRC5 EPC"]          = CrcModel{5, 0x9, 0x9, 0, false, false};
        m_models["CRC4 ITU"]          = CrcModel{4, 0x3, 0x0, 0, true, true};
        m_models["CRC3 ROHC"]         = CrcModel{3, 0x3, 0x7, 0, true, true};

        inited = true;
    }

    return;
}



