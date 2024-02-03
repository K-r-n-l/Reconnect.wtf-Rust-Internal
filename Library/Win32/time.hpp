#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#include "sdk/large_integer.hpp"

namespace horizon::win32
{

std::uint32_t GetTickCount32();
std::uint64_t GetTickCount64();

std::uintptr_t GetTickCount();

bool GetSystemTimeAsFileTime( FILETIME* file_time );

std::int16_t GetKeyState( std::int32_t key );
std::int16_t GetAsyncKeyState( std::int32_t key );

} // namespace horizon::win32