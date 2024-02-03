#include "PlayerWalkMovement.h"
#include "../../Esp/Visuals.h"

void Hooks::PlayerWalkMovement(AssemblyCSharp::PlayerWalkMovement* _This, AssemblyCSharp::InputState* _State, AssemblyCSharp::ModelState* _ModelState)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::PlayerWalkMovementhk.get_original< decltype(&PlayerWalkMovement)>()(_This, _State, _ModelState);

	if(!_This || !_State ||!_ModelState)
		return Hooks::PlayerWalkMovementhk.get_original< decltype(&PlayerWalkMovement)>()(_This, _State, _ModelState);

	if (Visuals::m_local_player->lifestate() & RustStructs::Dead || !Visuals::m_local_player->IsConnected())
		return Hooks::PlayerWalkMovementhk.get_original< decltype(&PlayerWalkMovement)>()(_This, _State, _ModelState);


	Hooks::PlayerWalkMovementhk.get_original< decltype(&PlayerWalkMovement)>()(_This, _State, _ModelState);


	if (m_options.m_options.AdminFlags)
		_ModelState->remove_flag(RustStructs::ModelState_Flag::Flying);

	if (m_options.m_options.SilentWalk && UnityEngine::Input::GetKey(m_options.m_options.SilentWalkKey))
		_ModelState->remove_flag(RustStructs::ModelState_Flag::OnGround);

	if (m_options.m_options.LadderShoot)
		_ModelState->remove_flag(RustStructs::ModelState_Flag::OnLadder);

	if (m_options.m_options.InteractiveDebug && UnityEngine::Input::GetKey(m_options.m_options.InteractiveKey))
	{
		_ModelState->remove_flag(RustStructs::ModelState_Flag::Aiming);
		_ModelState->SetMounted(true);
	}

	auto g_local_player = AssemblyCSharp::LocalPlayer::get_Entity();
	if (memory::IsAddressValid(g_local_player))
	{
		if (!_This->flying())
		{
			if (m_options.m_options.OmniSprint)
			{
				Vector3 vel = _This->get_TargetMovement();
				_ModelState->SetSprinting(true);

				float max_speed = (g_local_player->IsSwimming() || _ModelState->get_ducked() > 0.5) ? 1.7f : 5.5f;
				if (vel.Length() > 0.f) {
					Vector3 target_vel = Vector3(vel.x / vel.Length() * max_speed, vel.y, vel.z / vel.Length() * max_speed);
					_This->set_TargetMovement(target_vel);
				}
			}
		}
	}

	if (memory::IsAddressValid(g_local_player))
	{
		if (m_options.m_options.SmallerLocalRadius)
		{
			_This->capsule()->set_radius(0.44);
		}
	}
}