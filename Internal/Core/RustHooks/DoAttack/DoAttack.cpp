#include "DoAttack.h"
#include "../../Esp/Visuals.h"


void Hooks::DoAttack(AssemblyCSharp::FlintStrikeWeapon* instance)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::DoAttackhk.get_original< decltype(&DoAttack)>()(instance);

	if (!instance)
	{
		return Hooks::DoAttackhk.get_original< decltype(&DoAttack)>()(instance);
	}


	if (Visuals::m_local_player->lifestate() & RustStructs::Dead || !Visuals::m_local_player->IsConnected())
	{
		return Hooks::DoAttackhk.get_original< decltype(&DoAttack)>()(instance);
	}


	if (m_options.m_options.InstantEoka)
	{


		auto playerInput = AssemblyCSharp::LocalPlayer::get_Entity()->input();

		bool throwReady = false;

		auto inputState = playerInput->state();
		if (!inputState->IsDown(RustStructs::FIRE_PRIMARY) || !inputState->WasJustPressed(RustStructs::FIRE_PRIMARY))
		{
			return Hooks::DoAttackhk.get_original< decltype(&DoAttack)>()(instance);
		}

		
		auto magazine = instance->primaryMagazine();
		auto contents = magazine->contents();

		if (contents <= 0) {
			return;
		}

		magazine->contents() = contents - 1;
		float kd = instance->repeatDelay() + 0.1f;
		instance->StartAttackCooldown(kd);
		auto viewmodel = instance->viewModel();
		if (viewmodel)
		{
			viewmodel->Play(SECURE("attack"), 0);
		}

		instance->LaunchProjectile();

		instance->UpdateAmmoDisplay();
		instance->DidAttackClientside();
	}
	else {
		return Hooks::DoAttackhk.get_original< decltype(&DoAttack)>()(instance);
	}
}