#include "PlayerProjectileAttack.h"

#include "../../Esp/Visuals.h"


double CalcBulletDropzz(double height, double DepthPlayerTarget, float velocity, float gravity, float drag) {
	//	velocity *= 1 - 0.015625f * drag;

	double pitch = (win32::atan2(height, DepthPlayerTarget));
	double BulletVelocityXY = velocity * win32::cos(pitch);

	double Time = DepthPlayerTarget / BulletVelocityXY;
	double TotalVerticalDrop = (0.5f * gravity * Time * Time) * (1 - 0.015625f * drag);
	return TotalVerticalDrop * 10;
}

void Predictionzzz(Vector3 local, Vector3& target, Vector3 targetvel, float bulletspeed, float gravity, float drag) {

	float Dist = local.get_3d_dist(target);
	float BulletTime = Dist / bulletspeed;

	Vector3 vel = Vector3(targetvel.x, 0, targetvel.z) * 0.75f;

	Vector3 PredictVel = vel * BulletTime;

	target += PredictVel;

	double height = target.y - local.y;
	Vector3 dir = target - local;
	float DepthPlayerTarget = win32::sqrt(powFFFFFFFFFFFFFFFFFFFFFF(dir.x) + powFFFFFFFFFFFFFFFFFFFFFF(dir.z));

	float drop = CalcBulletDropzz(height, DepthPlayerTarget, bulletspeed, gravity, drag);

	target.y += drop;
}

void Hooks::PPA_WriteToStream(ProtoBuf::PlayerProjectileAttack* _This, ProtoBuf::Stream* Stream)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::PPA_WriteToStreamhk.get_original< decltype(&PPA_WriteToStream)>()(_This, Stream);

	if (!_This || !Stream)
	{
		return Hooks::PPA_WriteToStreamhk.get_original< decltype(&PPA_WriteToStream)>()(_This, Stream);
	}

	if (Visuals::m_local_player->lifestate() & RustStructs::Dead || !Visuals::m_local_player->IsConnected())
	{
		return Hooks::PPA_WriteToStreamhk.get_original< decltype(&PPA_WriteToStream)>()(_This, Stream);
	}


	auto PlayerAttack = _This->playerAttack();

	if (PlayerAttack) {

		auto attack = PlayerAttack->attack();

		if (attack)
		{

			if (FatBulletInstance)
			{
				PlayerAttack->projectileID() = FatBulletInstance->projectileID();

				_This->hitVelocity() = FatBulletInstance->currentVelocity();
				_This->hitDistance() = FatBulletInstance->traveledDistance();
				_This->travelTime() = FatBulletInstance->traveledTime();
			}

			//if (m_options.m_options.BulletTracers)
			//{
			//	auto BaseProjectile = Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>();
			//	if (memory::IsAddressValid(BaseProjectile))
			//	{
			//		
			//		if (BaseProjectile->IsA(AssemblyCSharp::BaseProjectile::StaticClass()) || BaseProjectile->IsA(AssemblyCSharp::BowWeapon::StaticClass()) || BaseProjectile->IsA(AssemblyCSharp::CrossbowWeapon::StaticClass()) && !BaseProjectile->IsA(AssemblyCSharp::BaseMelee::StaticClass()) && !BaseProjectile->IsA(AssemblyCSharp::ThrownWeapon::StaticClass()))
			//		{

			//			auto PrimaryMagazine = BaseProjectile->primaryMagazine();
			//			if (memory::IsAddressValid(PrimaryMagazine))
			//			{
			//				auto AmmoType = PrimaryMagazine->ammoType();
			//				if (memory::IsAddressValid(AmmoType))
			//				{
			//					AssemblyCSharp::ItemModProjectile* itemModProjectile = AmmoType->GetComponent<AssemblyCSharp::ItemModProjectile>((System::Type*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE(""), SECURE("ItemModProjectile"))));

			//					if (itemModProjectile)
			//					{
			//						auto m_projectile = itemModProjectile->projectileObject()->Get()->GetComponent<AssemblyCSharp::Projectile>(System::Type::Projectile());
			//						if (m_projectile)
			//						{
			//							auto TracerColor = Color{ m_colors.m_visuals.TracersColor[0], m_colors.m_visuals.TracersColor[1], m_colors.m_visuals.TracersColor[2], m_colors.m_visuals.TracersColor[3] };

			//							if (memory::IsAddressValid(Visuals::m_local_player))
			//							{
			//								auto local = Visuals::m_local_player;
			//								//UnityEngine::DDraw::Line(Visuals::m_local_player->eyes()->get_position() + spoofed_eye_pos, attack->hitPositionWorld(), TracerColor, m_options.m_options.TracersDuration, true, true);
			//							}
			//						}

			//					}
			//				}
			//			}
			//		}
			//	}
			//}

			auto camera = UnityEngine::Camera::get_main();
			if (camera) {

				auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 999999.f);
				if (AimbotTarget.m_player) {
					if (!AimbotTarget.m_heli)
					{


						if (m_options.m_options.HitboxOverride)
						{

							int selectedHitbox = m_options.m_options.SelectedHitbox;
							switch (selectedHitbox) {
							case 0: // Head
								attack->hitBone() = 698017942;
								attack->hitPartID() = 2173623152;
								break;
							case 1: // pelvis
								attack->hitBone() = 1031402764;
								attack->hitPartID() = 1750816991;
								break;
							case 2: // Random hitbox
								int randomIndex = my_rand() % 9;
								switch (randomIndex) {
								case 0: // Head
									attack->hitBone() = 698017942;
									attack->hitPartID() = 2173623152;
									break;
								case 1: // pelvis
									attack->hitBone() = 2811218643;
									attack->hitPartID() = 1750816991;
									break;
								case 2: // r_hip
									attack->hitBone() = 2331610670;
									attack->hitPartID() = 1750816991;
									break;
								case 3: // r_foot
									attack->hitBone() = 920055401;
									attack->hitPartID() = 1750816991;
									break;
								case 4: // spine1
									attack->hitBone() = 3771021956;
									attack->hitPartID() = 1750816991;
									break;
								case 5: // l_hand
									attack->hitBone() = 736328754;
									attack->hitPartID() = 1750816991;
									break;
								case 6: // r_upperarm
									attack->hitBone() = 3901657145;
									attack->hitPartID() = 1750816991;
									break;
								case 7: // l_knee
									attack->hitBone() = 3892428003;
									attack->hitPartID() = 1750816991;
									break;
								case 8: // spine4
									attack->hitBone() = 827230707;
									attack->hitPartID() = 1750816991;
									break;
								}
								break;
							}

							attack->hitPositionLocal() = { -.1f, -.1f, 0 };
							attack->hitNormalLocal() = { 0, -1, 0 };
						}

					}
					else
					{

						if (m_options.m_options.HeliHitboxOverride)
						{
							auto weakspots = AimbotTarget.m_player->weakspots();
							if (weakspots)
							{
								auto size = *reinterpret_cast<uintptr_t*>(weakspots + 0x18);

								bool tail_alive = false;
								bool main_alive = false;
								for (int i = 0; i < size; i++) {
									auto weakspot = *(uintptr_t*)(weakspots + 0x20 + i * 0x8);
									if (!weakspot)
										continue;
									auto health = *reinterpret_cast<float*>(weakspot + 0x24);
									if (health > 0) {
										if (i == 0) {
											main_alive = true;
										}
										else {
											tail_alive = true;
										}
									}
								}

								if (tail_alive) {
									attack->hitBone() = 2699525250;
									attack->hitPartID() = 2306822461;
									attack->hitPositionLocal() = { .9f, -.4f, .1f };
									attack->hitNormalLocal() = { .9f, -.4f, .1f };
								}
								else if (main_alive) {
									attack->hitBone() = 224139191;
									attack->hitPartID() = 2306822461;
									attack->hitPositionLocal() = { .9f, -.4f, .1f };
									attack->hitNormalLocal() = { .9f, -.4f, .1f };
								}

							}

						}
						
					}
				}

			}


		
		}
	}



	return Hooks::PPA_WriteToStreamhk.get_original< decltype(&PPA_WriteToStream)>()(_This, Stream);
}