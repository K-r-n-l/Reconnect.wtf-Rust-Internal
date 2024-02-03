#pragma once

#include "ClientInput.h"

#define M_PI 3.14159265358979323846f

inline void get_cyl_points_z(std::vector<Vector3>& re, float radius = 9.f, unsigned int sectors = 20, unsigned int rings = 20)
{
	const float invRings = 1.0f / static_cast<float>(rings - 1);
	const float invSectors = 1.0f / static_cast<float>(sectors - 1);
	const float halfPi = M_PI / 2;
	re.reserve(rings * sectors * 8);

	for (int r = 0; r < rings; r++) {
		float theta = -halfPi + r * invRings;
		const float sinTheta = win32::sin(theta);
		const float cosTheta = win32::cos(theta);

		float sinPhi, cosPhi;
		for (int s = 0; s < sectors; s++) {
			float phi = 2.0f * M_PI * s * invSectors;
			sinPhi = win32::sin(phi);
			cosPhi = win32::cos(phi);

			float x = cosPhi * sinTheta;
			float y = sinPhi * sinTheta;
			float z = cosTheta;

			x *= radius;
			y *= radius;
			z *= radius;

			re.emplace_back(x, y, z);
			re.emplace_back(x / 2, y / 2, z / 2);
			re.emplace_back(x, y / 2, z / 2);
			re.emplace_back(x, y, z / 2);
			re.emplace_back(x / 2, y, z);

			re.emplace_back(x, -y, z);
			re.emplace_back(-x, -y, z);
			re.emplace_back(x, -y, -z);
			re.emplace_back(-x, y, z);
			re.emplace_back(x, y, -z);
			re.emplace_back(-x, y, -z);
		}
	}
}


static inline float Max3(float f1, float f2, float f3)
{
	return maxx(maxx(f1, f2), f3);
}

static inline float NoClipRadius(float margin)
{
	return 0.5f - margin;
}

inline bool TestNoClipping(AssemblyCSharp::BasePlayer* ply, Vector3 oldPos, Vector3 newPos, float radius, float backtracking) {
	if (!ply)
		return false;



	int num = 429990145;

	Vector3 normalized = (newPos - oldPos).Normalized();
	Vector3 vector = oldPos - normalized * backtracking;
	float magnitude = (newPos - vector).UnityMagnitude();
	UnityEngine::Ray ray = UnityEngine::Ray(vector, normalized);

	UnityEngine::RaycastHit outHitInfo;

	bool flag = UnityEngine::Physics::Raycast(ray, outHitInfo, magnitude + radius, num, RustStructs::QueryTriggerInteraction::Ignore);
	if (!flag)
	{
		flag = UnityEngine::Physics::SphereCast(ray, radius, magnitude, num);
	}

	return flag;
}

inline bool ValidateEyePos(AssemblyCSharp::BasePlayer* player, Vector3 eyePos)
{
	if (!memory::IsAddressValid(player))
		return false;

	if (!memory::IsAddressValid(player->eyes()))
		return false;

	if (!memory::IsAddressValid(player->get_transform()))
		return false;

	bool flag = true;


	float desyncTime = (UnityEngine::Time::get_realtimeSinceStartup() - Visuals::m_local_player->lastSentTickTime());

	float num = 1.5f;
	float eye_clientframes = 2.0f;
	float eye_serverframes = 2.0f;
	float num2 = eye_clientframes / 60.0f;
	float num3 = eye_serverframes * Max3(UnityEngine::Time::get_deltaTime(), UnityEngine::Time::get_smoothDeltaTime(), UnityEngine::Time::get_fixedDeltaTime());
	float num4 = (desyncTime + num2 + num3) * num;
	Vector3 position2 = player->eyes()->get_position();

	float num9 = player->BoundsPadding() + num4 + player->GetJumpHeight();
	float num10 = win32::abs(player->eyes()->get_position().y - eyePos.y);
	if (num10 > num9)
	{
		flag = false;
	}

	

	if (eyePos.Distance(position2) > 0.01f && TestNoClipping(player, position2, eyePos, NoClipRadius(0.21f), 0.01f))
	{
		flag = false;
	}
	return flag;
}

inline void GenerateManipulationAngles(AssemblyCSharp::BasePlayer* local, AssemblyCSharp::BasePlayer* target)
{
	if (targetpos.IsZero())
		targetpos = target->get_bone_transform(47)->get_position();

	if (!memory::IsAddressValid(local) || !memory::IsAddressValid(target) ||
		!memory::IsAddressValid(local->get_transform()) || !memory::IsAddressValid(local->eyes()) ||
		!memory::IsAddressValid(target->get_transform()) || !memory::IsAddressValid(target->model()) ||
		!memory::IsAddressValid(local->modelState()))
	{
		return;
	}

	//Vector3 re_p = local->get_transform()->get_position() + local->get_transform()->get_up() * (local->eyes()->EyeOffset().y + local->eyes()->viewOffset().y);

	auto re_p2 = local->eyes()->get_center();

	if (AssemblyCSharp::IsVisible(re_p2, targetpos))
	{
		spoofed_eye_pos = Vector3(0.f, 0.f, 0.f);
		return;
	}

	std::vector<Vector3> arrz;
	get_cyl_points_z(arrz, m_options.m_options.max_spoofed_eye_distance, m_options.m_options.manipsectors, m_options.m_options.maniprings);

	auto layer = static_cast<int>(RustStructs::Layers::ProjectileLineOfSightCheck) |
		static_cast<int>(RustStructs::Layers::Terrain) |
		static_cast<int>(RustStructs::Layers::EntityLineOfSightCheck) |
		static_cast<int>(RustStructs::Layers::Construction);


	for (const auto& s : arrz)
	{
		if (s.y > 1.6f || s.y < -1.6f)
		{
			continue;
		}

		if (s.z >= 9.f || s.z <= -9.f)
			continue;

		Vector3 point = re_p2 + s;

		//if(!AssemblyCSharp::IsVisible_2(point, targetpos, layer, 0.2f))
		//{
		//	continue;
		//}

		//!AssemblyCSharp::IsVisible(point, targetpos) ||

		if (!ValidateEyePos(local, point) || !ValidateEyePos(local, re_p2))
		{
			continue;
		}

		m_options.m_options.StartShooting = true;
		m_options.m_options.Manipulation_Indicator = true;

		spoofed_eye_pos = s;
		return;
	}

	spoofed_eye_pos = Vector3(0.f, 0.f, 0.f);
	m_options.m_options.StartShooting = false;
	m_options.m_options.Manipulation_Indicator = false;
}