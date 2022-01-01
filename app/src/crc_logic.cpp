#include "ant/api/crc_logic.hpp"

using namespace ant::api;

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
    : m_width()
    , m_poly()
    , m_seed()
    , m_xor_out()
    , m_ref_in(false)
    , m_ref_out(false)
{

}

CrcLogic::CrcLogic(const CrcModel &a_model)
{
    setModel(a_model);
}

CrcModel CrcLogic::model()
{
    return {m_width, m_poly, m_seed, m_xor_out, m_ref_in, m_ref_out};
}

bool CrcLogic::setModel(const CrcModel &a_model)
{
    bool ret = false;

    if (validateModel(a_model)) {
        m_width       = a_model.width;
        m_poly        = a_model.poly;
        m_seed        = a_model.seed;
        m_xor_out     = a_model.xor_out;
        m_ref_in      = a_model.ref_in;
        m_ref_out     = a_model.ref_out;
        m_result_mask = std::numeric_limits<Crc>::max() >> ( sizeof( Crc )*8u - m_width);

        m_crc = m_seed;

        calculateTable();
        ret = true;
    }

    return ret;
}

bool CrcLogic::validateModel(const CrcModel &a_model)
{
    return a_model.width >= 3 && a_model.width <= 64;
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
    Crc mask = (m_ref_in) ? static_cast<Crc>(1u) :
                            static_cast<Crc>(1u) << ( m_width - 1u );

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
}



