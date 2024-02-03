#include "PlayerProjectileRicochet.h"
#include "../../Esp/Visuals.h"

void Hooks::PlayerProjectileRicochet(ProtoBuf::PlayerProjectileRicochet* _This, ProtoBuf::Stream* _Stream)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::PlayerProjectileRicochethk.get_original< decltype(&PlayerProjectileRicochet)>()(_This, _Stream);

	auto camera = Visuals::m_camera;
	if (!memory::IsAddressValid(camera))
		return Hooks::PlayerProjectileRicochethk.get_original< decltype(&PlayerProjectileRicochet)>()(_This, _Stream);

	auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
	if (!m_target.m_player)
		return Hooks::PlayerProjectileRicochethk.get_original< decltype(&PlayerProjectileRicochet)>()(_This, _Stream);

	auto BaseProjectile = Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>();
	if (!memory::IsAddressValid(BaseProjectile))
		return Hooks::PlayerProjectileRicochethk.get_original< decltype(&PlayerProjectileRicochet)>()(_This, _Stream);

	auto PrimaryMagazine = BaseProjectile->primaryMagazine();
	if (!memory::IsAddressValid(PrimaryMagazine))
		return Hooks::PlayerProjectileRicochethk.get_original< decltype(&PlayerProjectileRicochet)>()(_This, _Stream);

	auto AmmoType = PrimaryMagazine->ammoType();
	if (!memory::IsAddressValid(AmmoType))
		return Hooks::PlayerProjectileRicochethk.get_original< decltype(&PlayerProjectileRicochet)>()(_This, _Stream);

	AssemblyCSharp::ItemModProjectile* itemModProjectile = AmmoType->GetComponent<AssemblyCSharp::ItemModProjectile>((System::Type*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE(""), SECURE("ItemModProjectile"))));

	if (!memory::IsAddressValid(itemModProjectile))
		return Hooks::PlayerProjectileRicochethk.get_original< decltype(&PlayerProjectileRicochet)>()(_This, _Stream);

	auto projectile = itemModProjectile->projectileObject()->Get()->GetComponent<AssemblyCSharp::Projectile>(System::Type::Projectile());
	if (!memory::IsAddressValid(projectile))
		return Hooks::PlayerProjectileRicochethk.get_original< decltype(&PlayerProjectileRicochet)>()(_This, _Stream);



	auto HitScan = [&](Vector3 from) {
		Vector3 head_pos_ = m_target.m_player->get_bone_transform(48)->get_position();

		for (auto bone : { 48, 2, 3, 4, 15, 14, 26, 57 }) {
			Vector3 TargetPosition;
			if (bone == 48) TargetPosition = head_pos_;
			else TargetPosition = m_target.m_player->get_bone_transform(bone)->get_position();
			if (AssemblyCSharp::IsVisible(from, TargetPosition)) {
				return TargetPosition;
			}
		}
		return head_pos_;
	};

	Vector3 hitpos = _This->hitPosition();
	Vector3 normal = _This->hitNormal();
	Vector3 invel = _This->inVelocity();
	Vector3 outvel = _This->outVelocity();

	Vector3 direction = outvel.Normalized();

	auto hs = HitScan(hitpos);


	Vector3 targetpos = hs;


	Vector3 direction_to_target = targetpos - hitpos;

	Vector3 newVelocity = direction_to_target.Normalized() * outvel.Length();

	projectile->currentVelocity() = newVelocity;
	projectile->currentPosition() = (hitpos + projectile->currentVelocity().Normalized()) * 0.001f;

	return Hooks::PlayerProjectileRicochethk.get_original< decltype(&PlayerProjectileRicochet)>()(_This, _Stream);
}