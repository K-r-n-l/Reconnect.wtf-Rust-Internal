#include "DoFixedUpdate.h"
#include "../../Esp/Visuals.h"

void Hooks::DoFixedUpdate(AssemblyCSharp::PlayerWalkMovement* _This, AssemblyCSharp::ModelState* _ModelState)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::DoFixedUpdatehk.get_original< decltype(&DoFixedUpdate)>()(_This, _ModelState);

	if (!_This || !_ModelState)
	{
		return Hooks::DoFixedUpdatehk.get_original< decltype(&DoFixedUpdate)>()(_This, _ModelState);
	}

	if (Visuals::m_local_player->lifestate() & RustStructs::Dead || !Visuals::m_local_player->IsConnected())
	{
		return Hooks::DoFixedUpdatehk.get_original< decltype(&DoFixedUpdate)>()(_This, _ModelState);
	}


	Hooks::DoFixedUpdatehk.get_original< decltype(&DoFixedUpdate)>()(_This, _ModelState);
}