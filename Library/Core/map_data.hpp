#pragma once

#include "../include/auto.hpp"
#include "../include/base.hpp"
#include "../include/win32.hpp"

#include "../memory/operation.hpp"
#include "../memory/secure_call.hpp"
#include "../memory/system_call.hpp"

#include "../win32/sdk/handle.hpp"
#include "../win32/sdk/access_mask.hpp"
#include "../win32/sdk/io_apc_routine.hpp"
#include "../win32/sdk/io_status_block.hpp"
#include "../win32/sdk/object_attributes.hpp"
#include "../win32/sdk/object_information_class.hpp"
#include "../win32/sdk/object_wait_type.hpp"
#include "../win32/sdk/event_type.hpp"
#include "../win32/sdk/image_runtime_function_entry.hpp"
#include "../win32/sdk/file_information_class.hpp"
#include "../win32/sdk/memory_information_class.hpp"
#include "../win32/sdk/rtl_critical_section.hpp"
#include "../win32/sdk/rtl_condition_variable.hpp"
#include "../win32/sdk/unicode_string.hpp"
#include "../win32/sdk/large_integer.hpp"

#include "../win32/basic.hpp"
#include "../win32/file.hpp"
#include "../win32/image.hpp"
#include "../win32/process.hpp"
#include "../win32/string.hpp"
#include "../win32/time.hpp"
#include "../win32/trace.hpp"

namespace horizon::core
{

#define SECURE_IMPORT( Type, Name, ... )									\
	memory::WrapSecureCall< Type, __VA_ARGS__ > Name = { }

#define SYSTEM_IMPORT( Type, Name, ... )									\
	memory::WrapSystemCall< Type, __VA_ARGS__ > Name = { }

using namespace win32;

struct MapData
{
	bool Setup();
	void Destroy();

	bool IsValid() const;
	bool IsValidSubscription();
	bool IsValidConditionVariable() const;

	// 
	// shared data
	// 

	std::uint64_t m_signature = 0;																																										// 0x0000

	std::uintptr_t m_base = 0;																																												// 0x0008
	std::size_t m_size = 0;																																														// 0x0010

	win32::LARGE_INTEGER m_time = { };

	// 
	// native imports
	// 

	typedef bool(WINAPI* tRtlAddFunctionTable)(IMAGE_RUNTIME_FUNCTION_ENTRY*, std::uint32_t, std::uintptr_t);
	tRtlAddFunctionTable RtlAddFunctionTable = nullptr;

	typedef bool(WINAPI* tRtlDeleteFunctionTable)(IMAGE_RUNTIME_FUNCTION_ENTRY*);
	tRtlDeleteFunctionTable RtlDeleteFunctionTable = nullptr;

	typedef void*(WINAPI* tRtlAllocateHeap)(void*, std::uint32_t, std::size_t);
	tRtlAllocateHeap RtlAllocateHeap = nullptr;

	typedef void*(WINAPI* tRtlReAllocateHeap)(void*, std::uint32_t, void*, std::size_t);
	tRtlReAllocateHeap RtlReAllocateHeap = nullptr;

	typedef bool(WINAPI* tRtlFreeHeap)(void*, std::uint32_t, void*);
	tRtlFreeHeap RtlFreeHeap = nullptr;

	typedef bool(WINAPI* tRtlQueryPerformanceCounter)(LARGE_INTEGER*);
	tRtlQueryPerformanceCounter RtlQueryPerformanceCounter = nullptr;

	typedef bool(WINAPI* tRtlQueryPerformanceFrequency)(LARGE_INTEGER*);
	tRtlQueryPerformanceFrequency RtlQueryPerformanceFrequency = nullptr;

	typedef NTSTATUS(WINAPI* tRtlInitializeCriticalSection)(RTL_CRITICAL_SECTION*);
	tRtlInitializeCriticalSection RtlInitializeCriticalSection = nullptr;

	typedef NTSTATUS(WINAPI* tRtlInitializeCriticalSectionEx)(RTL_CRITICAL_SECTION*, std::int32_t, std::uint32_t);
	tRtlInitializeCriticalSectionEx RtlInitializeCriticalSectionEx = nullptr;

	typedef NTSTATUS(WINAPI* tRtlEnterCriticalSection)(RTL_CRITICAL_SECTION*);
	tRtlEnterCriticalSection RtlEnterCriticalSection = nullptr;

	typedef bool(WINAPI* tRtlTryEnterCriticalSection)(RTL_CRITICAL_SECTION*);
	tRtlTryEnterCriticalSection RtlTryEnterCriticalSection = nullptr;

	typedef NTSTATUS(WINAPI* tRtlLeaveCriticalSection)(RTL_CRITICAL_SECTION*);
	tRtlLeaveCriticalSection RtlLeaveCriticalSection = nullptr;

	typedef NTSTATUS(WINAPI* tRtlDeleteCriticalSection)(RTL_CRITICAL_SECTION*);
	tRtlDeleteCriticalSection RtlDeleteCriticalSection = nullptr;

	typedef void(WINAPI* tRtlInitializeConditionVariable)(RTL_CONDITION_VARIABLE*);
	tRtlInitializeConditionVariable RtlInitializeConditionVariable = nullptr;

	typedef NTSTATUS(WINAPI* tRtlSleepConditionVariableCS)(RTL_CONDITION_VARIABLE*, RTL_CRITICAL_SECTION*, LARGE_INTEGER*);
	tRtlSleepConditionVariableCS RtlSleepConditionVariableCS = nullptr;

	typedef void(WINAPI* tRtlWakeConditionVariable)(RTL_CONDITION_VARIABLE*);
	tRtlWakeConditionVariable RtlWakeConditionVariable = nullptr;

	typedef void(WINAPI* tRtlWakeAllConditionVariable)(RTL_CONDITION_VARIABLE*);
	tRtlWakeAllConditionVariable RtlWakeAllConditionVariable = nullptr;

	typedef int(WINAPI* twctomb)(char* s, wchar_t* wc);
	twctomb wctomb = nullptr;


	//int wctomb( char *s, wchar_t wc );

	//wctomb()


	typedef EXCEPTION_DISPOSITION(API_STDCALL* t__C_specific_handler)(EXCEPTION_RECORD*, void*, CONTEXT*, DISPATCHER_CONTEXT*);
	t__C_specific_handler __C_specific_handler = NULL;

	// 
	// kernel32 imports
	// 
	//typedef void(WINAPI* tOutputDebugStringA)(const char* const);
	//tOutputDebugStringA OutputDebugStringA = nullptr;

	//typedef void(WINAPI* tOutputDebugStringW)(const wchar_t* const);
	//tOutputDebugStringW OutputDebugStringW = nullptr;

	typedef int(WINAPI* twprintf_s)(const wchar_t* format, ...);
	twprintf_s wprintf_s = nullptr;

	typedef int(WINAPI* tprintf)(const char* format, ...);
	tprintf printf = nullptr;


	typedef time_t(WINAPI* ttime)(time_t);
	ttime time = nullptr;

	typedef tm*(WINAPI* tlocaltime)(time_t const* const);
	tlocaltime localtime = nullptr;


	typedef size_t(WINAPI* tstrftime)(char*, size_t, char const*, tm const*);
	tstrftime strftime = nullptr;



	//typedef BOOL(WINAPI* tVirtualProtect)(LPVOID, SIZE_T, DWORD,PDWORD);
	//tVirtualProtect VirtualProtect = nullptr;

	//
	//typedef BOOL(WINAPI* tVirtualProtectEx)(HANDLE, LPVOID, SIZE_T, DWORD, PDWORD);
	//tVirtualProtectEx VirtualProtectEx = nullptr;

	/*

	*/
	
	// 
	// msvcrt imports
	// 

	//typedef void*(WINAPI* tmalloc)(std::size_t);
	//tmalloc malloc = nullptr;

	//typedef void (WINAPI* tfree)(void*);
	//tfree free = nullptr;

	//typedef void* (WINAPI* tmemchr)(void*, int, std::size_t);
	//tmemchr memchr = nullptr;

	//typedef int (WINAPI* tmemcmp)(const void*, const void*, std::size_t);
	//tmemcmp memcmp = nullptr;

	//typedef void* (WINAPI* tmemcpy)(void*, const void*, std::size_t);
	//tmemcpy memcpy = nullptr;

	//typedef void* (WINAPI* tmemmove)(void*, void*, std::size_t);
	//tmemmove memmove = nullptr;

	//typedef void* (WINAPI* tmemset)(void*, int, std::size_t);
	//tmemset memset = nullptr;

	//typedef void (WINAPI* tqsort)(void*, std::size_t, std::size_t, int(*)(const void*, const void*));
	//tqsort qsort = nullptr;

	//typedef char* (WINAPI* tstrstr)(const char* const, const char* const);
	//tstrstr strstr = nullptr;

	/*
	_Check_return_
_ACRTIMP unsigned long __cdecl strtoul(
    _In_z_                   char const* _String,
    _Out_opt_ _Deref_post_z_ char**      _EndPtr,
    _In_                     int         _Radix
    );
	*/


	//typedef unsigned long (WINAPI* tstrtoul)(const char* const, char**, int);
	//tstrtoul strtoul = nullptr;


	//typedef int (WINAPI* t_vsnprintf_l)(char* const, std::size_t, const char* const, _locale_t, va_list);
	//t_vsnprintf_l _vsnprintf_l = nullptr;

	//typedef int (WINAPI* t_vsnwprintf_l)(wchar_t* const, std::size_t, const wchar_t* const, _locale_t, va_list);
	//t_vsnwprintf_l _vsnwprintf_l = nullptr;

	//typedef double (WINAPI* tfabs)(double);
	//tfabs fabs = nullptr;

	//typedef double (WINAPI* tfmod)(double, double);
	//tfmod fmod = nullptr;

	//typedef double (WINAPI* tpow)(double, double);
	//tpow pow = nullptr;

	//typedef double (WINAPI* tatan2)(double, double);
	//tatan2 atan2 = nullptr;

	//typedef double (WINAPI* tceil)(double);
	//tceil ceil = nullptr;

	//typedef double (WINAPI* tfloor)(double);
	//tfloor floor = nullptr;

	//typedef long (WINAPI* tatol)(const char* const);
	//tatol atol = nullptr;

	//typedef double (WINAPI* tatof)(const char* const);
	//tatof atof = nullptr;


	//typedef int (WINAPI* tsscanf)(const char* const, const char* const, ...);
	//tsscanf sscanf = nullptr;


	// 
	// object system api
	// 

	typedef std::int16_t(WINAPI* tNtUserGetAsyncKeyState)(std::int32_t);
	tNtUserGetAsyncKeyState NtUserGetAsyncKeyState = nullptr;

	typedef NTSTATUS(WINAPI* tNtClose)(HANDLE);
	tNtClose NtClose = nullptr;

	typedef NTSTATUS(WINAPI* tNtDuplicateObject)(HANDLE, HANDLE*, HANDLE, HANDLE*, ACCESS_MASK, bool, std::uint32_t);
	tNtDuplicateObject NtDuplicateObject = nullptr;

	typedef NTSTATUS(WINAPI* tNtMakeTemporaryObject)(HANDLE);
	tNtMakeTemporaryObject NtMakeTemporaryObject = nullptr;

	typedef NTSTATUS(WINAPI* tNtQueryObject)(HANDLE, OBJECT_INFORMATION_CLASS, void*, std::uint32_t, std::uint32_t*);
	tNtQueryObject NtQueryObject = nullptr;

	typedef NTSTATUS(WINAPI* tNtSetInformationObject)(HANDLE, OBJECT_INFORMATION_CLASS, void*, std::uint32_t);
	tNtSetInformationObject NtSetInformationObject = nullptr;

	typedef NTSTATUS(WINAPI* tNtSignalAndWaitForSingleObject)(HANDLE, HANDLE, bool, LARGE_INTEGER*);
	tNtSignalAndWaitForSingleObject NtSignalAndWaitForSingleObject = nullptr;

	typedef NTSTATUS(WINAPI* tNtWaitForMultipleObjects)(std::uint32_t, HANDLE*, OBJECT_WAIT_TYPE, bool, LARGE_INTEGER*);
	tNtWaitForMultipleObjects NtWaitForMultipleObjects = nullptr;

	typedef NTSTATUS(WINAPI* tNtWaitForSingleObject)(HANDLE, bool, LARGE_INTEGER*);
	tNtWaitForSingleObject NtWaitForSingleObject = nullptr;

	// 
	// event system api
	// 

	typedef NTSTATUS(WINAPI* tNtClearEvent)(HANDLE);
	tNtClearEvent NtClearEvent = nullptr;

	typedef NTSTATUS(WINAPI* tNtCreateEvent)(HANDLE*, ACCESS_MASK, OBJECT_ATTRIBUTES*, EVENT_TYPE, bool);
	tNtCreateEvent NtCreateEvent = nullptr;

	typedef NTSTATUS(WINAPI* tNtOpenEvent)(HANDLE*, ACCESS_MASK, OBJECT_ATTRIBUTES*);
	tNtOpenEvent NtOpenEvent = nullptr;

	typedef NTSTATUS(WINAPI* tNtPulseEvent)(HANDLE, std::int32_t*);
	tNtPulseEvent NtPulseEvent = nullptr;

	typedef NTSTATUS(WINAPI* tNtResetEvent)(HANDLE, std::int32_t*);
	tNtResetEvent NtResetEvent = nullptr;

	typedef NTSTATUS(WINAPI* tNtSetEvent)(HANDLE, std::int32_t*);
	tNtSetEvent NtSetEvent = nullptr;


	// 
	// file system api
	// 

	typedef NTSTATUS(WINAPI* tNtCreateFile)(HANDLE*, ACCESS_MASK, OBJECT_ATTRIBUTES*, IO_STATUS_BLOCK*, LARGE_INTEGER*, std::uint32_t, std::uint32_t, std::uint32_t, std::uint32_t, void*, std::uint32_t);
	tNtCreateFile NtCreateFile = nullptr;

	typedef NTSTATUS(WINAPI* tNtReadFile)(HANDLE, HANDLE, IO_APC_ROUTINE*, void*, IO_STATUS_BLOCK*, void*, std::uint32_t, LARGE_INTEGER*, std::uint32_t*);
	tNtReadFile NtReadFile = nullptr;

	typedef NTSTATUS(WINAPI* tNtWriteFile)(HANDLE, HANDLE, IO_APC_ROUTINE*, void*, IO_STATUS_BLOCK*, void*, std::uint32_t, LARGE_INTEGER*, std::uint32_t*);
	tNtWriteFile NtWriteFile = nullptr;

	typedef NTSTATUS(WINAPI* tNtLockFile)(HANDLE, HANDLE, IO_APC_ROUTINE*, void*, IO_STATUS_BLOCK*, LARGE_INTEGER*, LARGE_INTEGER*, std::uint32_t*, bool, bool);
	tNtLockFile NtLockFile = nullptr;

	typedef NTSTATUS(WINAPI* tNtUnlockFile)(HANDLE, IO_STATUS_BLOCK*, LARGE_INTEGER*, LARGE_INTEGER*, std::uint32_t*);
	tNtUnlockFile NtUnlockFile = nullptr;

	typedef NTSTATUS(WINAPI* tNtDeleteFile)(OBJECT_ATTRIBUTES*);
	tNtDeleteFile NtDeleteFile = nullptr;

	typedef NTSTATUS(WINAPI* tNtQueryInformationFile)(HANDLE, IO_STATUS_BLOCK*, void*, std::uint32_t, FILE_INFORMATION_CLASS);
	tNtQueryInformationFile NtQueryInformationFile = nullptr;

	typedef NTSTATUS(WINAPI* tNtQueryDirectoryFile)(HANDLE, HANDLE, IO_APC_ROUTINE*, void*, IO_STATUS_BLOCK*, void*, std::uint32_t, FILE_INFORMATION_CLASS, std::uint8_t, UNICODE_STRING*, std::uint8_t);
	tNtQueryDirectoryFile NtQueryDirectoryFile = nullptr;


	typedef size_t(WINAPI* tmbstowcs)(wchar_t*, char const*, size_t);
	tmbstowcs mbstowcs = nullptr;

	/*
	
	__DEFINE_CPP_OVERLOAD_STANDARD_NFUNC_0_2_SIZE(
    _ACRTIMP, mbstowcs,
    _Out_writes_opt_z_(_MaxCount), wchar_t,     _Dest,
    _In_z_                         char const*, _Source,
    _In_                           size_t,      _MaxCount
    )
	*/

	// 
	// memory system api
	// 

	typedef NTSTATUS(WINAPI* tNtAllocateVirtualMemory)(HANDLE, void**, std::uintptr_t, std::size_t*, std::uint32_t, std::uint32_t);
	tNtAllocateVirtualMemory NtAllocateVirtualMemory = nullptr;

	typedef NTSTATUS(WINAPI* tNtFreeVirtualMemory)(HANDLE, void**, std::size_t*, std::uint32_t);
	tNtFreeVirtualMemory NtFreeVirtualMemory = nullptr;

	typedef NTSTATUS(WINAPI* tNtLockVirtualMemory)(HANDLE, void**, std::size_t*, std::uint32_t);
	tNtLockVirtualMemory NtLockVirtualMemory = nullptr;

	typedef NTSTATUS(WINAPI* tNtUnlockVirtualMemory)(HANDLE, void**, std::size_t*, std::uint32_t);
	tNtUnlockVirtualMemory NtUnlockVirtualMemory = nullptr;

	typedef NTSTATUS(WINAPI* tNtQueryVirtualMemory)(HANDLE, void*, MEMORY_INFORMATION_CLASS, void*, std::size_t, std::size_t*);
	tNtQueryVirtualMemory NtQueryVirtualMemory = nullptr;

	typedef NTSTATUS(WINAPI* tNtProtectVirtualMemory)(HANDLE, void**, std::size_t*, std::uint32_t, std::uint32_t*);
	tNtProtectVirtualMemory NtProtectVirtualMemory = nullptr;

	typedef NTSTATUS(WINAPI* tNtReadVirtualMemory)(HANDLE, void*, void*, std::size_t, std::size_t*);
	tNtReadVirtualMemory NtReadVirtualMemory = nullptr;

	typedef NTSTATUS(WINAPI* tNtWriteVirtualMemory)(HANDLE, void*, void*, std::size_t, std::size_t*);
	tNtWriteVirtualMemory NtWriteVirtualMemory = nullptr;

	typedef NTSTATUS(WINAPI* tPlaySoundW)(LPCWSTR, HMODULE, DWORD);
	tPlaySoundW PlaySoundW = nullptr;
	// 
	// win32 user api
	// 

	typedef  std::int32_t(WINAPI* tNtUserGetKeyNameText)(std::int32_t, const wchar_t* const, std::int32_t);
	tNtUserGetKeyNameText NtUserGetKeyNameText = nullptr;

	typedef  std::int16_t(WINAPI* tNtUserGetKeyState)(std::int32_t);
	tNtUserGetKeyState NtUserGetKeyState = nullptr;

	typedef  std::uint32_t(WINAPI* tNtUserMapVirtualKeyEx)(std::int32_t, std::uint32_t, std::uintptr_t, std::uint32_t);
	tNtUserMapVirtualKeyEx NtUserMapVirtualKeyEx = nullptr;
};

} // namespace horizon::core

extern horizon::core::MapData g_map_data;