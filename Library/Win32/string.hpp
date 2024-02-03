#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#include "sdk/string.hpp"
#include "sdk/unicode_string.hpp"

#include "sdk/object_attributes.hpp"

#if defined( InitializeObjectAttributes )
#undef InitializeObjectAttributes
#endif // InitializeObjectAttributes

namespace horizon::win32
{

std::size_t strlen( const char* const string );
std::size_t wcslen( const wchar_t* const string );

std::size_t strnlen( const char* const string, std::size_t size );
std::size_t wcsnlen( const wchar_t* const string, std::size_t size );

char* strstr( const char* _Str, char const* _SubStr );

char* strcpy( char* destination, const char* const source );
wchar_t* wcscpy( wchar_t* destination, const wchar_t* const source );

char* strncpy( char* destination, const char* const source, std::size_t size );
wchar_t* wcsncpy( wchar_t* destination, const wchar_t* const source, std::size_t size );

int strcmp( const char* const left, const char* const right );
int wcscmp( const wchar_t* const left, const wchar_t* const right );

int strncmp( const char* const left, const char* const right, std::size_t size );
int wcsncmp( const wchar_t* const left, const wchar_t* const right, std::size_t size );

char tolower( char character );
wchar_t towlower( wchar_t character );

char toupper( char character );
wchar_t towupper( wchar_t character );

int stricmp( const char* const left, const char* const right );
int wcsicmp( const wchar_t* const left, const wchar_t* const right );

int strnicmp( const char* const left, const char* const right, std::size_t size );
int wcsnicmp( const wchar_t* const left, const wchar_t* const right, std::size_t size );

int vsnprintf_s( char* const destination, std::size_t size, const char* const format, va_list arg_list ) noexcept;
int vsnwprintf_s( wchar_t* const destination, std::size_t size, const wchar_t* const format, va_list arg_list ) noexcept;

int vsprintf_s( char* const destination, std::size_t size, const char* const format, va_list arg_list ) noexcept;
int vswprintf_s( wchar_t* const destination, std::size_t size, const wchar_t* const format, va_list arg_list ) noexcept;

int sprintf_s( char* const destination, std::size_t size, const char* const format, ... ) noexcept;
int swprintf_s( wchar_t* const destination, std::size_t size, const wchar_t* const format, ... ) noexcept;

int sscanf( const char* const destination, const char* const format, ... );

} // namespace horizon::win32