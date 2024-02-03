#include "OnNetworkMessage.h"
#include "../../Esp/Visuals.h"
#include "../../Aimbot/Aimbot.h"


typedef struct Str
{
	char stub[0x10];
	int len;
	wchar_t str[1];
} *str;


void CacheRaidEsp()
{



	auto EffectNetworkClass = CIl2Cpp::FindClass(SECURE(""), SECURE("EffectNetwork"));
	if (!EffectNetworkClass)
		return;

	auto effect = *reinterpret_cast<uintptr_t*>((uintptr_t)EffectNetworkClass + 0xB8);
	if (!effect)
	{
		printfz("\n Effect null...");
		return;
	}

	effect = *reinterpret_cast<uintptr_t*>(effect);
	if (!effect)
	{
		printfz("\n Effect1 null...");
		return;
	}

	auto world_pos = *reinterpret_cast<Vector3*>(effect + 0x64);
	
	auto pooled_str = (str)(*reinterpret_cast<uintptr_t*>(effect + 0x90));
	auto pooledString = pooled_str->str;




	if (!win32::wcsicmp(pooledString, SECURE(L"assets/prefabs/tools/c4/effects/c4_explosion.prefab"))) {
		printfz("\n pooledString - %ls", pooledString);
	}

	

	//if (pooledString.find(SECURE("explosion")) != std::string::npos)
	//{
	//	if (pooledString.find(SECURE("rocket")) != std::string::npos)
	//	{
	//		printfz("\n m_szTranslated: %s", pooledString.c_str());
	//	}
	//	if (pooledString.find(SECURE("c4")) != std::string::npos)
	//	{
	//		printfz("\n m_szTranslated: %s", pooledString.c_str());
	//	}
	//}
}

void Hooks::OnNetworkMessage(AssemblyCSharp::Client* _This, Network::Message* message)
{

	//CacheRaidEsp();

	if(!Visuals::m_local_player)
		return Hooks::OnNetworkMessagehk.get_original< decltype(&OnNetworkMessage)>()(_This, message);


	Hooks::OnNetworkMessagehk.get_original< decltype(&OnNetworkMessage)>()(_This, message);
}