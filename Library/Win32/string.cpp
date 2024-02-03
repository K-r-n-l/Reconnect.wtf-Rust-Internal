#include "string.hpp"

#include "../core/map_data.hpp"

namespace horizon::win32
{

// 
// [ procedure ] implementation
// 
std::size_t strlen( const char* const string )
{
	std::size_t length = 0;

	while( string[ length ] != '\0' )
		length++;

	return length;
}

std::size_t wcslen( const wchar_t* const string )
{
	std::size_t length = 0;

	while( string[ length ] != L'\0' )
		length++;

	return length;
}

std::size_t strnlen( const char* const string, std::size_t size )
{
	for( std::size_t i = 0; i < size; i++ )
	{
		if( string[ i ] == '\0' )
			return i;
	}

	return size;
}

std::size_t wcsnlen( const wchar_t* const string, std::size_t size )
{
	for( std::size_t i = 0; i < size; i++ )
	{
		if( string[ i ] == L'\0' )
			return i;
	}

	return size;
}

char* strstr(const char* haystack, const char* needle) {
	if (*needle == '\0') {
		return (char*)haystack; // An empty needle is always found in any haystack
	}

	while (*haystack) {
		const char* hay_ptr = haystack;
		const char* needle_ptr = needle;

		// Check if the substring matches starting from the current haystack position
		while (*hay_ptr && *needle_ptr && *hay_ptr == *needle_ptr) {
			hay_ptr++;
			needle_ptr++;
		}

		if (*needle_ptr == '\0') {
			// The entire needle was found in the haystack
			return (char*)haystack;
		}

		// Move to the next position in the haystack and continue searching
		haystack++;
	}

	return NULL; // Substring not found
}

char* strcpy( char* destination, const char* const source )
{
	for( std::size_t i = 0; source[ i ] != '\0'; i++ )
		destination[ i ] = source[ i ];

	return destination;
}

wchar_t* wcscpy( wchar_t* destination, const wchar_t* const source )
{
	for( std::size_t i = 0; source[ i ] != L'\0'; i++ )
		destination[ i ] = source[ i ];

	return destination;
}

char* strncpy( char* destination, const char* const source, std::size_t size )
{
	for( std::size_t i = 0; i < size; i++ )
	{
		if( source[ i ] == '\0' )
			break;

		destination[ i ] = source[ i ];
	}

	return destination;
}

wchar_t* wcsncpy( wchar_t* destination, const wchar_t* const source, std::size_t size )
{
	for( std::size_t i = 0; i < size; i++ )
	{
		if( source[ i ] == L'\0' )
			break;

		destination[ i ] = source[ i ];
	}

	return destination;
}

int strcmp( const char* const left, const char* const right )
{
	std::size_t i = 0;

	while( left[ i ] == right[ i ] )
	{
		if( left[ i ] == '\0' )
			return 0;

		i++;
	}

	return ( left[ i ] - right[ i ] );
}

int wcscmp( const wchar_t* const left, const wchar_t* const right )
{
	std::size_t i = 0;

	while( left[ i ] == right[ i ] )
	{
		if( left[ i ] == L'\0' )
			return 0;

		i++;
	}

	return ( left[ i ] - right[ i ] );
}

int strncmp( const char* const left, const char* const right, std::size_t size )
{
	std::size_t i = 0;

	while( left[ i ] == right[ i ] )
	{
		if( i >= size )
			break;

		if( left[ i ] == '\0' )
			return 0;

		i++;
	}

	return ( left[ i ] - right[ i ] );
}

int wcsncmp( const wchar_t* const left, const wchar_t* const right, std::size_t size )
{
	std::size_t i = 0;

	while( left[ i ] == right[ i ] )
	{
		if( i >= size )
			break;

		if( left[ i ] == L'\0' )
			return 0;

		i++;
	}

	return ( left[ i ] - right[ i ] );
}

char tolower( char character )
{
	if( character >= 'A' && character <= 'Z' )
		return ( character - ( 'A' - 'a' ) );

	return character;
}

wchar_t towlower( wchar_t character )
{
	if( character >= L'A' && character <= L'Z' )
		return ( character - ( L'A' - L'a' ) );

	return character;
}

char toupper( char character )
{
	if( character >= 'a' && character <= 'z' )
		return ( character - ( 'a' - 'A' ) );

	return character;
}

wchar_t towupper( wchar_t character )
{
	if( character >= L'a' && character <= L'z' )
		return ( character - ( L'a' - L'A' ) );

	return character;
}

int stricmp( const char* const left, const char* const right )
{
	std::size_t i = 0;

	while( toupper( left[ i ] ) == toupper( right[ i ] ) )
	{
		if( left[ i ] == '\0' )
			return 0;

		i++;
	}

	return ( toupper( left[ i ] ) - toupper( right[ i ] ) );
}

int wcsicmp( const wchar_t* const left, const wchar_t* const right )
{
	std::size_t i = 0;

	while( towupper( left[ i ] ) == towupper( right[ i ] ) )
	{
		if( left[ i ] == L'\0' )
			return 0;

		i++;
	}

	return ( towupper( left[ i ] ) - towupper( right[ i ] ) );
}

int strnicmp( const char* const left, const char* const right, std::size_t size )
{
	std::size_t i = 0;

	while( toupper( left[ i ] ) == toupper( right[ i ] ) )
	{
		if( i >= size )
			break;

		if( left[ i ] == '\0' )
			return 0;

		i++;
	}

	return ( toupper( left[ i ] ) - toupper( right[ i ] ) );
}

int wcsnicmp( const wchar_t* const left, const wchar_t* const right, std::size_t size )
{
	std::size_t i = 0;

	while( towupper( left[ i ] ) == towupper( right[ i ] ) )
	{
		if( i >= size )
			break;

		if( left[ i ] == L'\0' )
			return 0;

		i++;
	}

	return ( towupper( left[ i ] ) - towupper( right[ i ] ) );
}

int vsnprintf_s( char* const destination, std::size_t size, const char* const format, va_list arg_pack ) noexcept
{
	return 0;
	//return memory::SecureCall<int>((uintptr_t)g_map_data._vsnprintf_l, destination, size, format, nullptr, arg_pack );

	/*if( !memory::IsAddressValid( g_system ) )
	{
		return 0;
	}

	return invoke< int >( g_system->m__vsnprintf_l, destination, size, format, _locale_t( nullptr ), arg_pack );*/
}

int vsnwprintf_s( wchar_t* const destination, std::size_t size, const wchar_t* const format, va_list arg_pack ) noexcept
{
	return 0;
	//return memory::SecureCall<int>((uintptr_t)g_map_data._vsnwprintf_l, destination, size, format, nullptr, arg_pack );

	/*if( !memory::IsAddressValid( g_system ) )
	{
		return 0;
	}

	return invoke< int >( g_system->m__vsnwprintf_l, destination, size, format, _locale_t( nullptr ), arg_pack );*/
}

int vsprintf_s( char* const destination, std::size_t size, const char* const format, va_list arg_list ) noexcept
{
	return vsnprintf_s( destination, size, format, arg_list );
}

int vswprintf_s( wchar_t* const destination, std::size_t size, const wchar_t* const format, va_list arg_list ) noexcept
{
	return vsnwprintf_s( destination, size, format, arg_list );
}

int sprintf_s( char* const destination, std::size_t size, const char* const format, ... ) noexcept
{
	va_list arg_list = { };
	va_start( arg_list, format );
	const int result = vsprintf_s( destination, size, format, arg_list );
	va_end( arg_list );
	return result;
}

int swprintf_s( wchar_t* const destination, std::size_t size, const wchar_t* const format, ... ) noexcept
{
	va_list arg_list = { };
	va_start( arg_list, format );
	const int result = vswprintf_s( destination, size, format, arg_list );
	va_end( arg_list );
	return result;
}

int sscanf( const char* const destination, const char* const format, ... )
{
	va_list arg_pack = { };
	va_start( arg_pack, format );
	// g_map_data.sscanf( destination, format, arg_pack );
	// const int result = g_map_data.sscanf, destination, format, arg_pack );
	va_end( arg_pack );
	// return result;
	return 0;
}

} // namespace horizon::win32