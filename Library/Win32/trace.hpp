#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#include "../constant/character.hpp"
#include "../constant/hash.hpp"
#include "../constant/secure_string.hpp"
#include "../constant/string.hpp"

namespace horizon::win32
{

void DebugMessage( const char* const message );
void DebugMessage( const wchar_t* const message );

int TraceFormat( const char* const format, va_list arg_pack );
int TraceFormat( const wchar_t* const format, va_list arg_pack );

template< typename Type >
int Trace( const Type* format, ... )
{
	va_list arg_pack = { };
	va_start( arg_pack, format );
	const auto result = TraceFormat( format, arg_pack );
	va_end( arg_pack );
	return result;
}

#define TRACE( ... )							


//#define TRACE( Format, ... )			horizon::win32::Trace( SECURE( Format ), __VA_ARGS__ )				


#define TRACE_SEPARATOR()					TRACE( "==================================================" )

} // namespace horizon::win32