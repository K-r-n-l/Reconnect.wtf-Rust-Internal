#include "randomizer.hpp"

namespace horizon::constant
{
	constexpr auto rand_max = 0x7fff;
	uint32_t m_temp_seed = 4576;

	int update_seed()
	{
		m_temp_seed = ( 8253729 * m_temp_seed + 2396403 );
		return m_temp_seed;
	}

	float randomf()
	{
		return ( update_seed() >> 16 ) & 0x7fff;
	}

	float c_randf( float min, float max )
	{
		return ( ( max - min ) * ( ( float )randomf() / RAND_MAX ) ) + min;
	}

}