#include "TryToMove.h"
#include "../../Esp/Visuals.h"

void Hooks::TryToMove(AssemblyCSharp::ItemIcon* _This)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::TryToMovehk.get_original< decltype(&TryToMove)>()(_This);

	if(!_This)
		return Hooks::TryToMovehk.get_original< decltype(&TryToMove)>()(_This);

	if (Visuals::m_local_player->lifestate() & RustStructs::Dead || !Visuals::m_local_player->IsConnected())
	{
		return Hooks::TryToMovehk.get_original< decltype(&TryToMove)>()(_This);
	}

	Hooks::TryToMovehk.get_original< decltype(&TryToMove)>()(_This);

	if (m_options.m_options.FastLoot) {

		if (_This->queuedForLooting())
		{
			_This->RunTimedAction();
		}
	}
}