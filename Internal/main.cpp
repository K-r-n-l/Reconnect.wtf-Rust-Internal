#include "Core/CCore.hpp"

int DllMain(void* hModule, unsigned long dwCallReason, void* lpReserved) {
	if (dwCallReason != DLL_PROCESS_ATTACH)
		return true;

	if (g_map_data.Setup()) // would look pretty dope if i'd space it out
	{
		CCore::Init();
	}

	return TRUE;
}

//int __stdcall DllMain(void* module, unsigned long reason, void* reserved) {
//	if (reason == DLL_PROCESS_ATTACH)
//	{
//		wchar_t name[MAX_PATH];
//		GetModuleFileNameW((HMODULE)module, name, MAX_PATH);
//		::LoadLibraryW(name);
//	}
//
//	return TRUE;
//}
//
//
//unsigned long main_thread(void*) {
//	if (g_map_data.Setup()) // would look pretty dope if i'd space it out
//	{
//		CCore::Init();
//	}
//	return 0;
//}
//
//extern "C" __declspec(dllexport)
//int salva_1562346(int code, WPARAM param, LPARAM l_param) {
//
//	static bool done_once = true;
//
//	const auto pmsg = reinterpret_cast<MSG*>(l_param);
//
//	if (done_once) {
//		UnhookWindowsHookEx(reinterpret_cast<HHOOK>(l_param));
//
//		// CreateConsole();
//
//		if (const auto handle = CreateThread(nullptr, 0, &main_thread, nullptr, 0, nullptr); handle != nullptr)
//			CloseHandle(handle);
//
//		done_once = false;
//	}
//
//	return CallNextHookEx(NULL, code, param, l_param);
//}