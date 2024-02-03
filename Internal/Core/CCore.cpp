#include "CCore.hpp"
//#include "Renderer/Present.hpp"
#include "Sdk/Il2Cpp/CIl2Cpp.hpp"
#include "RustHooks/HookFunc/HookFunc.h"

auto CCore::CreateConsole() -> bool {
	if (DEBUG_MODE == 1)
	{
		uintptr_t msvcrt = win32::GetImage(SECURE(L"msvcrt.dll"));
		if (!memory::IsAddressValid(msvcrt))
			return false;

		printfz = (Proto_printf)(win32::GetImageExport(msvcrt, SECURE("printf")));
		freopenz = (Proto_freopen)(win32::GetImageExport(msvcrt, SECURE("freopen")));
		iob_funcz = (Proto_iob_func)(win32::GetImageExport(msvcrt, SECURE("__iob_func")));

		uintptr_t kenr = win32::GetImage(SECURE(L"kernel32.dll"));
		if (!memory::IsAddressValid(kenr))
			return false;

		crt_sprintf = (sprintf_szz)(win32::GetImageExport(msvcrt, SECURE("_vsnprintf_l")));
		alloc_console = (f_alloc_console)(win32::GetImageExport(kenr, SECURE("AllocConsole")));
		alloc_console();

		FILEz* _stdout = stdout;
		freopenz(SECURE("CONOUT$"), SECURE("w"), _stdout);
	}

	return true;
}

#define SETUP(x,y) x = *(decltype(x)*)(y);

auto CCore::Init() -> bool
{
	if (!CCore::CreateConsole())
		return false;

	m_game_assembly = win32::GetImage(HASH(L"GameAssembly.dll"));
	if (!m_game_assembly)
		return false;

	if (!CIl2Cpp::CreateIL2CPP())
		return false;

	
	Hooks::Hook();

	return true;
}