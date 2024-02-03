#include "map_data.hpp"

#include "../win32/basic.hpp"
#include "../win32/time.hpp"

namespace horizon::core
{

bool MapData::Setup()
{

	const auto image_native = win32::GetImage( HASH( L"ntdll.dll" ) );

	const auto image_winmm = win32::GetImage(HASH(L"Winmm.dll"));


	if (memory::IsUserAddress (image_winmm))
	{ 
		PlaySoundW = (tPlaySoundW)(win32::GetImageExport(image_winmm, SECURE("PlaySoundW")));
	}

	if( memory::IsUserAddress( image_native ) )
	{

		RtlAddFunctionTable = (tRtlAddFunctionTable)(win32::GetImageExport(image_native, SECURE("RtlAddFunctionTable")));//_swprintf
		RtlDeleteFunctionTable = (tRtlDeleteFunctionTable)(win32::GetImageExport(image_native, SECURE("RtlDeleteFunctionTable")));
		RtlAllocateHeap = (tRtlAllocateHeap)(win32::GetImageExport(image_native, SECURE("RtlAllocateHeap")));
		RtlReAllocateHeap = (tRtlReAllocateHeap)(win32::GetImageExport(image_native, SECURE("RtlReAllocateHeap")));
		RtlFreeHeap = (tRtlFreeHeap)(win32::GetImageExport(image_native, SECURE("RtlFreeHeap")));
		RtlQueryPerformanceCounter = (tRtlQueryPerformanceCounter)(win32::GetImageExport(image_native, SECURE("RtlQueryPerformanceCounter")));
		RtlQueryPerformanceFrequency = (tRtlQueryPerformanceFrequency)(win32::GetImageExport(image_native, SECURE("RtlQueryPerformanceFrequency")));
		RtlInitializeCriticalSection = (tRtlInitializeCriticalSection)(win32::GetImageExport(image_native, SECURE("RtlInitializeCriticalSection")));
		RtlInitializeCriticalSectionEx = (tRtlInitializeCriticalSectionEx)(win32::GetImageExport(image_native, SECURE("RtlInitializeCriticalSectionEx")));
		RtlEnterCriticalSection = (tRtlEnterCriticalSection)(win32::GetImageExport(image_native, SECURE("RtlEnterCriticalSection")));
		RtlTryEnterCriticalSection = (tRtlTryEnterCriticalSection)(win32::GetImageExport(image_native, SECURE("RtlTryEnterCriticalSection")));
		RtlLeaveCriticalSection = (tRtlLeaveCriticalSection)(win32::GetImageExport(image_native, SECURE("RtlLeaveCriticalSection")));
		RtlDeleteCriticalSection = (tRtlDeleteCriticalSection)(win32::GetImageExport(image_native, SECURE("RtlDeleteCriticalSection")));


		RtlInitializeConditionVariable = (tRtlInitializeConditionVariable)(win32::GetImageExport(image_native, SECURE("RtlInitializeConditionVariable")));
		RtlSleepConditionVariableCS = (tRtlSleepConditionVariableCS)(win32::GetImageExport(image_native, SECURE("RtlSleepConditionVariableCS")));
		RtlWakeConditionVariable = (tRtlWakeConditionVariable)(win32::GetImageExport(image_native, SECURE("RtlWakeConditionVariable")));
		RtlWakeAllConditionVariable = (tRtlWakeAllConditionVariable)(win32::GetImageExport(image_native, SECURE("RtlWakeAllConditionVariable")));
		
		
		NtClose = (tNtClose)(win32::GetImageExport(image_native, SECURE("NtClose")));
		NtDuplicateObject = (tNtDuplicateObject)(win32::GetImageExport(image_native, SECURE("NtDuplicateObject")));
		NtMakeTemporaryObject = (tNtMakeTemporaryObject)(win32::GetImageExport(image_native, SECURE("NtMakeTemporaryObject")));
		NtQueryObject = (tNtQueryObject)(win32::GetImageExport(image_native, SECURE("NtQueryObject")));
		NtSetInformationObject = (tNtSetInformationObject)(win32::GetImageExport(image_native, SECURE("NtSetInformationObject")));
		NtSignalAndWaitForSingleObject = (tNtSignalAndWaitForSingleObject)(win32::GetImageExport(image_native, SECURE("NtSignalAndWaitForSingleObject")));
		NtWaitForMultipleObjects = (tNtWaitForMultipleObjects)(win32::GetImageExport(image_native, SECURE("NtWaitForMultipleObjects")));

		NtWaitForSingleObject = (tNtWaitForSingleObject)(win32::GetImageExport(image_native, SECURE("NtWaitForSingleObject")));

		NtClearEvent = (tNtClearEvent)(win32::GetImageExport(image_native, SECURE("NtClearEvent")));
		NtCreateEvent = (tNtCreateEvent)(win32::GetImageExport(image_native, SECURE("NtCreateEvent")));
		NtOpenEvent = (tNtOpenEvent)(win32::GetImageExport(image_native, SECURE("NtOpenEvent")));
		NtPulseEvent = (tNtPulseEvent)(win32::GetImageExport(image_native, SECURE("NtPulseEvent")));
		NtResetEvent = (tNtResetEvent)(win32::GetImageExport(image_native, SECURE("NtResetEvent")));

		NtSetEvent = (tNtSetEvent)(win32::GetImageExport(image_native, SECURE("NtSetEvent")));

		NtCreateFile = (tNtCreateFile)(win32::GetImageExport(image_native, SECURE("NtCreateFile")));
		NtReadFile = (tNtReadFile)(win32::GetImageExport(image_native, SECURE("NtReadFile")));
		NtWriteFile = (tNtWriteFile)(win32::GetImageExport(image_native, SECURE("NtWriteFile")));
		NtLockFile = (tNtLockFile)(win32::GetImageExport(image_native, SECURE("NtLockFile")));
		NtUnlockFile = (tNtUnlockFile)(win32::GetImageExport(image_native, SECURE("NtUnlockFile")));
		NtDeleteFile = (tNtDeleteFile)(win32::GetImageExport(image_native, SECURE("NtDeleteFile")));
		NtQueryInformationFile = (tNtQueryInformationFile)(win32::GetImageExport(image_native, SECURE("NtQueryInformationFile")));
		NtQueryDirectoryFile = (tNtQueryDirectoryFile)(win32::GetImageExport(image_native, SECURE("NtQueryDirectoryFile")));
		NtAllocateVirtualMemory = (tNtAllocateVirtualMemory)(win32::GetImageExport(image_native, SECURE("NtAllocateVirtualMemory")));

		NtFreeVirtualMemory = (tNtFreeVirtualMemory)(win32::GetImageExport(image_native, SECURE("NtFreeVirtualMemory")));
		NtLockVirtualMemory = (tNtLockVirtualMemory)(win32::GetImageExport(image_native, SECURE("NtLockVirtualMemory")));
		NtUnlockVirtualMemory = (tNtUnlockVirtualMemory)(win32::GetImageExport(image_native, SECURE("NtUnlockVirtualMemory")));
		NtQueryVirtualMemory = (tNtQueryVirtualMemory)(win32::GetImageExport(image_native, SECURE("NtQueryVirtualMemory")));
		NtProtectVirtualMemory = (tNtProtectVirtualMemory)(win32::GetImageExport(image_native, SECURE("NtProtectVirtualMemory")));
		NtReadVirtualMemory = (tNtReadVirtualMemory)(win32::GetImageExport(image_native, SECURE("NtReadVirtualMemory")));
		NtWriteVirtualMemory = (tNtWriteVirtualMemory)(win32::GetImageExport(image_native, SECURE("NtWriteVirtualMemory")));

		__C_specific_handler = (t__C_specific_handler)(win32::GetImageExport(image_native, SECURE("__C_specific_handler")));


		/*const auto exception_procedure = *reinterpret_cast< std::uintptr_t* >( &__C_specific_handler );
		const auto exception_procedure_pointer = reinterpret_cast< std::uintptr_t* >( &__C_specific_handler );

		if( exception_procedure )
		{
			( *exception_procedure_pointer ) = ( image_native + exception_procedure );
		}
		else
		{
			return false;
		}*/
	}
	else
	{
		return false;
	}

	const auto image_kernel32 = win32::GetImage( HASH( L"kernel32.dll" ) );

	if( memory::IsUserAddress( image_kernel32 ) )
	{
		//OutputDebugStringA = (tOutputDebugStringA)(win32::GetImageExport(image_kernel32, SECURE("OutputDebugStringA")));
		//OutputDebugStringW = (tOutputDebugStringW)(win32::GetImageExport(image_kernel32, SECURE("OutputDebugStringW")));


		//VirtualProtect = (tVirtualProtect)(win32::GetImageExport(image_kernel32, SECURE("VirtualProtect")));

		//VirtualProtectEx = (tVirtualProtectEx)(win32::GetImageExport(image_kernel32, SECURE("VirtualProtectEx")));
	}
	else
	{
		return false;
	}

	/*const auto image_msvcrt = win32::GetImage( HASH( L"msvcrt.dll" ) );

	if( memory::IsUserAddress( image_msvcrt ) )
	{
		malloc = (tmalloc)(win32::GetImageExport(image_msvcrt, SECURE("malloc")));
		free = (tfree)(win32::GetImageExport(image_msvcrt, SECURE("free")));
		memchr = (tmemchr)(win32::GetImageExport(image_msvcrt, SECURE("memchr")));
		memcmp = (tmemcmp)(win32::GetImageExport(image_msvcrt, SECURE("memcmp")));
		memcpy = (tmemcpy)(win32::GetImageExport(image_msvcrt, SECURE("memcpy")));
		mbstowcs = (tmbstowcs)(win32::GetImageExport(image_msvcrt, SECURE("mbstowcs")));

		memmove = (tmemmove)(win32::GetImageExport(image_msvcrt, SECURE("memmove")));
		memset = (tmemset)(win32::GetImageExport(image_msvcrt, SECURE("memset")));

		qsort = (tqsort)(win32::GetImageExport(image_msvcrt, SECURE("qsort")));
		strstr = (tstrstr)(win32::GetImageExport(image_msvcrt, SECURE("strstr")));
		wprintf_s = (twprintf_s)(win32::GetImageExport(image_msvcrt, SECURE("wprintf_s")));
		printf = (tprintf)(win32::GetImageExport(image_msvcrt, SECURE("printf")));
		time = (ttime)(win32::GetImageExport(image_msvcrt, SECURE("time")));
		localtime = (tlocaltime)(win32::GetImageExport(image_msvcrt, SECURE("localtime")));
		strftime = (tstrftime)(win32::GetImageExport(image_msvcrt, SECURE("strftime")));
		wctomb = (twctomb)(tstrftime)(win32::GetImageExport(image_msvcrt, SECURE("wctomb")));
		_vsnprintf_l = (t_vsnprintf_l)(win32::GetImageExport(image_msvcrt, SECURE("_vsnprintf_l")));
		_vsnwprintf_l = (t_vsnwprintf_l)(win32::GetImageExport(image_msvcrt, SECURE("_vsnwprintf_l")));
		strtoul = (tstrtoul)(win32::GetImageExport(image_msvcrt, SECURE("strtoul")));

		fabs = (tfabs)(win32::GetImageExport(image_msvcrt, SECURE("fabs")));
		fmod = (tfmod)(win32::GetImageExport(image_msvcrt, SECURE("fmod")));
		pow = (tpow)(win32::GetImageExport(image_msvcrt, SECURE("pow")));
		atan2 = (tatan2)(win32::GetImageExport(image_msvcrt, SECURE("atan2")));
		ceil = (tceil)(win32::GetImageExport(image_msvcrt, SECURE("ceil")));
		floor = (tfloor)(win32::GetImageExport(image_msvcrt, SECURE("floor")));
		atol = (tatol)(win32::GetImageExport(image_msvcrt, SECURE("atol")));
		atof = (tatof)(win32::GetImageExport(image_msvcrt, SECURE("atof")));

		sscanf = (tsscanf)(win32::GetImageExport(image_msvcrt, SECURE("sscanf")));
	}
	else
	{
		return false;
	}*/

	return true;
}

void MapData::Destroy()
{
	// 
	// reset instance fields
	// 
	std::memset( this, 0, sizeof( MapData ) );
}

bool MapData::IsValid() const
{
	return ( m_signature == HASH( "MapData" ) );
}

bool MapData::IsValidSubscription()
{
	LARGE_INTEGER time = { };
	RtlQueryPerformanceCounter( &time );
	return ( time.QuadPart < m_time.QuadPart );
}

bool MapData::IsValidConditionVariable() const
{
	return true;
}

} // namespace horizon::core

horizon::core::MapData g_map_data = { };