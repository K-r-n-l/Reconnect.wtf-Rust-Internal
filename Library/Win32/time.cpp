#include "time.hpp"
#include "basic.hpp"
#include "trace.hpp"

#include "../core/map_data.hpp"

#include "../memory/operation.hpp"

#include "sdk/large_integer.hpp"
#include "sdk/ularge_integer.hpp"

namespace horizon::win32
{

std::uint32_t GetTickCount32()
{
	const auto user_shared_data = GetUserSharedData();

	if( memory::IsAddressValid( user_shared_data ) )
	{
		const auto tick_count = UInt32x32To64( user_shared_data->TickCount.LowPart, user_shared_data->TickCountMultiplier );
		return static_cast< std::uint32_t >( tick_count >> 24 );
	}
	
	return 0;
}

std::uint64_t GetTickCount64()
{
	const auto user_shared_data = GetUserSharedData();

	if( memory::IsAddressValid( user_shared_data ) )
	{
		const auto tick_count = UInt32x32To64( user_shared_data->TickCount.LowPart, user_shared_data->TickCountMultiplier );
		return static_cast< std::uint64_t >( tick_count >> 24 );
	}

	return 0;
}

std::uintptr_t GetTickCount()
{
#if defined( HORIZON_X32 )
	return GetTickCount32();
#elif defined( HORIZON_X64 )
	return GetTickCount64();
#else
	return 0;
#endif // HORIZON_X32
}

bool GetSystemTimeAsFileTime( FILETIME* file_time )
{
	if( !memory::IsAddressValid( file_time ) )
	{
		return false;
	}

	const auto user_shared_data = GetUserSharedData();

	if( !memory::IsAddressValid( user_shared_data ) )
	{
		return false;
	}

	LARGE_INTEGER system_time = { };

	const auto& time = user_shared_data->SystemTime;

	do
	{
		system_time.LowPart = time.LowPart;
		system_time.HighPart = time.High1Time;
	}
	while( system_time.HighPart != time.High2Time );

	file_time->dwLowDateTime = system_time.LowPart;
	file_time->dwHighDateTime = static_cast< DWORD >( system_time.HighPart );

	return true;
}

std::int16_t GetKeyState( std::int32_t key )
{
	return memory::SecureCall<int16_t>((uintptr_t)g_map_data.NtUserGetKeyState, key );
}

std::int16_t GetAsyncKeyState( std::int32_t key )
{
	return memory::SecureCall<int16_t>((uintptr_t)g_map_data.NtUserGetAsyncKeyState, key );
}

} // namespace horizon::win32