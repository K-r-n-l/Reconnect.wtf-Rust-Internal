#include "ProjectileUpdate.h"

#include "../../Esp/Visuals.h"
#include "../../Aimbot/Aimbot.h"

inline bool LineCircleIntersection(Vector3 center,
	float radius,
	Vector3 rayStart,
	Vector3 rayEnd,
	float& offset)
{
	Vector2 P(rayStart.x, rayStart.z);
	Vector2 Q(rayEnd.x, rayEnd.z);

	float a = Q.y - P.y;
	float b = P.x - Q.x;
	float c = (a * (P.x) + b * (P.y)) * -1.f;

	float x = center.x;
	float y = center.z;

	float c_x = (b * ((b * x) - (a * y)) - a * c) / (win32::pow(a, 2) + win32::pow(b, 2));
	float c_y = (a * ((-b * x) + (a * y)) - (b * c)) / (win32::pow(a, 2) + win32::pow(b, 2));

	Vector2 closestPoint(c_x, c_y);

	float distance = P.Distance(Q);

	if (P.Distance(closestPoint) > distance || Q.Distance(closestPoint) > distance)
	{
		return false;
	}

	if (radius > closestPoint.Distance(Vector2(center.x, center.z)))
	{
		Vector2 P(rayStart.x, rayStart.y);
		Vector2 Q(rayEnd.x, rayEnd.y);

		float a = Q.y - P.y;
		float b = P.x - Q.x;
		float c = (a * (P.x) + b * (P.y)) * -1.f;

		float x = center.x;
		float y = center.y;

		float c_x = (b * ((b * x) - (a * y)) - a * c) / (win32::pow(a, 2) + win32::pow(b, 2));
		float c_y = (a * ((-b * x) + (a * y)) - (b * c)) / (win32::pow(a, 2) + win32::pow(b, 2));

		Vector2 closestPoint(c_x, c_y);
		if (radius > closestPoint.Distance(Vector2(center.x, center.y)))
		{
			return true;
		}
		else
		{
			offset += win32::abs(center.y - closestPoint.y);
			return false;
		}
	}

	return false;
};


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

		bool valid; // 0x10
		float distance; // 0x14
		DWORD64 entity; // 0x18   //BasePlayer
		Vector3 point; // 0x20
		Vector3 normal; // 0x2C
		DWORD64/*Transform*/ bone; // 0x38
		DWORD64 material; // 0x40
		unsigned int partID; // 0x48
		DWORD64 collider; // 0x50
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
			sentPosition() =  (pos);
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
		hTest->HitTransform() = m_target.m_player->get_bone_transform(m_target.m_bone);
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
				Transform = target.m_player->get_bone_transform(target.m_bone);
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

		if (m_options.m_options.pierce)
		{
			layerMask &= ~256; //Deployed
			layerMask &= ~1; //Default
			layerMask &= ~1073741824; //tree
			layerMask &= ~16; //Water
			layerMask &= ~134217728; //Vehicle_Large
			layerMask &= ~32768; //Vehicle_World
			layerMask &= ~8192; //Vehicle_Detailed
			layerMask &= ~1073741824; //tree

		}

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

				traveledDistance() =(traveledDistance() + magnitude2);
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

void Hooks::ProjectileUpdate(AssemblyCSharp::Projectile* _This)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::ProjectileUpdatehk.get_original< decltype(&ProjectileUpdate)>()(_This);

	if(!_This)
		return Hooks::ProjectileUpdatehk.get_original< decltype(&ProjectileUpdate)>()(_This);

	if (Visuals::m_local_player->lifestate() & RustStructs::Dead || !Visuals::m_local_player->IsConnected())
	{
		return Hooks::ProjectileUpdatehk.get_original< decltype(&ProjectileUpdate)>()(_This);
	}

	do {

		/*if (_This->IsAlive())
		{
			_This->Launch();
		}*/

		

		//_This->SetEffectScale(Vector3().InverseLerp(12.f, 20.f, _This->currentVelocity().Magnitude()));
		//_This->DoFlybySound();
		float deltaTime = UnityEngine::Time::get_fixedDeltaTime();

		deltaTime *= UnityEngine::Time::get_timeScale();

		auto camera = UnityEngine::Camera::get_main();
		if (!memory::IsAddressValid(camera))
			break;

		if (m_options.m_options.NoSpread)
		{
			auto itemMod = _This->mod();
			if (itemMod)
			{
				itemMod->projectileSpread() = 0.f;
				itemMod->projectileVelocitySpread() = 0.f;
			}
		}

		if (m_options.m_options.BulletTracers)
		{
			Vector3 a = _This->currentVelocity() * deltaTime;
			float magnitude = a.Length();
			float num2 = 1 / magnitude;
			Vector3 vec2 = a * num2;
			bool flag = false;

			Vector3 vec3 = _This->currentPosition() + vec2 * magnitude;

			UnityEngine::DDraw().Line(_This->currentPosition(), vec3, Color::Red(), 5.f, false, false);
		}

		auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
		if (!memory::IsAddressValid(m_target.m_player))
			break;

		auto HeldItem = Visuals::m_local_player->GetHeldItemSafe();
		if (!memory::IsAddressValid(HeldItem))
			break;

		auto BaseProjectile = Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>();
		if (!memory::IsAddressValid(BaseProjectile))
			break;

		if (_This->owner() != Visuals::m_local_player)
			break;

		if (_This->projectileID() == 0 || !_This->owner()) {
			_This->Retire();
			break;
		}

	/*	if (BaseProjectile->IsA(AssemblyCSharp::BaseMelee::StaticClass()))
		{
			//break;
		}*/

		
		//if (m_options.m_options.ThickBullet)
		//{
		//	_This->thickness() = 1.f;
		//}

		if (m_options.m_options.ThickBullet && m_aimbot.m_aimbot.LowVelocity)
		{
			auto CurrentPosition = _This->currentPosition();
			auto CurrentVelocity = _This->currentVelocity();
			auto _this2 = (Projectile_c*)_This;

			Vector3 a = _This->currentVelocity() * deltaTime;
			float magnitude = a.Magnitude();
			float num2 = 1.f / magnitude;
			Vector3 vector2 = a * num2;
			bool flag = false;
			Vector3 vector3 = _This->currentPosition() + vector2 * magnitude;


			for (int i = 0; i < 30; i++)
			{
				auto NextCurrentPosition = CurrentPosition + (CurrentVelocity * (deltaTime / 30) * i);

				if (_this2->BulletTP(_This, NextCurrentPosition, CurrentPosition, CurrentVelocity, deltaTime))
				{
					RPC_Counter.Reset();
					_This->Retire();
					break;
				}
			}
		}
		else if (m_options.m_options.ThickBullet)
		{

			auto _this2 = (Projectile_c*)_This;
			auto owner = _This->owner();
			if (!owner) return;

			if (owner->IsLocalPlayer())
			{
				bool ret = false;

				if (_This->get_isAlive())
				{
					for (; _this2->IsAlive(); _this2->UpdateVelocity(0.03125f, (Projectile_c*)_This, ret)) {
						if (ret) {
							break;
						}

						if (_this2->launchTime() <= 0) {
							break;
						}

						float time = UnityEngine::Time::get_time();

						if (time - _this2->launchTime() < _this2->traveledTime() + 0.03125f) {
							break;
						}
					}
				}
				else
				{
					_This->Retire();
				}
			}
		}

	} while (0);

	Hooks::ProjectileUpdatehk.get_original< decltype(&ProjectileUpdate)>()(_This);
}