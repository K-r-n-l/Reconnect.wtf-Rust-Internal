#include "OnAttacked.h"
#include "../../Esp/Visuals.h"

void Hooks::OnAttacked(AssemblyCSharp::BasePlayer* instance, AssemblyCSharp::HitInfo* hitinfo)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::OnAttackedhk.get_original< decltype(&OnAttacked)>()(instance, hitinfo);

	if (!instance || !hitinfo)
	{
		return Hooks::OnAttackedhk.get_original< decltype(&OnAttacked)>()(instance, hitinfo);
	}

	if (Visuals::m_local_player->lifestate() & RustStructs::Dead || !Visuals::m_local_player->IsConnected())
	{
		return Hooks::OnAttackedhk.get_original< decltype(&OnAttacked)>()(instance, hitinfo);
	}	

	if (m_options.m_options.HitboxOverride)
	{
		int selectedHitbox = m_options.m_options.SelectedHitbox;

		if (selectedHitbox == 0)
		{
			hitinfo->HitBone() = AssemblyCSharp::StringPool::Get(SECURE("Head"));
		}
	}

	if (m_options.m_options.HitMaterial == 0) //flesh
	{
		auto stringpol = AssemblyCSharp::StringPool::Get(SECURE("Flesh"));
		hitinfo->HitMaterial() = stringpol;
	}
	else if (m_options.m_options.HitMaterial == 1) //glass
	{
		auto stringpol = AssemblyCSharp::StringPool::Get(SECURE("Glass"));
		hitinfo->HitMaterial() = stringpol;
	}
	else if (m_options.m_options.HitMaterial == 2) //metal
	{
		auto stringpol = AssemblyCSharp::StringPool::Get(SECURE("Metal"));
		hitinfo->HitMaterial() = stringpol;
	}
	else if (m_options.m_options.HitMaterial == 3) //water
	{
		auto stringpol = AssemblyCSharp::StringPool::Get(SECURE("Water"));
		hitinfo->HitMaterial() = stringpol;
	}

	if (m_options.m_options.Hitsound)
		g_map_data.PlaySoundW(SECURE(L"C:\\hitsound.wav"), NULL, 0x0001);

	Hooks::OnAttackedhk.get_original< decltype(&OnAttacked)>()(instance, hitinfo);
}