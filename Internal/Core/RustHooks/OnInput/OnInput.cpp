#include "OnInput.h"

#include "../../Esp/Visuals.h"
#include "../../Aimbot/Aimbot.h"

double CalcBulletDrop(double height, double DepthPlayerTarget, float velocity, float gravity, float drag) {
//	velocity *= 1 - 0.015625f * drag;

	double pitch = (win32::atan2(height, DepthPlayerTarget));
	double BulletVelocityXY = velocity * win32::cos(pitch);

	double Time = DepthPlayerTarget / BulletVelocityXY;
	double TotalVerticalDrop = (0.5f * gravity * Time * Time) * (1 - 0.015625f * drag);
	return TotalVerticalDrop * 10;
}

void Prediction(Vector3 local, Vector3& target, Vector3 targetvel, float bulletspeed, float gravity, float drag) {

	float Dist = local.get_3d_dist(target);
	float BulletTime = Dist / bulletspeed;

	Vector3 vel = Vector3(targetvel.x, 0, targetvel.z) * 0.75f;

	Vector3 PredictVel = vel * BulletTime;

	target += PredictVel;

	double height = target.y - local.y;
	Vector3 dir = target - local;
	float DepthPlayerTarget = win32::sqrt(powFFFFFFFFFFFFFFFFFFFFFF(dir.x) + powFFFFFFFFFFFFFFFFFFFFFF(dir.z));

	float drop = CalcBulletDrop(height, DepthPlayerTarget, bulletspeed, gravity, drag);

	target.y += drop;
}


void Hooks::OnInput(AssemblyCSharp::BaseMelee* _This)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::OnInputhk.get_original< decltype(&OnInput)>()(_This);

	if (_This->HasAttackCooldown())
		return Hooks::OnInputhk.get_original< decltype(&OnInput)>()(_This);

	if(!_This->IsFullyDeployed())
		return Hooks::OnInputhk.get_original< decltype(&OnInput)>()(_This);

	if (Visuals::m_local_player->lifestate() & RustStructs::Dead || !Visuals::m_local_player->IsConnected())
	{
		return Hooks::OnInputhk.get_original< decltype(&OnInput)>()(_This);
	}

	if(!m_aimbot.m_aimbot.SilentAim)
		return Hooks::OnInputhk.get_original< decltype(&OnInput)>()(_This);

	auto camera = AssemblyCSharp::LocalPlayer::get_Entity()->eyes();
	if (!memory::IsAddressValid(camera))
		return Hooks::OnInputhk.get_original< decltype(&OnInput)>()(_This);

	auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_position(), 500.f);
	if (!m_target.m_player)
		return Hooks::OnInputhk.get_original< decltype(&OnInput)>()(_This);



	if (targetpos.IsZero())
		targetpos = m_target.m_position;


	auto info = _This->GetOwnerItemDefinition();

	auto component = reinterpret_cast<AssemblyCSharp::ItemModProjectile*>(info->GetComponent(System::Type::ItemModProjectile()));
	auto currentProjectileSpread = component->projectileSpread();
	auto currentBodyRotation = AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->bodyRotation();
	auto currentTransformPosition = AssemblyCSharp::LocalPlayer::get_Entity()->get_transform()->get_position();
	auto projectileComponent = component->projectileObject()->Get()->GetComponent<AssemblyCSharp::Projectile>(System::Type::Projectile());


	auto playerInput = AssemblyCSharp::LocalPlayer::get_Entity()->input();

	bool throwReady = false;

	auto inputState = playerInput->state();

	component->projectileSpread() = 0.f;
	component->projectileVelocitySpread() = 0.f;


	auto Distance = Visuals::m_local_player->get_transform()->get_position().Distance(m_target.m_position);

	if (m_options.m_options.AutoShoot && Distance <= 30.f)
	{
		if (AssemblyCSharp::IsVisible(Visuals::m_local_player->eyes()->get_position() + spoofed_eye_pos, targetpos))
		{

			if (!_This->HasAttackCooldown())
			{


				Vector3 Local = AssemblyCSharp::LocalPlayer::get_Entity()->get_bone_transform(RustStructs::bones::head)->get_position();
				Prediction(Local, targetpos, m_target.m_velocity, component->projectileVelocity(), projectileComponent->gravityModifier(), projectileComponent->drag());

				auto posnormal = (targetpos - Local).Normalized();
				Vector4 toQuat = Vector4::QuaternionLookRotation(posnormal, Vector3(0, 1, 0));
				AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->SetBodyRotation(toQuat);

				_This->DoThrow();
				_This->StartAttackCooldown(_This->repeatDelay());
			}
		}
	}
	else if (inputState->IsDown(RustStructs::FIRE_SECONDARY) && inputState->WasJustPressed(RustStructs::FIRE_PRIMARY))
	{
		if (!_This->HasAttackCooldown())
		{
			

			Vector3 Local = AssemblyCSharp::LocalPlayer::get_Entity()->get_bone_transform(RustStructs::bones::head)->get_position();
			Prediction(Local, targetpos, m_target.m_velocity, component->projectileVelocity(), projectileComponent->gravityModifier(), projectileComponent->drag());

			auto posnormal = (targetpos - Local).Normalized();
			Vector4 toQuat = Vector4::QuaternionLookRotation(posnormal, Vector3(0, 1, 0));
			AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->SetBodyRotation(toQuat);

			_This->DoThrow();
			_This->StartAttackCooldown(_This->repeatDelay());
		}
	}

	//if (m_aimbot.m_aimbot.Manipulation && UnityEngine::Input::GetKey(m_aimbot.m_aimbot.ManipKey))
	//{
	//	if (inputState->IsDown(RustStructs::FIRE_SECONDARY) && inputState->WasJustPressed(RustStructs::FIRE_PRIMARY))
	//	{
	//		component->projectileSpread() = 0.f;


	//		Vector3 Local = AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position() + spoofed_eye_pos;
	//		Prediction(Local, m_target.m_position, m_target.m_velocity, component->projectileVelocity(), projectileComponent->gravityModifier(), projectileComponent->drag());

	//		auto posnormal = (m_target.m_position - Local).Normalized();
	//		Vector4 toQuat = Vector4::QuaternionLookRotation(posnormal, Vector3(0, 1, 0));
	//		AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->SetBodyRotation(toQuat);

	//		_This->DoThrow();
	//		_This->StartAttackCooldown(_This->repeatDelay());
	//		//spoofed_eye_pos
	//	}
	//}

	return Hooks::OnInputhk.get_original< decltype(&OnInput)>()(_This);
}