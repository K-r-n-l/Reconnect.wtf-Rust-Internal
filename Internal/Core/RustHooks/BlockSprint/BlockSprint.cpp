#include "BlockSprint.h"
#include "../../Esp/Visuals.h"


void Hooks::BlockSprint(AssemblyCSharp::BasePlayer* instance, float duration)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::BlockSprinthk.get_original< decltype(&BlockSprint)>()(instance, duration);

	if (!instance)
	{
		return Hooks::BlockSprinthk.get_original< decltype(&BlockSprint)>()(instance, duration);
	}

	if (Visuals::m_local_player->lifestate() & RustStructs::Dead || !Visuals::m_local_player->IsConnected())
	{
		return Hooks::BlockSprinthk.get_original< decltype(&BlockSprint)>()(instance, duration);
	}


	if (m_options.m_options.CanAttack)
		return;


	Hooks::BlockSprinthk.get_original< decltype(&BlockSprint)>()(instance, duration);
}