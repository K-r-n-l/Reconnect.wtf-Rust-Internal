#pragma once

#include "../../Globals/Globals.hpp"
#include "../../RustSDK/AssemblyCSharp/AssemblyCSharp.h"
#include "../../RustSDK/UnityEngine/UnityEngine.h"
#include "../../Settings/Settings.hpp"
#include "../../Utils/Bundle.hpp"

inline AssemblyCSharp::BaseProjectile* m_base_projectile = nullptr;


enum HookType
{
	None,
	PointerSwap,
	VirtualFunction
};

class Hook
{
private:
	uintptr_t OriginalPointer = 0;
	void* OurPointer = 0;

	uintptr_t PointerSwapEntry = 0;
	unsigned __int64 VirtualFunctionEntry = 0;

	HookType hookType = HookType::None;
public:
	bool PointerSwapHook(const char* classname, uint64_t function_to_hook, void* our_func, const char* name_space = "", int argCount = -1)
	{
		if (this->IsHooked()) return true;

		if (this->PointerSwapEntry = (uintptr_t)CIl2Cpp::FindMethod(CIl2Cpp::FindClass(name_space, classname), function_to_hook, argCount))
		{
			this->OriginalPointer = *(uintptr_t*)(this->PointerSwapEntry);
			this->OurPointer = our_func;

			*(void**)(this->PointerSwapEntry) = this->OurPointer;

			this->hookType = HookType::PointerSwap;

			return true;
		}
		else
		{
			return false;
		}
	}

	bool VirtualFunctionHook(const char* classname, uint64_t function_to_hook, void* our_func, const char* name_space = "", int argCount = -1)
	{
		if (this->IsHooked()) return true;

		if (auto method = (uintptr_t)CIl2Cpp::FindMethod(CIl2Cpp::FindClass(name_space, classname), function_to_hook, argCount))
		{
			if (auto methodPointer = *(uintptr_t*)(method))
			{
				if (auto table = (uintptr_t)CIl2Cpp::FindClass(name_space, classname))
				{
					if (methodPointer != (uintptr_t)our_func)
					{
						for (this->VirtualFunctionEntry = table; this->VirtualFunctionEntry <= table + 0x1500; this->VirtualFunctionEntry += 0x1)
						{
							auto& VirtualFunction = *(unsigned __int64*)(this->VirtualFunctionEntry);

							if (unsigned __int64 addr = VirtualFunction)
							{
								if (addr == methodPointer)
								{
									VirtualFunction = (unsigned __int64)our_func;

									this->OriginalPointer = addr;
									this->OurPointer = our_func;

									this->hookType = HookType::VirtualFunction;

									return true;
								}
							}
						}
					}
				}
			}
		}

		return false;
	}

	template< typename t>
	auto get_original() -> t
	{
		return reinterpret_cast<t>(this->OriginalPointer);
	}

	void Unhook()
	{
		if (!this->IsHooked()) return;

		switch (this->hookType)
		{
		case HookType::PointerSwap:
			*(void**)(this->PointerSwapEntry) = (void*)this->OriginalPointer;
			break;
		case HookType::VirtualFunction:
			*(unsigned __int64*)(this->VirtualFunctionEntry) = (unsigned __int64)this->OriginalPointer;
			break;
		}

		this->hookType = HookType::None;
	}

	bool IsHooked()
	{
		return this->hookType != HookType::None;
	}
};






#define HOOK( type, name, args ) inline static Hook name##hk; static type name args;

#define SETUP_HOOK(x,y) x = *(decltype(x)*)(y);


inline void(*UpdateFN)(AssemblyCSharp::MainMenuSystem*) = nullptr;
inline void(*ClientInputFN)(AssemblyCSharp::BasePlayer*, AssemblyCSharp::InputState*) = nullptr;

class Hooks {
private:
	HOOK(void, ClientInput, (AssemblyCSharp::BasePlayer* a1, AssemblyCSharp::InputState* a2))
	HOOK(void, OnGUI, (AssemblyCSharp::ExplosionsFPS* _This))
	HOOK(void, Update_, (AssemblyCSharp::MainMenuSystem* _This))
	HOOK(void, PPS_WriteToStream, (ProtoBuf::ProjectileShoot* _This, ProtoBuf::Stream* Stream))
	HOOK(void, OnInput, (AssemblyCSharp::BaseMelee* _This))
	HOOK(void, ProjectileUpdate, (AssemblyCSharp::Projectile* _This))
	HOOK(void, PPA_WriteToStream, (ProtoBuf::PlayerProjectileAttack* _This, ProtoBuf::Stream* Stream))
	HOOK(void, TryToMove, (AssemblyCSharp::ItemIcon* _This))
	HOOK(void, DoFixedUpdate, (AssemblyCSharp::PlayerWalkMovement* _This, AssemblyCSharp::ModelState* _ModelState))
	HOOK(void, PlayerWalkMovement, (AssemblyCSharp::PlayerWalkMovement* _This, AssemblyCSharp::InputState* _State, AssemblyCSharp::ModelState* _ModelState))
	HOOK(void, OnAttacked, (AssemblyCSharp::BasePlayer* instance, AssemblyCSharp::HitInfo* hitinfo))
	HOOK(void, BlockSprint, (AssemblyCSharp::BasePlayer* instance, float duration))
	HOOK(void, DoAttack, (AssemblyCSharp::FlintStrikeWeapon* instance))
	HOOK(void, PlayerProjectileRicochet, (ProtoBuf::PlayerProjectileRicochet* _This, ProtoBuf::Stream* Stream))
	HOOK(void, SCRUpdate, (AssemblyCSharp::SteamClientWrapper* _This))
    HOOK(void, SteamPlatformUpdate, (RustPlatformSteam::SteamPlatform* _This))
    HOOK(void, OnNetworkMessage, (AssemblyCSharp::Client* _This, Network::Message* packet))

		//PlayerIsMounted
	//POINTER(void, OnDisconnected, (void* _This))
	//POINTER(void, OnGUI2, (void* _This))

public:
	static auto Hook() -> void;
	static auto UnHook() -> void;

};