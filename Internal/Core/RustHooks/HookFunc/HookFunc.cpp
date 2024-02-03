#include "HookFunc.h"


void Hooks::Hook()
{
	//SETUP HOOK ORIGINALS//
	SETUP_HOOK(UpdateFN, CIl2Cpp::FindMethod(AssemblyCSharp::MainMenuSystem::StaticClass(), HASH(L"Update"), 0));
	//SETUP_HOOK(ClientInputFN, CIl2Cpp::FindMethod(AssemblyCSharp::BasePlayer::StaticClass(), HASH(L"ClientInput"), 1));
	//SETUP_HOOK(ProtoBuf::ProjectileShoot::WriteToStream_, CIl2Cpp::FindMethod(ProtoBuf::ProjectileShoot::StaticClass(), HASH(L"WriteToStream"), 1));

	//HOOK FUNCTIONS//
	Hooks::Update_hk.PointerSwapHook(SECURE("MainMenuSystem"), HASH("Update"), &Hooks::Update_, SECURE(""), 0);
	Hooks::OnGUIhk.PointerSwapHook(SECURE("PostProcessDebug"), HASH("OnGUI"), &Hooks::OnGUI, SECURE("UnityEngine.Rendering.PostProcessing"), 0);
	//Hooks::SCRUpdatehk.PointerSwapHook(SECURE("SteamClientWrapper"), HASH("Update"), &Hooks::SCRUpdate, SECURE(""), 0);

	//Hooks::OnNetworkMessagehk.VirtualFunctionHook(SECURE("Client"), HASH("OnNetworkMessage"), &Hooks::OnNetworkMessage, SECURE(""), 1);

}

