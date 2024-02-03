#include "Aimbot.h"
#include "../Esp/Visuals.h"
#include "../Settings/Settings.hpp"
#include "../RustHooks/ClientInput/Manipulation.h"

inline bool is_melee = false;

//struct BulletDropPredictionData
//{
//	float distCoeff;
//	float startY;
//	float yCoeff;
//};
//
//BulletDropPredictionData bulletDropData[11];
//
//#define M_PI 3.14159265358979323846f
//#define M_PI_F ((float)(M_PI))
//#define DEG2RAD(val) ((float)(val) * (float)(M_PI_F / 180.0f))
//#define RAD2DEG(val) ((float)(val) * (float)(180.0f / M_PI_F))
//
//Vector3 GetEndPointForTrajectory(float speed, float angle, float drag, float gravityMod)
//{
//	float pitchRad = DEG2RAD(angle);
//
//	Vector3 dir = {
//		(float)(win32::sinf(90.f) * win32::cosf(pitchRad)),
//		(float)win32::sinf(pitchRad),
//		(float)(win32::cosf(90.f) * win32::cosf(pitchRad))
//	};
//
//	Vector3 position = Vector3();
//	Vector3 velCheck = dir * speed;
//
//	const float stepSize = 0.03125f;
//
//	for (float travelTime = 0.f; travelTime < 8.f; travelTime += stepSize)
//	{
//		position += velCheck * stepSize;
//		velCheck.y -= 9.81f * gravityMod * stepSize;
//		velCheck -= velCheck * drag * stepSize;
//	}
//
//	return position;
//}
//
//void GenerateBuilletDropPredictionData(float drag, float gravityMod)
//{
//	int currentIndex = 0;
//	for (float angle = 35.f; angle <= 85.f; angle += 5.f)
//	{
//		BulletDropPredictionData& predData = bulletDropData[currentIndex++];
//
//		Vector3 a1 = GetEndPointForTrajectory(30.f, angle, drag, gravityMod);
//		Vector3 a2 = GetEndPointForTrajectory(50.f, angle, drag, gravityMod);
//
//		predData.distCoeff = a2.Length2D() / 50.f;
//		predData.startY = a1.y;
//		predData.yCoeff = (a2.y - a1.y) / 20.f;
//	}
//}
//
//inline Vector2 CalcAngle(const Vector3& Src, const Vector3& Dst)
//{
//	Vector3 dir = Src - Dst;
//	return Vector2{ RAD2DEG(win32::asin(dir.y / dir.Length())), RAD2DEG(-win32::atan2(dir.x, -dir.z)) };
//}
//
//auto cameraPos = UnityEngine::Camera::get_main()->get_positionz();
//auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(cameraPos, 500.f);
//
//Vector2 angle = CalcAngle(cameraPos, AimbotTarget.m_player->get_bone_transform(47)->get_position());
//
//float yRad = DEG2RAD(angle.y);
//
//int currentIndex = 0;


namespace O::Projectile {
	constexpr auto initialVelocity = 0x18;
	constexpr auto drag = 0x24;
	constexpr auto gravityModifier = 0x28;
	constexpr auto thickness = 0x2C;
	constexpr auto initialDistance = 0x30;
	constexpr auto remainInWorld = 0x34;
	constexpr auto stickProbability = 0x38;
	constexpr auto breakProbability = 0x3C;
	constexpr auto conditionLoss = 0x40;
	constexpr auto ricochetChance = 0x44;
	constexpr auto penetrationPower = 0x48;
	constexpr auto waterIntegrityLoss = 0x4C;
	constexpr auto damageProperties = 0x50;
	constexpr auto damageDistances = 0x58;
	constexpr auto createDecals = 0x80;
	constexpr auto doDefaultHitEffects = 0x81;
	constexpr auto flybySound = 0x88;
	constexpr auto flybySoundDistance = 0x90;
	constexpr auto closeFlybySound = 0x98;
	constexpr auto closeFlybyDistance = 0xA0;
	constexpr auto tumbleSpeed = 0xA4;
	constexpr auto tumbleAxis = 0xA8;
	constexpr auto swimScale = 0xB4;
	constexpr auto swimSpeed = 0xC0;
	constexpr auto owner = 0xD0;
	constexpr auto sourceWeaponPrefab = 0xD8;
	constexpr auto sourceProjectilePrefab = 0xE0;
	constexpr auto mod = 0xE8;
	constexpr auto projectileID = 0xF0;
	constexpr auto seed = 0xF4;
	constexpr auto clientsideEffect = 0xF8;
	constexpr auto clientsideAttack = 0xF9;
	constexpr auto integrity = 0xFC;
	constexpr auto maxDistance = 0x100;
	constexpr auto modifier = 0x104;
	constexpr auto invisible = 0x114;
	constexpr auto currentVelocity = 0x118;
	constexpr auto currentPosition = 0x124;
	constexpr auto traveledDistance = 0x130;
	constexpr auto traveledTime = 0x134;
	constexpr auto launchTime = 0x138;
	constexpr auto sentPosition = 0x13C;
	constexpr auto previousPosition = 0x148;
	constexpr auto previousVelocity = 0x154;
	constexpr auto previousTraveledTime = 0x160;
	constexpr auto isUnderwater = 0x164;
	constexpr auto isRicochet = 0x165;
	constexpr auto isRetiring = 0x166;
	constexpr auto flybyPlayed = 0x167;
	constexpr auto wasFacingPlayer = 0x168;
	constexpr auto flybyPlane = 0x16C;
	constexpr auto flybyRay = 0x17C;
	constexpr auto cleanupAction = 0x198;
	constexpr auto hitTest = 0x1A0;
	constexpr auto swimRandom = 0x1A8;
	constexpr auto _waterMaterialID = 0x4;
	constexpr auto cachedWaterString = 0x8;
};
#define safe_read(Addr, Type) memory::Read<Type>((DWORD64)Addr)
#define safe_write(Addr, Data, Type) memory::Write<Type>((DWORD64)Addr, Data);

class Projectile_c : AssemblyCSharp::Projectile {
public:
	IL2CPP_CLASS("Projectile");
	IL2CPP_FIELD(float, thickness);
	IL2CPP_FIELD(Vector3, currentPosition);
	IL2CPP_FIELD(Vector3, sentPosition);

	IL2CPP_FIELD(Vector3, currentVelocity);
	IL2CPP_FIELD(Vector3, initialVelocity);
	IL2CPP_FIELD(Vector3, previousPosition);
	IL2CPP_FIELD(Vector3, previousVelocity);
	IL2CPP_FIELD(AssemblyCSharp::HitTest*, hitTest);
	IL2CPP_FIELD(float, traveledTime);
	IL2CPP_FIELD(float, traveledDistance);
	IL2CPP_FIELD(float, gravityModifier);
	IL2CPP_FIELD(float, drag);
	IL2CPP_FIELD(float, closeFlybyDistance);
	IL2CPP_FIELD(float, tumbleSpeed);
	IL2CPP_FIELD(float, maxDistance);
	IL2CPP_FIELD(bool, createDecals);
	IL2CPP_FIELD(Vector3, tumbleAxis);
	IL2CPP_FIELD(float, previousTraveledTime);
	IL2CPP_FIELD(float, launchTime);
	IL2CPP_FIELD(Vector3, swimScale);
	IL2CPP_FIELD(float, penetrationPower);
	IL2CPP_FIELD(int, seed);
	IL2CPP_FIELD(float, flybySoundDistance);

	IL2CPP_FIELD(int, projectileID);
	//IL2CPP_FIELD(ItemModProjectile*, mod);
	IL2CPP_FIELD(float, initialDistance);
	//IL2CPP_FIELD(ItemModProjectile*, mod);
	IL2CPP_FIELD(float, integrity);
	IL2CPP_FIELD(AssemblyCSharp::BasePlayer*, owner);
	IL2CPP_FIELD(bool, isRicochet);
	IL2CPP_FIELD(AssemblyCSharp::AttackEntity*, sourceWeaponPrefab);
	IL2CPP_FIELD(AssemblyCSharp::DamageProperties*, damageProperties);
	IL2CPP_FIELD(float, ricochetChance);

	bool IsAlive() {
		return (this->integrity() > 0.001f && this->traveledDistance() < this->maxDistance() && this->traveledTime() < 8);
	}

	struct TraceInfo {

		bool valid; // 0x0
		float distance; // 0x4
		DWORD64 entity; // 0x8  
		Vector3 point; // 0x10
		Vector3 normal; // 0x1C
		DWORD64/*Transform*/ bone; // 0x28
		DWORD64 material; // 0x30
		unsigned int partID; // 0x38
		DWORD64 collider; // 0x40
	};
	inline Vector3 SimulateProjectilezzzzz(Vector3& position, Vector3& velocity, float& partialTime, float travelTime, Vector3 gravity, float drag)
	{
		float num = 0.03125f;
		Vector3 origin = position;
		if (partialTime > 0)
		{
			float num2 = num - partialTime;
			if (travelTime < num2)
			{
				origin = position;
				position += velocity * travelTime;
				partialTime += travelTime;
				return origin;
			}
			origin = position;
			position += velocity * num2;
			velocity += gravity * num;
			velocity -= velocity * drag * num;
			travelTime -= num2;
		}

		int num3 = int(travelTime / num);

		for (int i = 0; i < num3; i++)
		{
			origin = position;
			position += velocity * num;
			velocity += gravity * num;
			velocity -= velocity * drag * num;
		}
		partialTime = travelTime - num * (float)num3;
		if (partialTime > 0)
		{
			origin = position;
			position += velocity * partialTime;
		}
		return origin;
	}

	inline Vector3 Simulatezzzzz(bool returnvelocity, bool sendtoserver) {
		Vector3 pos = sentPosition(); Vector3 prev = tumbleAxis(); float part = tumbleSpeed(); float travel = maxx(traveledTime() - closeFlybyDistance(), 0);

		Vector3 gr = UnityEngine::Physics::get_gravity(); //static Vector3 get_gravity();


		Vector3 origin = SimulateProjectilezzzzz(pos, prev, part, travel, gr * gravityModifier(), drag());

		if (sendtoserver) {
			sentPosition() = (pos);
			tumbleAxis() = (prev);
			tumbleSpeed() = (part);
			closeFlybyDistance() = (traveledTime());
		}

		if (returnvelocity) return prev;

		return origin;
	}


	inline float GetHitDist() {
		float travel = traveledTime();

		float num6 = (travel + 2 / 60 + 0.03125f) * 1.5f;
		float maxdist = 0.1f + num6 * 5.5f;
		return maxdist;
	}

	bool BulletTP(AssemblyCSharp::Projectile* instance, Vector3 NextCurrentPosition, Vector3 CurrentPosition, Vector3 CurrentVelocity, float deltaTime)
	{

		if (RPC_Counter.Calculate() > 100)
		{
			return false;
		}

		auto camera = UnityEngine::Camera::get_main();
		if (!camera)
			return false;


		auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
		if (!m_target.m_player)
			return false;

		if (m_target.m_heli)
			return false;




		auto Line = NextCurrentPosition - CurrentPosition;
		auto LineLength = Line.Magnitude();
		Line.UnityNormalize();

		auto CenterPosition = m_target.m_player->PivotPoint() + Vector3(0.f, 0.9f, 0.f);
		auto v = CenterPosition - CurrentPosition;
		auto d = v.Dot(Line);

		if (d < 0.0f)
		{
			d = 0.0f;
		}
		else if (d > LineLength)
		{
			d = LineLength;
		}

		auto OriginalClosestPointOnLine = CurrentPosition + Line * d;
		auto ClosestPointOnLine = OriginalClosestPointOnLine;

		auto ClosestPoint = m_target.m_player->ClosestPoint(ClosestPointOnLine);
		auto OriginalDistance = m_target.m_player->Distance(ClosestPointOnLine);
		auto Distance = OriginalDistance;

		Vector3 HitPointWorld = ClosestPoint;


		if (m_target.m_player->mounted())
		{
			if (Distance > 7.5f)
			{
				return false;
			}
		}
		else
		{
			if (Distance > 3.5f)
			{
				return false;
			}
		}




		if (Distance > 1.2f)
		{
			auto endPositionTrajectoryUsage = Distance - 1.2f + 0.01f;
			auto amountNeeded = endPositionTrajectoryUsage / Distance;
			auto direction = HitPointWorld - ClosestPointOnLine;
			auto newPosition = ClosestPointOnLine + (direction * amountNeeded);

			if (ClosestPointOnLine.Distance(newPosition) > 1.f || !AssemblyCSharp::IsVisible_2(ClosestPointOnLine, newPosition, 10551296, 0.f))
				return false;

			ClosestPointOnLine = newPosition;
			Distance = m_target.m_player->Distance(ClosestPointOnLine);
		}

		if (Distance > 1.f)
		{
			auto playerDistanceUsage = minm(0.19f, Distance - 1.f);
			auto amountNeeded = 1.f - playerDistanceUsage / Distance;
			auto direction = HitPointWorld - ClosestPointOnLine;
			auto newPosition = ClosestPointOnLine + (direction * amountNeeded);

			HitPointWorld = newPosition;
		}

		if (!AssemblyCSharp::IsVisible_2(ClosestPointOnLine, HitPointWorld, 10551296, 0.f) || !AssemblyCSharp::IsVisible_2(CurrentPosition, OriginalClosestPointOnLine, 10551296, 0.f) ||
			!AssemblyCSharp::IsVisible_2(OriginalClosestPointOnLine, ClosestPointOnLine, 10551296, 0.f) || !AssemblyCSharp::IsVisible_2(CenterPosition, HitPointWorld, 10551296, 0.f))
		{
			return false;
		}

		static uintptr_t PlayerProjectileUpdateClass = 0; if (!PlayerProjectileUpdateClass) PlayerProjectileUpdateClass = (uintptr_t)CIl2Cpp::FindClass(SECURE("ProtoBuf"), SECURE("PlayerProjectileUpdate"));

		instance->previousPosition() = instance->currentPosition();
		instance->currentPosition() = OriginalClosestPointOnLine;
		instance->traveledDistance() += instance->previousPosition().Distance(instance->currentPosition());
		instance->traveledTime() += instance->previousPosition().Distance(instance->currentPosition()) / CurrentVelocity.Length();

		if (ProtoBuf::PlayerProjectileUpdate* playerProjectileUpdate = reinterpret_cast<ProtoBuf::PlayerProjectileUpdate*>(CIl2Cpp::il2cpp_object_new((void*)PlayerProjectileUpdateClass)))
		{
			playerProjectileUpdate->projectileID() = instance->projectileID();
			playerProjectileUpdate->travelTime() = instance->traveledTime();
			playerProjectileUpdate->curVelocity() = instance->currentVelocity();
			playerProjectileUpdate->curPosition() = instance->currentPosition();

			Visuals::m_local_player->SendProjectileUpdate(playerProjectileUpdate);
			RPC_Counter.Increment();

			if (m_options.m_options.ThickBulletLine)
			{
				UnityEngine::DDraw().Arrow(instance->previousPosition(), instance->currentPosition(), 0.1f, Color(0.f, 1.f, 0.f, 1.f), 30.f);
			}
		}

		instance->previousPosition() = instance->currentPosition();
		instance->currentPosition() = ClosestPointOnLine;
		instance->traveledDistance() += instance->previousPosition().Distance(instance->currentPosition());
		instance->traveledTime() += instance->previousPosition().Distance(instance->currentPosition()) / CurrentVelocity.Length();

		if (ProtoBuf::PlayerProjectileUpdate* playerProjectileUpdate = reinterpret_cast<ProtoBuf::PlayerProjectileUpdate*>(CIl2Cpp::il2cpp_object_new((void*)PlayerProjectileUpdateClass)))
		{
			playerProjectileUpdate->projectileID() = instance->projectileID();
			playerProjectileUpdate->travelTime() = instance->traveledTime();
			playerProjectileUpdate->curVelocity() = instance->currentVelocity();
			playerProjectileUpdate->curPosition() = instance->currentPosition();

			Visuals::m_local_player->SendProjectileUpdate(playerProjectileUpdate);
			RPC_Counter.Increment();

			if (m_options.m_options.ThickBulletLine)
			{
				UnityEngine::DDraw().Arrow(instance->previousPosition(), instance->currentPosition(), 0.1f, Color(1.f, 0.f, 0.f, 1.f), 30.f);
			}
		}



		AssemblyCSharp::HitTest* hTest = instance->hitTest();
		if (!hTest)
		{
			auto g_hit_test_class = CIl2Cpp::FindClass(SECURE(""), SECURE("HitTest"));
			hTest = (AssemblyCSharp::HitTest*)CIl2Cpp::il2cpp_object_new((void*)g_hit_test_class);
		}

		if (HitPointWorld == instance->currentPosition())
		{
			HitPointWorld.y += 0.01f;
		}

		UnityEngine::Ray ray = UnityEngine::Ray(instance->currentPosition(), (HitPointWorld - instance->currentPosition()).Normalized());

		hTest->MaxDistance() = 1000.0f;
		hTest->AttackRay() = ray;
		hTest->DidHit() = true;
		hTest->HitTransform() = m_target.m_player->get_bone_transform(48);
		hTest->HitEntity() = m_target.m_player;
		hTest->HitPoint() = hTest->HitTransform()->InverseTransformPoint(HitPointWorld);
		hTest->HitNormal() = hTest->HitTransform()->InverseTransformDirection(HitPointWorld);
		hTest->damageProperties() = instance->damageProperties();
		hTest->HitMaterial() = CIl2Cpp::il2cpp_string_new(SECURE("Flesh"));


		if (m_options.m_options.ThickBulletLine)
		{
			UnityEngine::DDraw().Arrow(instance->currentPosition(), HitPointWorld, 0.1f, Color(0.f, 0.f, 1.f, 1.f), 30.f);
			UnityEngine::DDraw().Arrow(HitPointWorld, m_target.m_player->ClosestPoint(HitPointWorld), 0.1f, Color(1.f, 1.f, 0.f, 1.f), 30.f);
		}
		instance->DoHit(hTest, hTest->HitPoint(), hTest->HitNormal());
		return true;
	}

	inline bool DoFatBulletHit(Projectile_c* pr, Vector3 point) {
		float maxdist = GetHitDist();

		auto target = AssemblyCSharp::BasePlayer::GetTarget(point, maxdist);

		if (pr->get_isAlive() && target.m_player && !target.m_team && target.m_player->mounted() || target.m_heli) {
			if (!AssemblyCSharp::IsVisible(target.m_position, point)) {
				return false;
			}

			auto ht = hitTest();
			safe_write(ht + 0x66, true, bool); //DidHit
			safe_write(ht + 0x88, (DWORD64)target.m_player, DWORD64); //HitEntity
			UnityEngine::Transform* Transform;

			if (!target.m_heli) {
				Transform = target.m_player->get_bone_transform(47);
			}
			else
			{
				Transform = target.m_player->get_bone_transform(19);
			}

			if (!Transform)
				return false;

			safe_write(ht + 0xB0, (uintptr_t)Transform, DWORD64);

			Vector3 hitpoint = Transform->InverseTransformPoint(point);
			safe_write(ht + 0x90, hitpoint, Vector3); //hitPoint
			safe_write(ht + 0x14, point, Vector3);


			AssemblyCSharp::HitTest* test = (AssemblyCSharp::HitTest*)ht;
			bool result = pr->DoHit(test, point, Vector3());
			//Sphere(point, 0.015f, col(1, 0, 0, 1), 20, true);
			return true;
		}
		return false;
	}

	inline void UpdateHitTest(TraceInfo info) {
		auto ht = hitTest();

		safe_write(ht + 0x66, true, bool); //DidHit
		safe_write(ht + 0x88, info.entity, DWORD64); //HitEntity
		safe_write(ht + 0xA8, info.distance, float); //HitDistance

		if (info.material != 0) {
			System::String* material = (AssemblyCSharp::AssetNameCache::GetName(info.material));
			safe_write(ht + 0xC0, material, System::String*); //HitMaterial  
		}

		safe_write(ht + 0xB8, info.partID, unsigned int); //HitPart
		safe_write(ht + 0xB0, info.bone, DWORD64); //HitTransform
		safe_write(ht + 0x90, info.point, Vector3); //HitPoint
		safe_write(ht + 0x9C, info.normal, Vector3); //HitNormal
		safe_write(ht + 0x78, info.collider, DWORD64); //collider


		auto go = memory::IsAddressValid(info.collider) ? reinterpret_cast<UnityEngine::Component*>(info.collider)->get_gameObject() : reinterpret_cast<UnityEngine::Component*>(info.bone)->get_gameObject();

		safe_write(ht + 0x70, go, UnityEngine::GameObject*); //gameObject
		if (info.bone != 0) {

			auto htt = safe_read(ht + 0xB0, UnityEngine::Transform*);
			Vector3 hitpoint = htt->InverseTransformPoint(info.point);
			safe_write(ht + 0x90, hitpoint, Vector3); //hitPoint

			Vector3 normalpoint = htt->InverseTransformDirection(info.normal);
			safe_write(ht + 0x9C, normalpoint, Vector3); //HitNormal
		}

	}
	inline bool DoHit2(Projectile_c* pr, DWORD64 ht, Vector3 point, Vector3 normal, TraceInfo info, bool& exit) {
		bool result = false;
		if (!IsAlive())
			return result;

		auto material = info.material != 0 ? AssemblyCSharp::AssetNameCache::GetName(info.material)->c_str() : (SECURE(L"generic"));

		bool canIgnore = AssemblyCSharp::IsVisible(sentPosition(), currentPosition() + currentVelocity().Normalized() * 0.01f);
		if (!canIgnore) {
			integrity() = (0);
			return true;
		}

		float org;
		if (canIgnore) {
			Vector3 attackStart = Simulatezzzzz(false, true);

			safe_write(ht + 0x14, UnityEngine::Ray(attackStart, Vector3()), UnityEngine::Ray);
		}

		if (canIgnore && win32::wcscmp(material, SECURE(L"Flesh"))) {
			DWORD64 Tra = safe_read(ht + 0xB0, DWORD64);
			if (Tra) {
				auto st = SECURE("head");
				reinterpret_cast<UnityEngine::Transform*>(Tra)->set_name(st);
			}

			result = pr->DoHit((AssemblyCSharp::HitTest*)ht, point, normal);
			sentPosition() = (currentPosition());

		}
		return result;
	}
	inline bool DoMovement(float deltaTime, Projectile_c* pr) {
		Vector3 a = currentVelocity() * deltaTime;
		float magnitude = a.Length();
		float num2 = 1 / magnitude;
		Vector3 vec2 = a * num2;
		bool flag = false;

		Vector3 vec3 = currentPosition() + vec2 * magnitude;
		float num3 = traveledTime() + deltaTime;


		

		auto ht = hitTest();
		if (!ht) {
			/*HitTest_TypeInfo*/

			auto g_hit_test_class = CIl2Cpp::FindClass(SECURE(""), SECURE("HitTest"));
			auto g_hit_test = CIl2Cpp::il2cpp_object_new((void*)g_hit_test_class);
			ht = (AssemblyCSharp::HitTest*)g_hit_test;
			hitTest() = (AssemblyCSharp::HitTest*)g_hit_test;
		}
		UnityEngine::Ray ray = UnityEngine::Ray(currentPosition(), vec2);
		safe_write(ht + 0x14, ray, UnityEngine::Ray); //AttackRay
		safe_write(ht + 0x34, magnitude, float); //MaxDistance

		AssemblyCSharp::BasePlayer* ow = this->owner();
		safe_write(ht + 0x80, (DWORD64)ow, DWORD64); //IgnoreEntity
		safe_write(ht + 0x2C, 0, float); //Radius
		safe_write(ht + 0x30, 0.2f, float); //Forgiveness                                        FAT BULLET
		if (!Visuals::m_local_player || ow->userID() == Visuals::m_local_player->userID()) {
			safe_write(ht + 0x10, 0x2, int); //Type
		}
		else safe_write(ht + 0x10, 0x1, int); //Type	`

		if (sourceWeaponPrefab()) {
			safe_write(ht + 0x65, true, bool); //BestHit
			safe_write(ht + 0x68, damageProperties(), AssemblyCSharp::DamageProperties*); //DamageProperties
		}
	OFFSET:
		typedef DWORD64(__stdcall* Unknown)(DWORD64);
		DWORD64 st = safe_read(m_game_assembly + GetListAddress, DWORD64); //Method$Facepunch.Pool.GetList\u003CTraceInfo\u003E() address
		if (!st)
		{
			goto OFFSET;
		}

		Unknown get_list = (Unknown)(m_game_assembly + GetListMethodAddress);//Method$Facepunch.Pool.GetList\u003CTraceInfo\u003E() MethodAddress
		if (!get_list)
		{
			goto OFFSET;
		}

		DWORD64 rs = get_list(st);
		if (!rs)
		{
			goto OFFSET;
		}


		int layerMask = 1269916433;

		layerMask &= ~256; //Deployed
		layerMask &= ~1; //Default
		layerMask &= ~1073741824; //tree
		layerMask &= ~16; //Water
		layerMask &= ~134217728; //Vehicle_Large
		layerMask &= ~32768; //Vehicle_World
		layerMask &= ~8192; //Vehicle_Detailed
		layerMask &= ~1073741824; //tree


		AssemblyCSharp::GameTrace::TraceAll(memory::ToAddress(ht), rs, layerMask /*mask*/);

		ht = hitTest();
		safe_write(ht + 0x34, 0, float); //AttackEnd == AttackStart


		int size = safe_read(rs + 0x18, int);

		DWORD64 lst = safe_read(rs + 0x10, DWORD64);

		auto camera = UnityEngine::Camera::get_main();
		if (!camera)
			return false;


		auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
		if (!m_target.m_player)
			return false;



		if (m_target.m_player->mounted() || m_target.m_heli)
		{
			if (DoFatBulletHit(pr, currentPosition()))
			{
				RPC_Counter.Reset();
				integrity() = (0);
				flag = true;
				return false;
			}
		}
		else
		{
			for (int i = 0; i < 30; i++)
			{
				auto NextCurrentPosition = currentPosition() + (currentVelocity() * (deltaTime / 30) * i);

				if (BulletTP(this, NextCurrentPosition, currentPosition(), currentVelocity(), deltaTime))
				{
					RPC_Counter.Reset();
					integrity() = (0);
					flag = true;
					return false;
				}
			}

		}



		for (int i = 0; i < size && this->IsAlive() && !flag; i++) {
			TraceInfo Trace = safe_read(lst + 0x20 + (i * sizeof(TraceInfo)), TraceInfo);
			if (Trace.valid) {
				this->UpdateHitTest(Trace);

				Vector3 vec4 = reinterpret_cast<AssemblyCSharp::HitTest*>(ht)->HitPointWorld(); //Vector3 HitPointWorld();
				Vector3 normal = reinterpret_cast<AssemblyCSharp::HitTest*>(ht)->HitNormalWorld(); //Vector3 HitNormalWorld();

				//Line(currentPosition(), vec4, col(1, 1, 1, 1), 20, true, true);
				//Sphere(Trace.point, 0.05f, col(0.5, 0, 0, 1), 20, true);

				float magnitude2 = (vec4 - currentPosition()).Length();
				float num5 = magnitude2 * num2 * deltaTime;

				traveledDistance() = (traveledDistance() + magnitude2);
				traveledTime() = (traveledTime() + num5);
				currentPosition() = (vec4);

				bool exit = false;
				if (this->DoHit2(pr, memory::ToAddress(ht), vec4, normal, Trace, exit)) {
					flag = true;
				}

				if (exit) {
					return true;
				}
			}
		}

		if (!flag && this->IsAlive()) {
			float magnitude3 = (vec3 - currentPosition()).Length();
			float num6 = magnitude3 * num2 * deltaTime;
			traveledDistance() = (traveledDistance() + magnitude3);
			traveledTime() = (traveledTime() + num6);
			currentPosition() = (vec3);
		}

		return false;
	}
	inline void DoVelocityUpdate(float deltaTime, Projectile* pr) {
		if (!IsAlive())
			return;

		Vector3 gr = UnityEngine::Physics::get_gravity(); //static Vector3 get_gravity();

		Vector3 tr = (gr * gravityModifier() * deltaTime);
		currentVelocity() = (currentVelocity() + tr);

		Vector3 dr = (currentVelocity() * drag() * deltaTime);
		currentVelocity() = (currentVelocity() - dr);
	}

	void CreateHT() {
		auto _this = (Projectile*)this;
		//if (!memory::IsAddressValid(_this))
		if (!_this)
			return;

		auto ht = hitTest();
		if (!ht) {
			/*HitTest_TypeInfo*/

			auto g_hit_test_class = CIl2Cpp::FindClass(SECURE(""), SECURE("HitTest"));
			auto g_hit_test = CIl2Cpp::il2cpp_object_new((void*)g_hit_test_class);
			ht = (AssemblyCSharp::HitTest*)g_hit_test;
			hitTest() = (AssemblyCSharp::HitTest*)g_hit_test;
		}
	}

	inline void UpdateVelocity(float deltaTime, Projectile_c* pr, bool& rett) {
		if (traveledTime() != 0) {
			previousPosition() = (this->currentPosition());
			previousTraveledTime() = (this->traveledTime());
		}

		UnityEngine::Transform* Transform = pr->get_transform();
		Vector3 pos = Transform->get_position();
		this->currentPosition() = (pos);

		if (traveledTime() == 0) {


			this->sentPosition() = (pos);
			this->previousPosition() = (pos);
			tumbleSpeed() = (0); closeFlybyDistance() = (0);  tumbleAxis() = (initialVelocity()); sentPosition() = (sentPosition()); createDecals() = (false);



			Vector3 InitialVel = initialVelocity();
			*reinterpret_cast<float*>((uintptr_t)this + 0x90) = InitialVel.Length();
			this->previousTraveledTime() = 0.f;
			this->createDecals() = (false);
			*reinterpret_cast<bool*>((uintptr_t)this + 0xf8) = false;
			CreateHT();

		}

		deltaTime *= UnityEngine::Time::get_timeScale();


		bool ret = this->DoMovement(deltaTime, pr);
		if (!ret) {
			this->DoVelocityUpdate(deltaTime, pr);
		}

		auto Trans = pr->get_transform(); //Component | Transform get_transform(); 
		Trans->set_position(currentPosition()); //Transform | void set_position(Vector3 value); 

		Vector4 rotation = Vector4::QuaternionLookRotation(currentVelocity(), Vector3(0, 1, 0));

		Trans->set_rotation(rotation);
	}
};


void LaunchProjectile(AssemblyCSharp::Projectile* _This)
{
	if (!_This->isAuthoritative())
		return;

	auto _this2 = (Projectile_c*)_This;
	bool rett = false;

	while (_this2->IsAlive() && (_this2->traveledDistance() < _this2->initialDistance() || _this2->traveledTime() < 0.1f))
		_this2->UpdateVelocity(0.03125f, _this2, rett);
}

auto Hooks::PPS_WriteToStream(ProtoBuf::ProjectileShoot* _This, ProtoBuf::Stream* Stream) -> void {

	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	if(!memory::IsAddressValid(_This))
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);


	m_options.m_options.fixed_time_last_shot = UnityEngine::Time::get_fixedTime();
	m_options.m_options.just_shot = true;
	m_options.m_options.did_reload = false;

	if(!m_aimbot.m_aimbot.SilentAim)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	if(m_options.m_options.pSilentType != 0)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	auto LocalPlayer = Visuals::m_local_player;
	if (!memory::IsAddressValid(LocalPlayer))
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	if(!LocalPlayer ->HasFlag(256))
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	auto camera = LocalPlayer->eyes();
	if(!memory::IsAddressValid(camera))
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_position(), 500.f);
	if(!m_target.m_player)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	auto HeldItem = LocalPlayer->GetHeldItemSafe();
	if(!memory::IsAddressValid(HeldItem))
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	if(!LocalPlayer->IsWeapon())
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	auto BaseProjectile = Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>();
	if(!memory::IsAddressValid(BaseProjectile))
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	if (BaseProjectile->IsA(AssemblyCSharp::BaseMelee::StaticClass()))
	{
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);
	}

	auto PrimaryMagazine = BaseProjectile->primaryMagazine();
	if(!memory::IsAddressValid(PrimaryMagazine))
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);
	
	auto AmmoType = PrimaryMagazine->ammoType();
	if(!memory::IsAddressValid(AmmoType))
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	auto created_projectiles = BaseProjectile->createdProjectiles();
	if (!created_projectiles)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	auto created_projectiles_ = created_projectiles->_items;
	if (!created_projectiles_)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);
	
	auto created_projectiles_size = created_projectiles->_size;
	if (!created_projectiles_size)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	auto projectile_shoot_projectiles = _This->projectiles();
	if (!projectile_shoot_projectiles)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);
	
	auto projectile_shoot_items = projectile_shoot_projectiles->_items;
	if (!projectile_shoot_items)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	auto projectile_shoot_size = projectile_shoot_projectiles->_size;
	if (!projectile_shoot_size)
		return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);
	
	//auto isTeammate = LocalPlayer->IsSameTeam(m_target.m_player);
	//if (isTeammate && !m_aimbot.m_aimbot.TargetTeam)
	//	return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);

	Vector3 StartPosition;
	Vector3 OriginalVelocity;
	Vector3 m_aim_angle;
	float travel_time = 0.f;
	bool visible = false;
	bool manipulated = false;
	bool vis_fat = false;

	if (m_aimbot.m_aimbot.LowVelocity)
	{
		auto LocalPlayer = AssemblyCSharp::LocalPlayer::get_Entity();/* LocalPlayer->GetHeldItemSafe()->heldEntity()->class_name*/
		auto TargetPlayer = m_target.m_player;

		auto LocalPlayerPos = LocalPlayer->get_bone_transform(48)->get_position();
		auto TargetPlayerPos = TargetPlayer->get_bone_transform(48)->get_position();

		auto LocalPlayerY = LocalPlayer->get_transform()->get_position().y;
		auto TargetPlayerY = TargetPlayer->get_transform()->get_position().y;

		auto TotalHeight = TargetPlayerY - LocalPlayerY;
		//printfz(SECURE("\nTotalHeight: %f"), TotalHeight);

		if (memory::IsAddressValid(LocalPlayer))
		{
			auto HeldItem = LocalPlayer->ActiveItem();
			if (memory::IsAddressValid(HeldItem))
			{
				auto HeldItemID = LocalPlayer->GetHeldItemSafe()->info()->itemid();


				if (BaseProjectile->IsA(AssemblyCSharp::BowWeapon::StaticClass()) || HeldItemID == 1953903201) //Nailgun
				{

					if (m_target.m_distance <= 95)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.41;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.46;
						else
							BaseProjectile->projectileVelocityScale() = 0.36;
					}
					else if (m_target.m_distance > 95 && m_target.m_distance <= 120)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.46;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.51;
						else
							BaseProjectile->projectileVelocityScale() = 0.41;
					}
					else if (m_target.m_distance > 120 && m_target.m_distance <= 140)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.51;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.56;
						else
							BaseProjectile->projectileVelocityScale() = 0.46; //0.455
					}
					else if (m_target.m_distance > 140 && m_target.m_distance <= 180)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.56;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.61;
						else
							BaseProjectile->projectileVelocityScale() = 0.51;
					}
					else if (m_target.m_distance > 180 && m_target.m_distance <= 220)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.61;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.65;
						else
							BaseProjectile->projectileVelocityScale() = 0.56;
					}
					else if (m_target.m_distance > 220 && m_target.m_distance <= 275)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.65;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.71;
						else
							BaseProjectile->projectileVelocityScale() = 0.61;
					}
					else if (m_target.m_distance > 275 && m_target.m_distance <= 300)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.71;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.76;
						else
							BaseProjectile->projectileVelocityScale() = 0.66;
					}
					else if (m_target.m_distance > 300 && m_target.m_distance <= 335)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.76;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.81;
						else
							BaseProjectile->projectileVelocityScale() = 0.71;
					}
					else if (m_target.m_distance > 335 && m_target.m_distance <= 360)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.81;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.86;
						else
							BaseProjectile->projectileVelocityScale() = 0.76;
					}
					else if (m_target.m_distance > 360 && m_target.m_distance <= 380)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.86;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.91;
						else
							BaseProjectile->projectileVelocityScale() = 0.81;
					}
					else if (m_target.m_distance > 380 && m_target.m_distance <= 400)
					{
						if (TotalHeight > 25 && TotalHeight <= 50)
							BaseProjectile->projectileVelocityScale() = 0.91;
						else if (TotalHeight > 50)
							BaseProjectile->projectileVelocityScale() = 0.96;
						else
							BaseProjectile->projectileVelocityScale() = 0.86;
					}
				}
				else if (HeldItemID == 1318558775 || HeldItemID == 1796682209) //Mp5a4 || CustomSmg
				{
					float Testing = 0.f;
					if (!AssemblyCSharp::IsVisible(LocalPlayerPos, TargetPlayerPos) && TotalHeight > 10)
						Testing += 0.2f;
					else
						Testing = 0.f;

					if (m_target.m_distance <= 160)
						BaseProjectile->projectileVelocityScale() = 0.27 + Testing;
					else if (m_target.m_distance > 160 && m_target.m_distance <= 170)
						BaseProjectile->projectileVelocityScale() = 0.28 + Testing;
					else if (m_target.m_distance > 170 && m_target.m_distance <= 174)
						BaseProjectile->projectileVelocityScale() = 0.29 + Testing;
					else if (m_target.m_distance > 174 && m_target.m_distance <= 180)
						BaseProjectile->projectileVelocityScale() = 0.30 + Testing;
					else if (m_target.m_distance > 180 && m_target.m_distance <= 190)
						BaseProjectile->projectileVelocityScale() = 0.31 + Testing;
					else if (m_target.m_distance > 190 && m_target.m_distance <= 194)
						BaseProjectile->projectileVelocityScale() = 0.32 + Testing;
					else if (m_target.m_distance > 194 && m_target.m_distance <= 200)
						BaseProjectile->projectileVelocityScale() = 0.33 + Testing;
					else if (m_target.m_distance > 200 && m_target.m_distance <= 205)
						BaseProjectile->projectileVelocityScale() = 0.34 + Testing;
					else if (m_target.m_distance > 205 && m_target.m_distance <= 210)
						BaseProjectile->projectileVelocityScale() = 0.35 + Testing;
					else if (m_target.m_distance > 212 && m_target.m_distance <= 220)
						BaseProjectile->projectileVelocityScale() = 0.36 + Testing;
					else if (m_target.m_distance > 220 && m_target.m_distance <= 225)
						BaseProjectile->projectileVelocityScale() = 0.37 + Testing;
					else if (m_target.m_distance > 225 && m_target.m_distance <= 230)
						BaseProjectile->projectileVelocityScale() = 0.38 + Testing;
					else if (m_target.m_distance > 230 && m_target.m_distance <= 240)
						BaseProjectile->projectileVelocityScale() = 0.39 + Testing;
					else if (m_target.m_distance > 240 && m_target.m_distance <= 245)
						BaseProjectile->projectileVelocityScale() = 0.40 + Testing;
					else if (m_target.m_distance > 245 && m_target.m_distance <= 250)
						BaseProjectile->projectileVelocityScale() = 0.41 + Testing;
					else if (m_target.m_distance > 250 && m_target.m_distance <= 258)
						BaseProjectile->projectileVelocityScale() = 0.42 + Testing;
					else if (m_target.m_distance > 258 && m_target.m_distance <= 265)
						BaseProjectile->projectileVelocityScale() = 0.43 + Testing;
					else if (m_target.m_distance > 265 && m_target.m_distance <= 270)
						BaseProjectile->projectileVelocityScale() = 0.44 + Testing;
					else if (m_target.m_distance > 270 && m_target.m_distance <= 275)
						BaseProjectile->projectileVelocityScale() = 0.45 + Testing;
					else if (m_target.m_distance > 275 && m_target.m_distance <= 285)
						BaseProjectile->projectileVelocityScale() = 0.46 + Testing;
					else if (m_target.m_distance > 285 && m_target.m_distance <= 290)
						BaseProjectile->projectileVelocityScale() = 0.47 + Testing;
					else if (m_target.m_distance > 290 && m_target.m_distance <= 295)
						BaseProjectile->projectileVelocityScale() = 0.48 + Testing;
					else if (m_target.m_distance > 295 && m_target.m_distance <= 300)
						BaseProjectile->projectileVelocityScale() = 0.49 + Testing;
					else if (m_target.m_distance > 300)
						BaseProjectile->projectileVelocityScale() = 0.80f;

				}
				else if (HeldItemID == -1758372725 || HeldItemID == -852563019 || HeldItemID == 649912614 || HeldItemID == 1914691295 || HeldItemID == 1373971859 || HeldItemID == 818877484) /* Thompson, M92 Pistol, Revolver, Prototype 17, Python Revolver, Semi-auto Pistol */
				{

					float Testing = 0.f;
					if (!AssemblyCSharp::IsVisible(LocalPlayerPos, TargetPlayerPos) && TotalHeight > 10)
						Testing += 0.2f;
					else
						Testing = 0.f;

					if (m_target.m_distance <= 205)
						BaseProjectile->projectileVelocityScale() = 0.34 + Testing;
					else if (m_target.m_distance > 205 && m_target.m_distance <= 215)
						BaseProjectile->projectileVelocityScale() = 0.35 + Testing;
					else if (m_target.m_distance > 215 && m_target.m_distance <= 223)
						BaseProjectile->projectileVelocityScale() = 0.36 + Testing;
					else if (m_target.m_distance > 223 && m_target.m_distance <= 228)
						BaseProjectile->projectileVelocityScale() = 0.37 + Testing;
					else if (m_target.m_distance > 228 && m_target.m_distance <= 233)
						BaseProjectile->projectileVelocityScale() = 0.38 + Testing;
					else if (m_target.m_distance > 233 && m_target.m_distance <= 241)
						BaseProjectile->projectileVelocityScale() = 0.39 + Testing;
					else if (m_target.m_distance > 241 && m_target.m_distance <= 247)
						BaseProjectile->projectileVelocityScale() = 0.40 + Testing;
					else if (m_target.m_distance > 247 && m_target.m_distance <= 252)
						BaseProjectile->projectileVelocityScale() = 0.41 + Testing;
					else if (m_target.m_distance > 252 && m_target.m_distance <= 257)
						BaseProjectile->projectileVelocityScale() = 0.42 + Testing;
					else if (m_target.m_distance > 257 && m_target.m_distance <= 264)
						BaseProjectile->projectileVelocityScale() = 0.43 + Testing;
					else if (m_target.m_distance > 264 && m_target.m_distance <= 273)
						BaseProjectile->projectileVelocityScale() = 0.44 + Testing;
					else if (m_target.m_distance > 273 && m_target.m_distance <= 275)
						BaseProjectile->projectileVelocityScale() = 0.45 + Testing;
					else if (m_target.m_distance > 275 && m_target.m_distance <= 283)
						BaseProjectile->projectileVelocityScale() = 0.46 + Testing;
					else if (m_target.m_distance > 283 && m_target.m_distance <= 290)
						BaseProjectile->projectileVelocityScale() = 0.47 + Testing;
					else if (m_target.m_distance > 290 && m_target.m_distance <= 294)
						BaseProjectile->projectileVelocityScale() = 0.48 + Testing;
					else if (m_target.m_distance > 294 && m_target.m_distance <= 300)
						BaseProjectile->projectileVelocityScale() = 0.49 + Testing;
					else if (m_target.m_distance > 300)
						BaseProjectile->projectileVelocityScale() = 1.f;

				}
				else
				{
					if (HeldItemID == 1318558775 || HeldItemID == 1796682209) //Mp5a4 || CustomSmg									
						BaseProjectile->projectileVelocityScale() = 0.60f;
					else if (HeldItemID == 28201841) //M39 Rifle
						BaseProjectile->projectileVelocityScale() = 1.0f;
					else if (HeldItemID == -2069578888) //M249
						BaseProjectile->projectileVelocityScale() = 1.0f;
					else if (HeldItemID == 1588298435) //Bolt Action Rifle
						BaseProjectile->projectileVelocityScale() = 1.75f;
					else if (HeldItemID == -1214542497) //HMLMG
						BaseProjectile->projectileVelocityScale() = 0.5f;
					else if (HeldItemID == -778367295) //L96 Rifle
						BaseProjectile->projectileVelocityScale() = 1.f;
					else
						BaseProjectile->projectileVelocityScale() = 0.5f;
				}
			}
		}
	}
	else/* if (memory::IsAddressValid(LocalPlayer->ActiveItem()))*/
	{
		auto HeldItemID = LocalPlayer->GetHeldItemSafe()->info()->itemid();

		if (HeldItemID == 1318558775 || HeldItemID == 1796682209) //Mp5a4 || CustomSmg									
			BaseProjectile->projectileVelocityScale() = 0.60f;
		else if (HeldItemID == 28201841) //M39 Rifle
			BaseProjectile->projectileVelocityScale() = 1.0f;
		else if (HeldItemID == -2069578888) //M249
			BaseProjectile->projectileVelocityScale() = 1.0f;
		else if (HeldItemID == 1588298435) //Bolt Action Rifle
			BaseProjectile->projectileVelocityScale() = 1.75f;
		else if (HeldItemID == -1214542497) //HMLMG
			BaseProjectile->projectileVelocityScale() = 0.5f;
		else if (HeldItemID == -778367295) //L96 Rifle
			BaseProjectile->projectileVelocityScale() = 1.f;
		else
			BaseProjectile->projectileVelocityScale() = 0.5f;
	}
	//printfz(SECURE("\nLocalPlayerY: %f"), LocalPlayerY);
	//printfz(SECURE("\nTargetPlayerY: %f"), TargetPlayerY);
	//printfz("\nHelditem ID: %u", LocalPlayer->GetHeldItemSafe()->info()->itemid());
	//printfz("\nProjectileVelocityScale: %f", BaseProjectile->projectileVelocityScale());
	//printfz("\nTarget Distance: %f", m_target.m_distance);

	auto HitScan = [&](Vector3 from) {
		Vector3 head_pos_ = m_target.m_player->get_bone_transform(RustStructs::bones::head)->get_position();

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

	auto BestPos = HitScan(LocalPlayer->eyes()->get_position() + spoofed_eye_pos);

	for (std::int32_t index = 0; index < projectile_shoot_size; index++)
	{
		auto c_projectile = projectile_shoot_items->m_Items[index];
		if (!c_projectile) continue;

		StartPosition = c_projectile->startPos();
		if (StartPosition.IsZero()) continue;

		OriginalVelocity = c_projectile->startVel();
		if (OriginalVelocity.IsZero()) continue;

		if (m_options.m_options.AutoShootType == 0)
		{
			if (m_aimbot.m_aimbot.Manipulation && UnityEngine::Input::GetKey(m_aimbot.m_aimbot.ManipKey)) {
				auto rpcposv2 = StartPosition;
				if (spoofed_eye_pos != Vector3(0, 0, 0))
				{
					StartPosition += (spoofed_eye_pos);

					if (m_options.m_options.HitScan)
					{
						visible = AssemblyCSharp::IsVisible(StartPosition, BestPos, 0.2f);
					}
					else {
						visible = AssemblyCSharp::IsVisible(StartPosition, targetpos, 0.2f);
					}

					//visible = AssemblyCSharp::IsVisible(StartPosition, m_target.m_position);

					if (StartPosition != rpcposv2)
					{
						manipulated = true;

						if (visible)
						{
							visible = true;

							c_projectile->startPos() = StartPosition;
						}
					}
				}
			}
		}
		else if (m_options.m_options.AutoShootType == 1)
		{


			auto playerInput = LocalPlayer->input();

			if (playerInput)
			{
				auto inputState = playerInput->state();

				if (playerInput)
				{
					//auto state = playerInput->state();
					//if (state)
					{

						if (!inputState->IsDown(RustStructs::FORWARD)
							&& !inputState->IsDown(RustStructs::BACKWARD)
							&& !inputState->IsDown(RustStructs::LEFT)
							&& !inputState->IsDown(RustStructs::RIGHT))
						{
							if (m_aimbot.m_aimbot.Manipulation) {
								auto rpcposv2 = StartPosition;
								if (spoofed_eye_pos != Vector3(0, 0, 0))
								{
									StartPosition += (spoofed_eye_pos);

									if (m_options.m_options.HitScan)
									{
										visible = AssemblyCSharp::IsVisible(StartPosition, BestPos, 0.2f);
									}
									else {
										visible = AssemblyCSharp::IsVisible(StartPosition, targetpos, 0.2f);
									}

									//visible = AssemblyCSharp::IsVisible(StartPosition, m_target.m_position);

									if (StartPosition != rpcposv2)
									{
										manipulated = true;

										if (visible)
										{
											visible = true;

											c_projectile->startPos() = StartPosition;
										}
									}
								}
							}
						}
					}
				}
			}
		}	
	}

	for (std::int32_t index = 0; index < created_projectiles_size; index++)
	{
		auto c_projectile = *(AssemblyCSharp::Projectile**)((uintptr_t)created_projectiles_ + 0x20 + index * 0x8);
		if (!c_projectile) continue;

		AssemblyCSharp::ItemModProjectile* itemModProjectile = AmmoType->GetComponent<AssemblyCSharp::ItemModProjectile>((System::Type*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE(""), SECURE("ItemModProjectile"))));

		Vector3 LocalPos = LocalPlayer->get_positionz();

		auto CompoundBowWeapon = LocalPlayer->GetHeldEntityCast<AssemblyCSharp::CompoundBowWeapon>();

		float BulletSpeed = 0.f;

		if (m_options.m_options.HitScan)
		{
			Vector3 a;


			if (m_aimbot.m_aimbot.NewPrediction)
			{
				Vector3 aim_angle = GetAimDirectionToTarget(LocalPlayer, BaseProjectile, BestPos, m_target.m_velocity, itemModProjectile, StartPosition) - StartPosition;
				m_aim_angle = (aim_angle).Normalized() * OriginalVelocity.Length();
			}
			else
				SimulateProjectile(m_target.m_player, StartPosition, BestPos, m_aim_angle, a, travel_time, c_projectile, BaseProjectile, itemModProjectile, m_target.m_velocity);
		}
		else {
			
			if (m_options.m_options.ThickBullet)
			{
				Vector3 a;
				//SimulateProjectile(m_target.m_player, StartPosition, m_target.m_position, m_aim_angle, a, travel_time, c_projectile, BaseProjectile, itemModProjectile, m_target.m_velocity);
				if (m_aimbot.m_aimbot.NewPrediction)
				{
					Vector3 aim_angle = GetAimDirectionToTarget(LocalPlayer, BaseProjectile, Vector3(targetpos.x, targetpos.y, targetpos.z), m_target.m_velocity, itemModProjectile, StartPosition) - StartPosition;
					m_aim_angle = (aim_angle).Normalized() * OriginalVelocity.Length();
				}
				else
					SimulateProjectile(m_target.m_player, StartPosition, Vector3(targetpos.x, targetpos.y, targetpos.z), m_aim_angle, a, travel_time, c_projectile, BaseProjectile, itemModProjectile, m_target.m_velocity);
			}
			else
			{
				Vector3 a;
				//SimulateProjectile(m_target.m_player, StartPosition, m_target.m_position, m_aim_angle, a, travel_time, c_projectile, BaseProjectile, itemModProjectile, m_target.m_velocity);
				if (m_aimbot.m_aimbot.NewPrediction)
				{
					Vector3 aim_angle = GetAimDirectionToTarget(LocalPlayer, BaseProjectile, m_target.m_position, m_target.m_velocity, itemModProjectile, StartPosition) - StartPosition;
					m_aim_angle = (aim_angle).Normalized() * OriginalVelocity.Length();
				}
				else
					SimulateProjectile(m_target.m_player, StartPosition, m_target.m_position, m_aim_angle, a, travel_time, c_projectile, BaseProjectile, itemModProjectile, m_target.m_velocity);
			}
		}
	}

	int aimbot_percentage = (my_rand() % (MAXIMUM_AIMBOT_ACCURACY - MINIMUM_AIMBOT_ACCURACY + 1)) + MINIMUM_AIMBOT_ACCURACY;
	for (std::int32_t index = 0; index < projectile_shoot_size; index++)
	{
		auto c_projectile = projectile_shoot_items->m_Items[index];
		if (!c_projectile) continue;

		if (!m_options.m_options.ignore_target && m_target.m_player || manipulated && !m_target.m_team) {
			if (aimbot_percentage <= (int)m_aimbot.m_aimbot.AimbotAccuracy)
				c_projectile->startVel() = m_aim_angle;
		}
	}

	for (std::int32_t index = 0; index < created_projectiles_size; index++)
	{
		auto c_projectile = *(Projectile**)((uintptr_t)created_projectiles_ + 0x20 + index * 0x8);
		if (!c_projectile) continue;

		if (!m_options.m_options.ignore_target && m_target.m_player || manipulated && !m_target.m_team) {
			if (aimbot_percentage <= (int)m_aimbot.m_aimbot.AimbotAccuracy)
				c_projectile->currentVelocity() = m_aim_angle;
		}

		auto desynctime = UnityEngine::Time::get_realtimeSinceStartup() - AssemblyCSharp::LocalPlayer::get_Entity()->lastSentTickTime();
		auto desyncpercentage = (((desynctime / 0.85f) * 100.0f) + 1.f) / 100;

		//auto original_dist = c_projectile->initialDistance();
		//if (m_options.m_options.InstantBullet && UnityEngine::Input::GetKey(m_aimbot.m_aimbot.ManipKey) && desyncpercentage >= 0.85)
		//{
		//	auto HeldItemID = LocalPlayer->GetHeldItemSafe()->info()->itemid();
		//	float distance = m_target.m_position.Distance(LocalPlayer->get_transform()->get_position());

		//	Vector3 r = StartPosition;
		//	Vector3 v = m_aim_angle;
		//	Vector3 n = StartPosition + (v * m_options.m_options.max_spoofed_eye_distance);

		//	if (BaseProjectile->IsA(AssemblyCSharp::BowWeapon::StaticClass()) || HeldItemID == 1953903201) //Nailgun
		//	{
		//		if (distance <= 50)
		//			c_projectile->initialDistance() = r.Distance(n);
		//	}
		//	else
		//		c_projectile->initialDistance() = r.Distance(n);
		//}
		//else {
		//	c_projectile->initialDistance() = 0.f;
		//}

		//c_projectile->integrity() = 1.f;
		//c_projectile->initialVelocity() = (m_aim_angle);

		//c_projectile->currentVelocity() = (m_aim_angle);
		//c_projectile->initialVelocity() = (m_aim_angle);
		//c_projectile->previousVelocity() = (m_aim_angle);

		//c_projectile->currentPosition() = (StartPosition);
		//c_projectile->previousPosition() = (StartPosition);
		//c_projectile->sentPosition() = (StartPosition);
		//c_projectile->tumbleAxis() = (StartPosition); //?
	

		//LaunchProjectile(c_projectile);


		if (m_options.m_options.AutoShootType == 0)
		{
			if (m_aimbot.m_aimbot.Manipulation && UnityEngine::Input::GetKey(m_aimbot.m_aimbot.ManipKey))
			{
				//visible = AssemblyCSharp::IsVisible(StartPosition, m_target.m_position);
				if (m_options.m_options.HitScan)
				{
					visible = AssemblyCSharp::IsVisible(StartPosition, BestPos);
				}
				else {
					visible = AssemblyCSharp::IsVisible(StartPosition, targetpos);
				}

				if (visible)
				{
					if (manipulated)
					{
						auto g = c_projectile->get_gameObject();
						if (!memory::IsAddressValid(g))
							continue;


						auto t = g->get_transform();
						if (!memory::IsAddressValid(t))
							continue;

						t->set_position(StartPosition);
					}
				}
			}
		}
		else if (m_options.m_options.AutoShootType == 1)
		{
			auto playerInput = LocalPlayer->input();

			if (playerInput)
			{
				auto inputState = playerInput->state();

				if (playerInput)
				{
					auto state = playerInput->state();
					if (state)
					{

						if (!inputState->IsDown(RustStructs::FORWARD)
							&& !inputState->IsDown(RustStructs::BACKWARD)
							&& !inputState->IsDown(RustStructs::LEFT)
							&& !inputState->IsDown(RustStructs::RIGHT))
						{
							if (m_aimbot.m_aimbot.Manipulation)
							{
								//visible = AssemblyCSharp::IsVisible(StartPosition, m_target.m_position);
								if (m_options.m_options.HitScan)
								{
									visible = AssemblyCSharp::IsVisible(StartPosition, BestPos);
								}
								else {
									visible = AssemblyCSharp::IsVisible(StartPosition, targetpos);
								}

								if (visible)
								{
									if (manipulated)
									{
										auto g = c_projectile->get_gameObject();
										if (!memory::IsAddressValid(g))
											continue;


										auto t = g->get_transform();
										if (!memory::IsAddressValid(t))
											continue;

										t->set_position(StartPosition);
									}
								}
							}
						}
					}
				}
			}
		}
	}

	return Hooks::PPS_WriteToStreamhk.get_original< decltype(&PPS_WriteToStream)>()(_This, Stream);
}