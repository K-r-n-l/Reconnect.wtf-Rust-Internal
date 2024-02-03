#include "trace.hpp"
#include "string.hpp"

#include "../core/map_data.hpp"

namespace horizon::win32
{

void DebugMessage( const char* const message )
{
	if( !memory::IsAddressValid( message ) )
	{
		return;
	}

	memory::SecureCall<void>((uintptr_t)g_map_data.printf, message );
}

void DebugMessage( const wchar_t* const message )
{
	if( !memory::IsAddressValid( message ) )
	{
		return;
	}

	memory::SecureCall<void>((uintptr_t)g_map_data.wprintf_s, message );
}
const std::string currentDateTime() {
	time_t     now = g_map_data.time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *g_map_data.localtime(&now);

	memory::SecureCall<void>((uintptr_t)g_map_data.strftime,buf, sizeof(buf), SECURE("%m/%d/%Y : %X"), &tstruct);
	return buf;
}

int TraceFormat( const char* const format, va_list arg_pack )
{
	char message[ 1024 ] = { };
	vsprintf_s( message, ARRAYSIZE( message ), format, arg_pack );

	char output[ 2048 ] = { };
	sprintf_s( output, ARRAYSIZE( output ), SECURE( "[%s] [debug] %s\n" ), currentDateTime().c_str(), message);

	DebugMessage( output );
	return 0;
}

int TraceFormat( const wchar_t* const format, va_list arg_pack )
{
	wchar_t message[ 1024 ] = { };
	vswprintf_s( message, ARRAYSIZE( message ), format, arg_pack );

	//time(0);

	wchar_t output[ 2048 ] = { };
	swprintf_s( output, ARRAYSIZE( output ), SECURE( L"[%s] [debug] %s\n" ), currentDateTime().c_str(), message );

	DebugMessage( output );
	return 0;
}

}