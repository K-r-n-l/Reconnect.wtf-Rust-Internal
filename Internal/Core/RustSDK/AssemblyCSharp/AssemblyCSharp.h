#pragma once
#include "../UnityEngine/UnityEngine.h"
#include "../ProtoBuf/ProtoBuf.h"
#include "../../Settings/Settings.hpp"

namespace AssemblyCSharp {
	static float crt_sqrt2(float number)
	{
		long        i;
		float       x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y; // floating point bit level hacking [sic]
		i = 0x5f3759df - (i >> 1); // Newton's approximation
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y)); // 1st iteration
		y = y * (threehalfs - (x2 * y * y)); // 2nd iteration

		return 1 / y;
	}
	inline float get_2d_dist(const Vector2& Src, const Vector3& Dst) {
		return crt_sqrt2(powFFFFFFFFFFFFFFFFFFFFFF(Src.x - Dst.x) + powFFFFFFFFFFFFFFFFFFFFFF(Src.y - Dst.y));
	}

	inline float get_fov(Vector3 Pos) {

		Vector3 ScreenPos = UnityEngine::Camera::get_main()->WorldToScreen(Pos);
		if (ScreenPos.IsZero())
			return 1000;

		return get_2d_dist(Vector2(screen_center.x, screen_center.y), ScreenPos);
	}

	struct InputMessage : Il2CppObject {
		IL2CPP_CLASS("InputMessage");

		IL2CPP_FIELD(Vector3, aimAngles);

	};

	struct InputState : Il2CppObject
	{
		IL2CPP_CLASS("InputState");

		IL2CPP_FIELD(InputMessage*, current);
		bool IsDown(RustStructs::BUTTON btn);
		bool WasJustPressed(RustStructs::BUTTON btn);
	};

	struct FacepunchBehaviour : UnityEngine::MonoBehaviour {
		IL2CPP_CLASS("FacepunchBehaviour");
	};

	struct BaseMonoBehaviour : FacepunchBehaviour {
		IL2CPP_CLASS("BaseMonoBehaviour");
	};

	struct BaseNetworkable : BaseMonoBehaviour
	{
		struct EntityRealm : Il2CppObject
		{
			using List = System::ListDictionary< std::uint32_t, BaseNetworkable* >;
			IL2CPP_FIELD(List*, entityList);
		};

		IL2CPP_CLASS("BaseNetworkable");
		IL2CPP_STATIC_FIELD(EntityRealm*, clientEntities);
		IL2CPP_FIELD(System::String*, _prefabName);
		IL2CPP_FIELD(System::String*, _prefabNameWithoutExtension);
		IL2CPP_PROPERTY(System::String*, PrefabName);
		IL2CPP_PROPERTY(System::String*, ShortPrefabName);

		IL2CPP_FIELD(std::uintptr_t*, net);

		IL2CPP_FIELD(uint64_t, prefabID);

		auto PlayerDestroyed() -> bool;
		bool PlayerValid();
	};

	struct Model : UnityEngine::MonoBehaviour
	{
		IL2CPP_FIELD(System::Array<System::String*>*, boneNames);
		IL2CPP_FIELD(System::Array<UnityEngine::Transform*>*, boneTransforms);

		IL2CPP_FIELD(UnityEngine::Transform*, headBone);
		IL2CPP_FIELD(UnityEngine::Transform*, rootBone);
		IL2CPP_FIELD(UnityEngine::Transform*, eyeBone);

	};


	struct ViewmodelSway : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("ViewmodelSway");

		IL2CPP_FIELD(float, positionalSwaySpeed);
		IL2CPP_FIELD(float, positionalSwayAmount);
		IL2CPP_FIELD(float, rotationSwaySpeed);
		IL2CPP_FIELD(float, rotationSwayAmount);
	};

	struct ViewmodelBob : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("ViewmodelBob");

		IL2CPP_FIELD(float, bobSpeedWalk);
		IL2CPP_FIELD(float, bobSpeedRun);
		IL2CPP_FIELD(float, leftOffsetRun);
		IL2CPP_FIELD(Vector4, bobAmountWalk);
		IL2CPP_FIELD(Vector4, bobAmountRun);
	};

	struct ViewModelLower : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("ViewModelLower");

		IL2CPP_FIELD(bool, lowerOnSprint);
		IL2CPP_FIELD(bool, lowerWhenCantAttack);
		IL2CPP_FIELD(bool, shouldLower);
	};

	struct BaseViewModel : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("BaseViewModel");

		IL2CPP_STATIC_PROPERTY(BaseViewModel*, ActiveModel);

		IL2CPP_FIELD(ViewmodelSway*, sway);
		IL2CPP_FIELD(ViewModelLower*, lower);
		IL2CPP_FIELD(ViewmodelBob*, bob);
		IL2CPP_FIELD(bool, useViewModelCamera);

	};

	struct BaseEntity : BaseNetworkable {
		IL2CPP_CLASS("BaseEntity");

		IL2CPP_FIELD(System::String*, _name);
		IL2CPP_FIELD(Model*, model);
		IL2CPP_FIELD(RustStructs::BaseEntity_Flags, flags);
		IL2CPP_FIELD(UnityEngine::Bounds, bounds);
		IL2CPP_FIELD(ULONG, skinID);

		void SetSkin(uint64_t skin_id);
		Vector3 ClosestPoint(Vector3 position);
		void ServerRPC(const char* msg);
		void SendSignalBroadcast(RustStructs::Signal a, char* str);
		auto GetParentVelocity() -> Vector3;
		void OnSignal(RustStructs::Signal a, char* str);
		auto GetWorldVelocity() -> Vector3;

		Vector3 GetInheritedThrowVelocity(Vector3 dir)
		{
			if (!this)return Vector3();
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetInheritedThrowVelocity"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<Vector3>(procedure, this, dir);
			}

			TRACE("%s: procedure not a valid!", FN);
			return Vector3();
		}
		

		float Distance(Vector3 pos)
		{
			if (!this)
				return 0.f;
			const auto is_dead = reinterpret_cast<float(*)(BaseEntity*, Vector3)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("Distance"), 1)));
			return is_dead(this, pos);
		}

		void OnSkinRefreshStart()
		{
			if (!this)
				return;

			const auto is_dead = reinterpret_cast<void(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("OnSkinRefreshStart"), 0)));
			return is_dead(this);
		}

		void OnSkinRefreshEnd()
		{
			if (!this)
				return;

			const auto is_dead = reinterpret_cast<void(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("OnSkinRefreshEnd"), 0)));
			return is_dead(this);
		}

		void OnSkinChanged(uint64_t oldSkinID, uint64_t newSkinID)
		{
			if (!this)
				return;

			const auto is_dead = reinterpret_cast<void(*)(BaseEntity*, uint64_t, uint64_t)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("OnSkinChanged"), 2)));
			return is_dead(this, oldSkinID, newSkinID);
		}

		Vector3 PivotPoint()
		{
			if (!this)
				return Vector3();
			const auto is_dead = reinterpret_cast<Vector3(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("PivotPoint"), 0)));
			return is_dead(this);
		}

		float BoundsPadding()
		{
			if (!this)
				return 0.f;
			const auto is_dead = reinterpret_cast<float(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("BoundsPadding"), 0)));
			return is_dead(this);
		}

		float MaxVelocity()
		{
			if (!this)
				return 0.f;
			const auto is_dead = reinterpret_cast<float(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("MaxVelocity"), 0)));
			return is_dead(this);
		}
	};

	struct ResourceEntity : BaseEntity
	{
		IL2CPP_CLASS("ResourceEntity");

		IL2CPP_FIELD(bool, isKilled);
		IL2CPP_FIELD(float, health);
		IL2CPP_FIELD(float, startHealth);
	};


	struct CollectibleEntity : BaseEntity
	{
		IL2CPP_CLASS("CollectibleEntity");
	};

	struct AutoTurret : BaseEntity
	{
		IL2CPP_CLASS("AutoTurret");

		IL2CPP_FIELD(float, sightRange);
	};


	

	struct FlameTurret : BaseEntity
	{
		IL2CPP_CLASS("FlameTurret");
	};

	struct SamSite : BaseEntity
	{
		IL2CPP_CLASS("SamSite");
	};

	struct GamePhysics
	{
		IL2CPP_CLASS("GamePhysics");


		static auto LineOfSightRadius(Vector3 p0, Vector3 p1, int layerMask, float radius, float padding, BaseEntity* ignoreEntity) -> bool;
		static auto LineOfSightRadius_3(Vector3 p0, Vector3 p1, int layerMask, float padding, BaseEntity* ignoreEntity) -> bool;
	
		static auto Trace(UnityEngine::Ray ray, float radius, UnityEngine::RaycastHit hitInfo, float maxDistance, int layerMask, RustStructs::QueryTriggerInteraction triggerInteraction, BaseEntity* ignoreEntity) -> bool;
		static auto LineOfSight(Vector3 p0, Vector3 p1, int layerMask, BaseEntity* ignoreEntity) -> bool;
		static auto Trace2(UnityEngine::Ray* ray, float radius, UnityEngine::RaycastHit hitInfo, float maxDistance, int layerMask, RustStructs::QueryTriggerInteraction triggerInteraction, BaseEntity* ignoreEntity) -> bool;

	};

	inline bool IsVisible(Vector3 source, Vector3 destination, int p1 = 0.2f) {

		int mask = m_options.m_options.pierce ? 10551296 : 1503731969;

		return GamePhysics::LineOfSightRadius_3(source, destination, mask, p1, nullptr);
	}
	inline bool IsVisible_2(Vector3 source, Vector3 destination, int layerm, int p1 = 0.2f) {
		int mask = m_options.m_options.pierce ? 10551296 : 1503731969;

		return GamePhysics::LineOfSightRadius_3(source, destination, mask, p1, nullptr);
	}
	inline bool IsVisible_3(Vector3 source, Vector3 destination) {
		int mask = m_options.m_options.pierce ? 10551296 : 1503731969;

		return GamePhysics::LineOfSight(source, destination, mask, nullptr);
	}
	struct BaseCombatEntity : BaseEntity {
		IL2CPP_CLASS("BaseCombatEntity");

		IL2CPP_FIELD(float, _health);
		IL2CPP_FIELD(float, _maxHealth);
		IL2CPP_FIELD(RustStructs::LifeState, lifestate);

		bool IsDead();
		bool IsAlive();
	};

	struct SleepingBag : BaseCombatEntity
	{
		IL2CPP_CLASS("SleepingBag");

		IL2CPP_FIELD(unsigned long, deployerUserID);
	};

	enum PlayerModelMountPoses // TypeDefIndex: 1518
	{
		// Fields
		Chair = 0,
		Driving = 1,
		Horseback = 2,
		HeliUnarmed = 3,
		HeliArmed = 4,
		HandMotorBoat = 5,
		MotorBoatPassenger = 6,
		SitGeneric = 7,
		SitRaft = 8,
		StandDrive = 9,
		SitShootingGeneric = 10,
		SitMinicopter_Pilot = 11,
		SitMinicopter_Passenger = 12,
		ArcadeLeft = 13,
		ArcadeRight = 14,
		SitSummer_Ring = 15,
		SitSummer_BoogieBoard = 16,
		SitCarPassenger = 17,
		SitSummer_Chair = 18,
		SitRaft_NoPaddle = 19,
		Sit_SecretLab = 20,
		Sit_Workcart = 21,
		Sit_Cardgame = 22,
		Sit_Crane = 23,
		Sit_Snowmobile_Shooting = 24,
		Sit_RetroSnowmobile_Shooting = 25,
		Driving_Snowmobile = 26,
		ZiplineHold = 27,
		Sit_Locomotive = 28,
		Sit_Throne = 29,
		Standing = 128
	};
	struct BaseMountable : BaseCombatEntity
	{
		IL2CPP_CLASS("BaseMountable");

		IL2CPP_FIELD(bool, canWieldItems);
		IL2CPP_FIELD(PlayerModelMountPoses, mountPose);
	};

	struct BaseHelicopter : BaseCombatEntity {
		IL2CPP_CLASS("BaseHelicopter");
		IL2CPP_FIELD(uintptr_t, weakspots);

	};

	struct TranslatePhrase : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TranslatePhrase");
		IL2CPP_FIELD(System::String*, english);
	};

	struct Item;
	struct ItemDefinition : UnityEngine::MonoBehaviour
	{

		IL2CPP_CLASS("ItemDefinition");
		IL2CPP_FIELD(std::int32_t, itemid);
		IL2CPP_FIELD(System::String*, shortname);
		IL2CPP_FIELD(RustStructs::ItemCategory, category);
		IL2CPP_FIELD(TranslatePhrase*, displayName);

		System::String* GetDisplayName(Item* item);
		UnityEngine::Sprite* FindIconSprite(int id);

	};
	struct BaseEntity;
	struct ViewModel : BaseMonoBehaviour
	{
		IL2CPP_CLASS("ViewModel");
		IL2CPP_FIELD(BaseEntity*, instance);
		void Play(const char* name, int layer);
	};
	struct BasePlayer;

	struct HeldEntity : BaseEntity
	{
		IL2CPP_CLASS("HeldEntity");
		IL2CPP_FIELD(ViewModel*, viewModel);

		BasePlayer* GetOwnerPlayer()
		{
			if (!this)
				return {};

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetOwnerPlayer"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<BasePlayer*>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}
	};

	struct AttackEntity : HeldEntity
	{
		IL2CPP_CLASS("AttackEntity");
		IL2CPP_FIELD(float, aiAimSwayOffset);
		IL2CPP_FIELD(float, aiAimCone);
		IL2CPP_FIELD(float, repeatDelay);
		IL2CPP_FIELD(float, timeSinceDeploy);
		IL2CPP_FIELD(float, deployDelay);
		IL2CPP_FIELD(float, nextAttackTime);

		ItemDefinition* GetOwnerItemDefinition();

		bool IsFullyDeployed();
		bool HasAttackCooldown();
		auto StartAttackCooldown(float coolDown) -> void;
	};
	struct ResourceDispenserGatherPropertyEntry : Il2CppObject {
		IL2CPP_CLASS("ResourceDispenser.GatherPropertyEntry");

		IL2CPP_FIELD(float, gatherDamage);
	};

	struct ResourceDispenserGatherProperties : Il2CppObject {
		IL2CPP_CLASS("ResourceDispenser.GatherProperties");

		IL2CPP_FIELD(ResourceDispenserGatherPropertyEntry*, Tree);
		IL2CPP_FIELD(ResourceDispenserGatherPropertyEntry*, Ore);
		IL2CPP_FIELD(ResourceDispenserGatherPropertyEntry*, Flesh);

	};
	struct DamageProperties;

	struct HitTest;
	struct BasePlayer;
	struct BaseMelee : AttackEntity {
		IL2CPP_CLASS("BaseMelee");

		IL2CPP_FIELD(float, maxDistance);
		IL2CPP_FIELD(DamageProperties*, damageProperties);
		IL2CPP_FIELD(ResourceDispenserGatherProperties*, gathering);
		IL2CPP_FIELD(bool, canThrowAsProjectile);

		void DoThrow();
		void ProcessAttack(HitTest* hit);
		Vector3 GetInheritedVelocity(BasePlayer* player, Vector3 dir);
	};


	struct RecoilProperties : UnityEngine::Object
	{
		IL2CPP_CLASS("RecoilProperties");

		IL2CPP_FIELD(RecoilProperties*, newRecoilOverride);
		IL2CPP_FIELD(float, recoilYawMin);
		IL2CPP_FIELD(float, recoilYawMax);
		IL2CPP_FIELD(float, recoilPitchMin);
		IL2CPP_FIELD(float, recoilPitchMax);

		IL2CPP_FIELD(float, timeToTakeMin);
		IL2CPP_FIELD(float, timeToTakeMax);
		IL2CPP_FIELD(float, ADSScale);
		IL2CPP_FIELD(float, movementPenalty);
		IL2CPP_FIELD(float, clampPitch);
	};
	struct DamageProperties : Il2CppObject {
		IL2CPP_CLASS("DamageProperties");

	};

	struct BaseEntity;
	struct Projectile;
	struct HitInfo : Il2CppObject {
		IL2CPP_CLASS("HitInfo");

		IL2CPP_FIELD(float, ProjectileDistance);
		IL2CPP_FIELD(float, ProjectileIntegrity);
		IL2CPP_FIELD(float, ProjectileTravelTime);
		IL2CPP_FIELD(uint64_t, HitMaterial);
		IL2CPP_FIELD(uint64_t, HitBone);
		IL2CPP_FIELD(uint64_t, ProjectileID);

		IL2CPP_FIELD(Vector3, HitPositionWorld);

	};

	struct HitTest : Il2CppObject {
		IL2CPP_CLASS("HitTest");
		IL2CPP_FIELD(Il2CppString*, HitMaterial);
		IL2CPP_FIELD(bool, DidHit);
		IL2CPP_FIELD(bool, BestHit);

		IL2CPP_FIELD(UnityEngine::Ray, AttackRay);
		IL2CPP_FIELD(float, Radius);
		IL2CPP_FIELD(float, Forgiveness);
		IL2CPP_FIELD(float, MaxDistance);
		IL2CPP_FIELD(float, HitDistance);
		IL2CPP_FIELD(int, type);
		IL2CPP_FIELD(int, HitPart);
		IL2CPP_FIELD(UnityEngine::Collider*, collider);
		IL2CPP_FIELD(UnityEngine::GameObject*, gameObject);

		IL2CPP_FIELD(BaseEntity*, HitEntity);
		IL2CPP_FIELD(BaseEntity*, IgnoreEntity);

		IL2CPP_FIELD(Vector3, HitPoint);
		IL2CPP_FIELD(Vector3, HitNormal);
		IL2CPP_FIELD(DamageProperties*, damageProperties);

		IL2CPP_FIELD(UnityEngine::Transform*, HitTransform);



		Vector3 HitPointWorld();
		Vector3 HitNormalWorld();


		ProtoBuf::Attack* BuildAttackMessage()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("BuildAttackMessage"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
					printfz("\n procedure - %p", procedure);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<ProtoBuf::Attack*>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}
	};

	class Object {
	public:

	};
	struct GameManifest
	{
		IL2CPP_CLASS("GameManifest");

		static Object* GUIDToObject(System::String* guid);
	};
	template<typename T = Object>
	class ResourceRef {
	public:
		T* Get() {
			if (!this) return nullptr;
			System::String* guid = *reinterpret_cast<System::String**>(this + 0x10);
			T* _cachedObject = (T*)GameManifest::GUIDToObject(guid);

			return _cachedObject;
		}
	};
	struct ItemModProjectile : public UnityEngine::Component {
		IL2CPP_CLASS("ItemModProjectile");
		IL2CPP_FIELD(ResourceRef<UnityEngine::GameObject>*, projectileObject);
		IL2CPP_FIELD(float, projectileVelocity);
		IL2CPP_FIELD(float, projectileSpread);
		IL2CPP_FIELD(float, projectileVelocitySpread);

		IL2CPP_FIELD(int, numProjectiles);

		float GetRandomVelocity();


	};
	struct BasePlayer;

	struct RaycastHitEx : Il2CppObject {
		IL2CPP_CLASS("RaycastHitEx");


		static BaseEntity* GetEntity(UnityEngine::RaycastHit  hit)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetEntity"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<BaseEntity*>(procedure, hit);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}
	};

	struct ServerProjectile : BaseMonoBehaviour
	{
		IL2CPP_CLASS("ServerProjectile");


		IL2CPP_FIELD(Vector3, initialVelocity);
		IL2CPP_FIELD(float, drag);
		IL2CPP_FIELD(float, gravityModifier);
		IL2CPP_FIELD(float, speed);
		IL2CPP_FIELD(float, scanRange);
		IL2CPP_FIELD(float, radius);

		void _cctor()
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH(".ctor"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}

		}
	};

	struct Projectile : BaseMonoBehaviour
	{
		IL2CPP_CLASS("Projectile");
		IL2CPP_FIELD(float, thickness);
		IL2CPP_FIELD(Vector3, currentPosition);
		IL2CPP_FIELD(Vector3, sentPosition);

		IL2CPP_FIELD(Vector3, currentVelocity);
		IL2CPP_FIELD(Vector3, initialVelocity);
		IL2CPP_FIELD(Vector3, previousPosition);
		IL2CPP_FIELD(Vector3, previousVelocity);
		IL2CPP_FIELD(HitTest*, hitTest);
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
		IL2CPP_FIELD(ItemModProjectile*, mod);
		IL2CPP_FIELD(float, initialDistance);
		//IL2CPP_FIELD(ItemModProjectile*, mod);
		IL2CPP_FIELD(float, integrity);
		IL2CPP_FIELD(BasePlayer*, owner);
		IL2CPP_FIELD(bool, isRicochet);
		IL2CPP_FIELD(AttackEntity*, sourceWeaponPrefab);
		IL2CPP_FIELD(DamageProperties*, damageProperties);
		IL2CPP_FIELD(float, ricochetChance);

		bool IsAlive();
		void UpdateVelocity(float deltaTime);
		bool DoRicochet(HitTest* test, Vector3 point, Vector3 normal);
		bool DoHit(HitTest* test, Vector3 point, Vector3 normal);
		auto Retire() -> void;
		auto get_isAlive() -> bool;
		auto isAuthoritative() -> bool;

		static bool IsWaterMaterial(Il2CppString* name);

		bool Reflect(int seed, Vector3 point, Vector3 normal);

		void FixedUpdate()
		{

			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("FixedUpdate"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}


		void Launch()
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Launch"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		float CalculateEffectScale()
		{
			if (!this) return 0.f;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("CalculateEffectScale"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<float>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return 0.f;
		}

		void DoFlybySound()
		{
			if (!this)return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("DoFlybySound"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		void SetEffectScale(float eScale)
		{
			if (!this)return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SetEffectScale"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, eScale);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

	};


	struct Magazine : AttackEntity {

		IL2CPP_CLASS("BaseProjectile.Magazine");
		IL2CPP_FIELD(ItemDefinition*, ammoType);
		IL2CPP_FIELD(int, contents);
		IL2CPP_FIELD(int, capacity);

		bool CanReload(BasePlayer* owner)
		{
			if (!this)return false;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("CanReload"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, this, owner);
			}

			TRACE("%s: procedure not a valid!", FN);
			return false;
		}
	};

	struct BaseProjectile : AttackEntity
	{
		IL2CPP_CLASS("BaseProjectile");
		IL2CPP_FIELD(bool, automatic);
		IL2CPP_FIELD(float, reloadTime);
		IL2CPP_FIELD(float, stancePenalty);
		IL2CPP_FIELD(float, reloadPressTime);
		IL2CPP_FIELD(float, aimconePenalty);
		IL2CPP_FIELD(float, aimCone);
		IL2CPP_FIELD(float, hipAimCone);
		IL2CPP_FIELD(float, aimconePenaltyPerShot);
		IL2CPP_FIELD(float, deployDelay);
		IL2CPP_FIELD(float, animationDelay);
		IL2CPP_FIELD(UnityEngine::Transform*, MuzzlePoint);
		IL2CPP_FIELD(System::List<Projectile*>*, createdProjectiles);
		IL2CPP_FIELD(RecoilProperties*, recoil);
		IL2CPP_FIELD(float, aimSway);
		IL2CPP_FIELD(float, aimSwaySpeed);
		IL2CPP_FIELD(Magazine*, primaryMagazine);
		IL2CPP_FIELD(float, projectileVelocityScale);
		IL2CPP_FIELD(float, lastShotTime);
		IL2CPP_FIELD(float, nextReloadTime);
		IL2CPP_FIELD(float, repeatDelay);
		IL2CPP_FIELD(bool, triggerReady);
		IL2CPP_FIELD(bool, manualCycle);
		IL2CPP_FIELD(bool, needsCycle);
		IL2CPP_PROPERTY(bool, isSemiAuto);
		IL2CPP_FIELD(bool, UsingInfiniteAmmoCheat);
		IL2CPP_FIELD(bool, isReloading);
		IL2CPP_FIELD(bool, fractionalReload);
		IL2CPP_FIELD(bool, isCycling);

		bool HasReloadCooldown();
		float GetProjectileVelocityScale(bool max);

		void ShotFired();
		void BeginCycle();
		void DidAttackClientside();
		void UpdateAmmoDisplay();
		void LaunchProjectile();

		void DoAttack();

		void DoAttackRecreation()
		{
			auto ownerPlayer = this->GetOwnerPlayer();
			if (!ownerPlayer)
				return;

			this->triggerReady() = true;

			if (this->manualCycle() && this->needsCycle())
			{
				this->BeginCycle();
				return;
			}

			if (this->HasAttackCooldown())
			{
				return;
			}

			if (this->get_isSemiAuto())
			{
				this->triggerReady() = false;
			}
			if (this->primaryMagazine()->contents() <= 0)
			{
				return;
			}
			this->primaryMagazine()->contents()--;
			this->SendSignalBroadcast(RustStructs::Signal::Attack, SECURE(""));
			if (this->viewModel())
			{
				this->viewModel()->Play(SECURE("attack"), 0);
			}
			this->LaunchProjectile();
			this->UpdateAmmoDisplay();
			this->ShotFired();
			this->DidAttackClientside();
			if (!this->manualCycle())
			{
				this->BeginCycle();
				return;
			}
			this->needsCycle() = true;
		}
	};

	struct TorchWeapon : BaseMelee {
		IL2CPP_CLASS("TorchWeapon");

	};

	struct ThrownWeapon : AttackEntity {
		IL2CPP_CLASS("ThrownWeapon");

	};

	struct FlameThrower : AttackEntity {
		IL2CPP_CLASS("FlameThrower");

	};
	struct BowWeapon : AttackEntity {
		IL2CPP_CLASS("BowWeapon");

		IL2CPP_FIELD(bool, attackReady);
		IL2CPP_FIELD(float, arrowBack);
	};

	struct SnowballGun : AttackEntity {
		IL2CPP_CLASS("SnowballGun");


	};

	struct CompoundBowWeapon : AttackEntity {
		IL2CPP_CLASS("CompoundBowWeapon");

		IL2CPP_FIELD(float, stringHoldDurationMax);
		IL2CPP_FIELD(float, movementPenalty);
		IL2CPP_FIELD(float, currentHoldProgress);
	};


	struct CrossbowWeapon : AttackEntity {
		IL2CPP_CLASS("CrossbowWeapon");

	};

	struct FlintStrikeWeapon : BaseProjectile {
		IL2CPP_CLASS("FlintStrikeWeapon");

		IL2CPP_FIELD(bool, didSparkThisFrame);
		IL2CPP_FIELD(float, successFraction);


	};

	struct Chainsaw : BaseMelee {
		IL2CPP_CLASS("Chainsaw");

	};

	struct MedicalTool : AttackEntity {
		IL2CPP_CLASS("MedicalTool");

		IL2CPP_FIELD(float, healDurationSelf);
	};

	struct BaseLauncher : AttackEntity {
		IL2CPP_CLASS("BaseLauncher");

	};
	struct BaseVehicle : BaseEntity {
		IL2CPP_CLASS("BaseVehicle");

	};

	struct Item : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("Item");
		IL2CPP_FIELD(std::uint32_t, uid);
		IL2CPP_FIELD(ItemDefinition*, info);
		IL2CPP_FIELD(HeldEntity*, heldEntity);
		IL2CPP_FIELD(ULONG, skin);
		IL2CPP_FIELD(std::uint32_t, amount);
		IL2CPP_FIELD(float, _condition);
		IL2CPP_FIELD(float, _maxCondition);

		template<typename T = void*>
		T* HeldEntityCast() {
			if (!this) return nullptr;
			return *reinterpret_cast<T**>(this->heldEntity());
		}


		void MarkDirty()
		{
			if (!this)return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("MarkDirty"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		BaseEntity* GetHeldEntity();

		IL2CPP_PROPERTY(UnityEngine::Sprite*, iconSprite);

		System::String* GetItemName();
		System::String* GetItemShortName();

		void SetSkin(uint64_t skin_id);
	};
	struct TOD_AtmosphereParameters : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_AtmosphereParameters");

		IL2CPP_FIELD(float, RayleighMultiplier); //Intensity of the atmospheric Rayleigh scattering
		IL2CPP_FIELD(float, MieMultiplier); //Intensity of the atmospheric Mie scattering
		IL2CPP_FIELD(float, Brightness); //Overall brightness of the atmosphere
		IL2CPP_FIELD(float, Contrast); //Overall contrast of the atmosphere
		IL2CPP_FIELD(float, Directionality); //Directionality factor that determines the size of the glow around the sun
		IL2CPP_FIELD(float, Fogginess); //Density of the fog covering the sky
	};

	struct Gradient : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("Gradient");

		IL2CPP_FIELD(Vector4, m_Ptr); //0x10
	};

	struct TOD_DayParameters : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_DayParameters");

		IL2CPP_FIELD(Gradient*, SkyColor); //Intensity of the light source
		IL2CPP_FIELD(float, LightIntensity); //Intensity of the light source
		IL2CPP_FIELD(float, ShadowStrength); //Opacity of the shadows dropped by the light source
		IL2CPP_FIELD(float, AmbientMultiplier); //Brightness multiplier of the ambient light
		IL2CPP_FIELD(float, ReflectionMultiplier); //Brightness multiplier of the reflection probe
		
	};

	struct TOD_NightParameters : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_NightParameters");

		IL2CPP_FIELD(float, LightIntensity); //Intensity of the light source
		IL2CPP_FIELD(float, ShadowStrength); //Opacity of the shadows dropped by the light source
		IL2CPP_FIELD(float, AmbientMultiplier); //Brightness multiplier of the ambient light
		IL2CPP_FIELD(float, ReflectionMultiplier); //Brightness multiplier of the reflection probe
	};

	struct TOD_StarParameters : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_StarParameters");

		IL2CPP_FIELD(float, Size); //Size of the stars
		IL2CPP_FIELD(float, Brightness); //Brightness of the stars
	};

	struct TOD_CloudParameters : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_CloudParameters");

		IL2CPP_FIELD(float, Size); //Size of the clouds
		IL2CPP_FIELD(float, Opacity); //Opacity of the clouds
		IL2CPP_FIELD(float, Coverage); //How much sky is covered by clouds
		IL2CPP_FIELD(float, Sharpness); //Sharpness of the cloud to sky transition
		IL2CPP_FIELD(float, SharColoringpness); //Coloring of the clouds
		IL2CPP_FIELD(float, Attenuation); //Amount of skylight that is blocked
		IL2CPP_FIELD(float, Brightness); //Brightness of the clouds
	};

	struct TOD_AmbientParameters : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_AmbientParameters");

		IL2CPP_FIELD(float, Saturation); //Saturation of the ambient light
		IL2CPP_FIELD(float, UpdateInterval); //Refresh interval of the ambient light probe in seconds
	};
	struct TOD_Scattering : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_Scattering");
		IL2CPP_FIELD(UnityEngine::Shader*, ScatteringShader);
	};

	struct TOD_Components : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_Components");

		IL2CPP_FIELD(UnityEngine::Component*, Atmosphere);

		inline TOD_Scattering* Skattering()
		{
			if (!this) return {};

			static std::size_t offset = 0;
			const auto field = CIl2Cpp::FindField(this->StaticClass(), HASH("<BillboardMaterials>k__BackingField"));
			if (memory::IsAddressValid(field))
			{
				offset = CIl2Cpp::il2cpp_field_get_offset(field);
			}

			return *reinterpret_cast<TOD_Scattering**>(memory::ToAddress(this) + offset);
			//<Resources>k__backingfield
		}
		inline System::Array<UnityEngine::Renderer*>* BillboardRenderers()
		{
			if (!this) return {};

			static std::size_t offset = 0;
			const auto field = CIl2Cpp::FindField(this->StaticClass(), HASH("<BillboardRenderers>k__BackingField"));
			if (memory::IsAddressValid(field))
			{
				offset = CIl2Cpp::il2cpp_field_get_offset(field);
			}

			return *reinterpret_cast<System::Array<UnityEngine::Renderer*>**>(memory::ToAddress(this) + offset);
			//<Resources>k__backingfield
		}

		inline System::Array<UnityEngine::Material*>* BillboardMaterials()
		{
			if (!this) return {};

			static std::size_t offset = 0;
			const auto field = CIl2Cpp::FindField(this->StaticClass(), HASH("<BillboardMaterials>k__BackingField"));
			if (memory::IsAddressValid(field))
			{
				offset = CIl2Cpp::il2cpp_field_get_offset(field);
			}

			return *reinterpret_cast<System::Array<UnityEngine::Material*>**>(memory::ToAddress(this) + offset);
			//<Resources>k__backingfield
		}
	};

	struct TOD_Resources : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_Resources");

		IL2CPP_FIELD(UnityEngine::Material*, Skybox);
	};

	struct TOD_Sky : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("TOD_Sky");

		IL2CPP_PROPERTY(TOD_Components*, Components);

		IL2CPP_STATIC_FIELD(System::List<TOD_Sky*>*, instances);

		IL2CPP_STATIC_PROPERTY(TOD_Sky*, Instance);

		IL2CPP_FIELD(TOD_AtmosphereParameters*, Atmosphere);
		IL2CPP_FIELD(TOD_DayParameters*, Day);
		IL2CPP_FIELD(TOD_NightParameters*, Night);
		IL2CPP_FIELD(TOD_StarParameters*, Stars);
		IL2CPP_FIELD(TOD_CloudParameters*, Clouds);
		IL2CPP_FIELD(TOD_AmbientParameters*, Ambient);


		inline TOD_Resources* Resources()
		{
			if (!this) return {};

			static std::size_t offset = 0;
			const auto field = CIl2Cpp::FindField(this->StaticClass(), HASH("<Resources>k__backingfield"));
			if (memory::IsAddressValid(field))
			{
				offset = CIl2Cpp::il2cpp_field_get_offset(field);
			}

			return *reinterpret_cast<TOD_Resources**>(memory::ToAddress(this) + offset);
			//<Resources>k__backingfield
		}

		inline static TOD_Sky* GetClassReference()
		{
			static TOD_Sky* stored_pointer = nullptr;
			if (stored_pointer == nullptr)
			{
				const auto instances_field = instances();

				if (!memory::IsAddressValid(instances_field))
				{
					return nullptr;
				}

				if (instances_field->_size > 0)
				{
					const auto items = instances_field->_items;
					if (memory::IsAddressValid(items))
					{
						const auto first_instance = items->m_Items[0];
						if (memory::IsAddressValid(first_instance))
						{
							stored_pointer = first_instance;
						}
					}
				}
			}

			return stored_pointer;
		}
	};

	struct ItemContainerId // TypeDefIndex: 9515
	{
		// Fields
		unsigned long Value; // 0x0
	};

	struct ItemContainer : Item {
		IL2CPP_CLASS("ItemContainer");
		IL2CPP_FIELD(System::List<Item*>*, itemList);

		IL2CPP_FIELD(ItemContainerId, uid);

		Item* GetItem(int id);
	};


	struct PlayerInventory : Il2CppObject {
		IL2CPP_CLASS("PlayerInventory");

		IL2CPP_FIELD(ItemContainer*, containerMain);
		IL2CPP_FIELD(ItemContainer*, containerBelt);
		IL2CPP_FIELD(ItemContainer*, containerWear);

		Item* FindItemUID(int id);
		ItemContainer* get_belt();
	};

	struct SkinnedMultiMesh : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("SkinnedMultiMesh");

		IL2CPP_PROPERTY(System::List<UnityEngine::Renderer*>*, Renderers);
	};

	struct PlayerModel : FacepunchBehaviour
	{

		IL2CPP_CLASS("PlayerModel");
		IL2CPP_FIELD(SkinnedMultiMesh*, _multiMesh);
		IL2CPP_FIELD(bool, isLocalPlayer);
		IL2CPP_FIELD(bool, visible);
		IL2CPP_FIELD(UnityEngine::Vector3, velocity);
		IL2CPP_FIELD(UnityEngine::Vector3, newVelocity);
		//IL2CPP_FIELD(UnityEngine::BoxCollider*, collision);
		IL2CPP_PROPERTY(bool, IsNpc);
		IL2CPP_FIELD(Vector3, position);
		IL2CPP_FIELD(UnityEngine::GameObject*, headBone);

		bool isNpc();

		static void RebuildAll()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("RebuildAll"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}
	};

	struct PlayerEyes : UnityEngine::Component {
		IL2CPP_CLASS("PlayerEyes");

		IL2CPP_FIELD(Vector3, viewOffset);
		IL2CPP_FIELD(Vector3, EyeOffset);
		IL2CPP_PROPERTY(Vector3, position);
		IL2CPP_PROPERTY(Vector3, center);
		IL2CPP_PROPERTY(Vector4, rotation);

		void set_position(Vector3 pos);
		Vector3 MovementRight();
		Vector3 MovementForward();
		Vector4 bodyRotation();
		Vector3 BodyForward();
		Vector3 BodyRight();

		void SetBodyRotation(Vector4 rot);

		void DoFirstPersonCamera(UnityEngine::Camera* cam) {
			if (!this)
				return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("DoFirstPersonCamera"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, cam);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		UnityEngine::Ray* BodyRay()
		{

			if (!this)
				return {};

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("BodyRay"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<UnityEngine::Ray*>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}
	};
	struct ModelState : Il2CppObject
	{
		IL2CPP_CLASS("ModelState");

		IL2CPP_FIELD(float, waterLevel);
		IL2CPP_FIELD(int, flags);
		IL2CPP_FIELD(int, poseType);

		IL2CPP_PROPERTY(bool, mounted);

		auto SetMounted(bool value) -> void;
		auto get_ducked() -> bool;

		auto SetOnLadder(bool value) -> void;

		void SetSprinting(bool Value);


	

		void remove_flag(RustStructs::ModelState_Flag flag);
		void set_flag(RustStructs::ModelState_Flag flag);


	};

	struct PlayerInput : Il2CppObject {
		IL2CPP_CLASS("PlayerInput");


		IL2CPP_FIELD(InputState*, state);
		IL2CPP_FIELD(Vector2, bodyAngles);
		IL2CPP_FIELD(Vector2, recoilAngles);
	};

	struct OBB // TypeDefIndex: 12983
	{
		// Fields
		Vector4 rotation; // 0x0
		Vector3 position; // 0x10
		Vector3 extents; // 0x1C
		Vector3 forward; // 0x28
		Vector3 right; // 0x34
		Vector3 up; // 0x40
		float reject; // 0x4C

	};

	struct BaseMovement : UnityEngine::MonoBehaviour
	{
		IL2CPP_CLASS("BaseMovement");

		IL2CPP_FIELD(bool, adminCheat);
		IL2CPP_FIELD(float, adminSpeed);

		IL2CPP_PROPERTY(Vector3, TargetMovement);

		void set_Grounded(float value);
		void set_TargetMovement(Vector3 value);
		float Crawling();
		Vector3 InheritedVelocity();
		float ducking();
		void TeleportTo(Vector3 pos, BasePlayer* player);
	};

	struct PlayerWalkMovement : BaseMovement
	{
		IL2CPP_CLASS("PlayerWalkMovement");

		IL2CPP_FIELD(float, groundAngle);
		IL2CPP_FIELD(float, groundAngleNew);
		IL2CPP_FIELD(float, gravityMultiplier);
		IL2CPP_FIELD(float, maxAngleClimbing);
		IL2CPP_FIELD(float, maxAngleWalking);
		IL2CPP_FIELD(float, groundTime);
		IL2CPP_FIELD(bool, flying);
		IL2CPP_FIELD(bool, wasJumping);
		IL2CPP_FIELD(bool, swimming);
		IL2CPP_FIELD(bool, jumping);
		IL2CPP_FIELD(float, capsuleHeight);
		IL2CPP_FIELD(bool, sprintForced);
		IL2CPP_FIELD(bool, grounded);

		IL2CPP_FIELD(float, jumpTime);
		IL2CPP_FIELD(float, landTRigidbodyime);
		IL2CPP_FIELD(float, landTime);

		IL2CPP_FIELD(UnityEngine::Rigidbody*, body);
		IL2CPP_FIELD(UnityEngine::CapsuleCollider*, capsule);

		void Jump(AssemblyCSharp::ModelState* ms);

		void TeleportTo(Vector3 pos, BasePlayer* player);
		Vector3 FallVelocity();
	};

	struct PlayerTick : Il2CppObject
	{
		IL2CPP_CLASS("PlayerTick");

		IL2CPP_FIELD(Vector3, position);
		IL2CPP_FIELD(Vector3, eyePos);
	};

	struct AssetNameCache : Il2CppObject {
		IL2CPP_CLASS("AssetNameCache");

		static System::String* GetName(uintptr_t a1)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetName"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::String*>(procedure, a1);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}
	};

	struct LocalPlayer;
	struct BasePlayer : BaseHelicopter
	{
		IL2CPP_CLASS("BasePlayer");
		IL2CPP_FIELD(PlayerModel*, playerModel);
		IL2CPP_FIELD(PlayerInventory*, inventory);
		IL2CPP_FIELD(std::uint32_t, playerFlags);
		IL2CPP_FIELD(float, lastSentTickTime);
		IL2CPP_FIELD(float, clientTickInterval);
		IL2CPP_FIELD(bool, wasDead);
		IL2CPP_FIELD(bool, clothingBlocksAiming);
		IL2CPP_FIELD(float, clothingMoveSpeedReduction);
		IL2CPP_FIELD(PlayerEyes*, eyes);
		IL2CPP_FIELD(ModelState*, modelState);
		IL2CPP_FIELD(PlayerInput*, input);
		IL2CPP_FIELD(BaseMountable*, mounted);
		IL2CPP_FIELD(PlayerWalkMovement*, movement);
		IL2CPP_FIELD(PlayerTick*, lastSentTick);
	//	IL2CPP_FIELD(Vector3, lookingAtPoint);


		bool OnLadder()
		{
			if (!this) return false;
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("OnLadder"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return false;
		}

		void ForcePositionTo(Vector3 Pos)
		{
			if (!this)return;
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("ForcePositionTo"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, Pos);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}
		

		void ChatMessage(const char* msg)
		{

			if (!this)return;
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("ChatMessage"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, CIl2Cpp::il2cpp_string_new(msg));
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		bool IsServerFalling()
		{

			if (!this)return {};
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsServerFalling"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}


		inline Vector3 lookingAtPoint()
		{
			if (!this) return {};

			static std::size_t offset = 0;
			const auto field = CIl2Cpp::FindField(this->StaticClass(), HASH("<lookingAtPoint>k__BackingField"));
			if (memory::IsAddressValid(field))
			{
				offset = CIl2Cpp::il2cpp_field_get_offset(field);
			}

			return *reinterpret_cast<Vector3*>(memory::ToAddress(this) + offset);
			//<Resources>k__backingfield
		}
		IL2CPP_FIELD(uint64_t, userID);
		IL2CPP_FIELD(System::String*, UserIDString);
		IL2CPP_FIELD(uint64_t, currentTeam);
		IL2CPP_FIELD(uintptr_t, clientTeam);

		using List = System::ListDictionary< uint64_t, BasePlayer* >;
		IL2CPP_STATIC_FIELD(List*, visiblePlayerList);
		IL2CPP_STATIC_PROPERTY(bool, isMounted);

		void SendClientTick();
		auto OnLand(float fVelocity) -> float;

		auto IsWeapon() -> bool;
		auto IsMelee() -> bool;
		static inline int ClosesestToCrosshair(uintptr_t player);
		auto GetMaxSpeed() -> float;
		auto MaxEyeVelocity() -> float;

		OBB WorldSpaceBounds();

		bool IsSameTeam(BasePlayer* player)
		{
			return currentTeam() == player->currentTeam();
		}

		
		UnityEngine::Bounds GetBounds()
		{
			if (!this)return {};
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetBounds"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<UnityEngine::Bounds>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}


		BaseMovement* GetBaseMovement();

		void SendProjectileRicochet(ProtoBuf::PlayerProjectileRicochet* ricochet);
		void SendProjectileAttack(ProtoBuf::PlayerProjectileAttack* ricochet);
		void SendProjectileUpdate(ProtoBuf::PlayerProjectileUpdate* ricochet);

		bool IsRunning();
		bool IsSwimming();
		float GetRadius();
		float GetHeight(bool ducked);

		void SetVA(const Vector2& VA);
		Vector2 GetVA();
		Vector2 GetRA();

		class Target {
		public:
			Vector3 m_position;
			BasePlayer* m_player;
			int m_bone = 47;
			float m_distance = 10000;
			float m_fov = m_aimbot.m_aimbot.AimbotFOV;
			Vector3 m_velocity;

			bool m_team = false;
			bool m_npc = false;
			bool m_heli = false;
			bool m_sleeping = false;
			bool m_visible = false;

			bool operator<(const Target& b) {
				if (m_fov == m_aimbot.m_aimbot.AimbotFOV) {
					return m_distance < b.m_distance;
				}
				else {
					return m_fov < b.m_fov;
				}
			}
		};
		static Target GetTarget(Vector3 Source, float MaxDist);

		bool IsLocalPlayer();
		bool HasFlag(uint16_t flag);
		bool IsReceivingSnapshot();
		bool IsConnected();
		bool IsSleeper();
		bool IsAdmin();

		bool IsSleeping();

		auto BoundsPadding() -> float;

		bool is_teammate(BasePlayer* local_player) {
			auto team = local_player->clientTeam();

			auto member = *reinterpret_cast<uintptr_t*>(team + 0x28);

			auto size = *reinterpret_cast<int*>(member + 0x18);

			auto list = *reinterpret_cast<uintptr_t*>(member + 0x10);

			auto steam_id = this->userID();

			for (int i = 0; i < size; i++) {
				auto player = *reinterpret_cast<uintptr_t*>(list + 0x20 + i * 0x8);

				auto id = *reinterpret_cast<unsigned long*>(player + 0x18);

				if (id == steam_id)
					return true;
			}
			return false;
		}
		System::String* get_displayName();
		std::string* get_displayNameTest();
		auto GetJumpHeight() -> float;

		std::uint32_t GetHeldItemID();
		auto ActiveItem() -> Item*;
		Item* GetHeldItemSafe();

		template<typename T>
		T* GetHeldEntityCast() {
			if (!this) return {};
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetHeldEntity"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<T*>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}


		UnityEngine::Transform* get_bone_transform(int bone_id);
	};

	struct ConsoleNetwork : Il2CppObject {
		IL2CPP_CLASS("ConsoleNetwork");

		static bool ClientRunOnServer(Unity_String strCommand);
	};

	struct Graphics : Il2CppObject {
		IL2CPP_CLASS("Graphics");

		static void SetFov(float value);
	};

	struct BaseAnimalNPC : Il2CppObject
	{
		IL2CPP_CLASS("BaseAnimalNPC");
	};

	struct StorageContainer : Il2CppObject
	{
		IL2CPP_CLASS("StorageContainer");
	};

	struct BoxStorage : StorageContainer
	{
		IL2CPP_CLASS("BoxStorage");
	};

	struct Recycler : StorageContainer
	{
		IL2CPP_CLASS("Recycler");
	};

	struct Door : Il2CppObject {
		IL2CPP_CLASS("Door");

		void KnockDoor(AssemblyCSharp::BasePlayer* player);
	};

	struct LootContainer : StorageContainer
	{
		IL2CPP_CLASS("LootContainer");
	};

	struct HackableLockedCrate : LootContainer
	{
		IL2CPP_CLASS("HackableLockedCrate");
		IL2CPP_FIELD(float, requiredHackSeconds);
	};

	struct DroppedItemContainer : BaseCombatEntity
	{
		IL2CPP_CLASS("DroppedItemContainer");

		IL2CPP_FIELD(System::String*, _playerName);
		IL2CPP_FIELD(System::String*, lootPanelName);

	};

	struct BaseLadder : BaseCombatEntity
	{
		IL2CPP_CLASS("BaseLadder");

	};

	struct StashContainer : StorageContainer
	{
		IL2CPP_CLASS("StashContainer");
	};

	struct Bear : BaseAnimalNPC
	{
		IL2CPP_CLASS("Bear");
	};
	struct Polarbear : BaseAnimalNPC
	{
		IL2CPP_CLASS("Polarbear");
	};

	struct Stag : BaseAnimalNPC
	{
		IL2CPP_CLASS("Stag");
	};

	struct Boar : BaseAnimalNPC
	{
		IL2CPP_CLASS("Boar");
	};

	struct Wolf : BaseAnimalNPC
	{
		IL2CPP_CLASS("Wolf");
	};

	struct Chicken : BaseAnimalNPC
	{
		IL2CPP_CLASS("Chicken");
	};

	struct PlayerNameID : Il2CppObject
	{
		IL2CPP_CLASS("PlayerNameID");

		IL2CPP_FIELD(System::String*, username);
		IL2CPP_FIELD(uint64_t, userid);
	};

	struct BuildingPrivlidge : StorageContainer
	{
		IL2CPP_CLASS("BuildingPrivlidge");
		IL2CPP_FIELD(float, cachedProtectedMinutes);
		IL2CPP_FIELD(float, cachedUpkeepPeriodMinutes);
		IL2CPP_FIELD(float, cachedUpkeepCostFraction);
		IL2CPP_FIELD(System::List<ProtoBuf::PlayerNameID*>*, authorizedPlayers);

		float CalculateUpkeepCostFraction() {
			if (!this) return 0.f;
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("CalculateUpkeepCostFraction"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<float>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return 0.f;
		}

		float UpKeep() {
			if (!this) return 0.f;
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetProtectedMinutes"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<float>(procedure, this, true);
			}

			TRACE("%s: procedure not a valid!", FN);
			return 0.f;
		}
	};





	struct Workbench : StorageContainer
	{
		IL2CPP_CLASS("Workbench");
	};

	struct BaseOven : StorageContainer
	{
		IL2CPP_CLASS("BaseOven");
	};

	struct ResearchTable : StorageContainer
	{
		IL2CPP_CLASS("ResearchTable");
	};

	struct ReclaimBackpack : StorageContainer
	{
		IL2CPP_CLASS("ReclaimBackpack");
	};

	struct WorldItem : BaseEntity
	{
		IL2CPP_CLASS("WorldItem");
		IL2CPP_FIELD(Item*, item);
	};

	struct DroppedItem : WorldItem
	{
		IL2CPP_CLASS("DroppedItem");
	};


	struct BradleyAPC : BaseCombatEntity
	{
		IL2CPP_CLASS("BradleyAPC");
	};

	struct BaseCorpse : BaseCombatEntity
	{
		IL2CPP_CLASS("BaseCorpse");
	};

	struct LootableCorpse : BaseCorpse
	{
		IL2CPP_CLASS("LootableCorpse");
	};
	
	struct PlayerCorpse : LootableCorpse
	{
		IL2CPP_CLASS("PlayerCorpse");
	};

	struct HTNPlayer : BasePlayer
	{
		IL2CPP_CLASS("HTNPlayer");
	};
	struct ScientistNPC : BasePlayer
	{
		IL2CPP_CLASS("ScientistNPC");
	};

	struct GingerbreadNPC : BasePlayer
	{
		IL2CPP_CLASS("GingerbreadNPC");
	};

	struct TunnelDweller : BasePlayer
	{
		IL2CPP_CLASS("TunnelDweller");
	};

	struct UnderwaterDweller : BasePlayer
	{
		IL2CPP_CLASS("UnderwaterDweller");
	};


	struct Hammer : BaseMelee {
		IL2CPP_CLASS("Hammer");

	};
	struct Plannel_Guide : HeldEntity {

	};

	struct Planner : HeldEntity {
		IL2CPP_CLASS("Planner");

		IL2CPP_FIELD(Vector3, rotationOffset);
		IL2CPP_FIELD(Plannel_Guide*, guide);

	};
	
	struct ItemId// TypeDefIndex: 9516
	{
		// Fields
		unsigned long Value; // 0x0
	};

	struct LocalPlayer : Il2CppObject
	{
		IL2CPP_CLASS("LocalPlayer");

		IL2CPP_STATIC_PROPERTY(BasePlayer*, Entity);

		static ItemContainer* GetLootContainer(int index)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetLootContainer"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<ItemContainer*>(procedure, index);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}

		static void MoveItem(uint32_t itemid, uint32_t targetContainer, int targetSlot, int amount, bool altMove)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("MoveItem"), 5);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, itemid, targetContainer, targetSlot, amount, altMove);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}
	};


	struct ItemIcon : UnityEngine::MonoBehaviour {
		IL2CPP_CLASS("ItemIcon");

		using List = System::ListDictionary< int, float >;
		IL2CPP_FIELD(List*, containerLootStartTimes);
		IL2CPP_FIELD(void*, timedAction);
		IL2CPP_FIELD(Vector2, timedActionTime);
		IL2CPP_FIELD(bool, queuedForLooting);
		IL2CPP_FIELD(Item*, item);

		IL2CPP_FIELD(float, queuedLootAtTime);



		auto StartTimedLootAction(float delay) -> void;

		void RunTimedAction();

		auto SetTimedLootAction(int a1, void* a2) -> void;
	};

	struct BaseNpc : BaseCombatEntity
	{
		IL2CPP_CLASS("BaseNpc");

		IL2CPP_FIELD(float, AttackRange);
	};

	struct NPCPlayer : BasePlayer
	{
		IL2CPP_CLASS("NPCPlayer");
	};

	struct GunTrap : StorageContainer
	{
		IL2CPP_CLASS("GunTrap");
	};
	struct SupplyDrop : StorageContainer
	{
		IL2CPP_CLASS("SupplyDrop");
	};
	struct RidableHorse : BaseCombatEntity
	{
		IL2CPP_CLASS("RidableHorse");
	};
	struct OreHotSpot : BaseCombatEntity
	{
		IL2CPP_CLASS("OreHotSpot");
	};

	struct MainMenuSystem : UnityEngine::Component
	{
		IL2CPP_CLASS("MainMenuSystem");
	};

	struct ExplosionsFPS
	{
		IL2CPP_CLASS("ExplosionsFPS");
	};

	struct AimConeUtil : UnityEngine::MonoBehaviour {
		IL2CPP_CLASS("AimConeUtil");

		static Vector3 GetModifiedAimConeDirection(float aimCone, Vector3 inputVec, bool anywhereInside);
	};


	

	struct DecayEntity : BaseCombatEntity
	{
		IL2CPP_CLASS("DecayEntity");
	};

	struct StabilityEntity : DecayEntity
	{
		IL2CPP_CLASS("StabilityEntity");
	};

	struct BuildingBlock : StabilityEntity
	{
		IL2CPP_CLASS("BuildingBlock");


		void UpgradeToGrade(RustStructs::BuildingGrade grade, int xd, BasePlayer* player);

		bool CanChangeToGrade(RustStructs::BuildingGrade grade, int xd, BasePlayer* player);

		bool CanAffordUpgrade(RustStructs::BuildingGrade grade, int xd, BasePlayer* player);
		bool IsUpgradeBlocked();


	};

	struct StringPool : Il2CppObject {
		IL2CPP_CLASS("StringPool");

		static uint64_t Get(const char* name);

	};

	struct GameTrace : Il2CppObject {
		IL2CPP_CLASS("GameTrace");

		static bool Trace(HitTest* test, int layerMask);

		static void TraceAll(uintptr_t a1, uintptr_t a2, int a3)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("TraceAll"), 3);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, a1, a2, a3);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}
	};

	struct TerrainCollision : Il2CppObject {
		IL2CPP_CLASS("TerrainCollision");


		bool GetIgnore(Vector3 pos, float radius)
		{
			if (!this)
				return false;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetIgnore"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, this, pos, radius);
			}

			TRACE("%s: procedure not a valid!", FN);
			return 0.f;
		}
	};


	struct TerrainHeightMap : Il2CppObject
	{
		IL2CPP_CLASS("TerrainHeightMap");

		float GetHeight(Vector3 pos)
		{
			if (!this)
				return false;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetHeight"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<float>(procedure, this, pos);
			}

			TRACE("%s: procedure not a valid!", FN);
			return 0.f;
		}
	};


	struct TerrainMeta : Il2CppObject {
		IL2CPP_CLASS("TerrainMeta");


		IL2CPP_STATIC_PROPERTY(UnityEngine::Terrain_C*, Terrain);
		IL2CPP_STATIC_PROPERTY(TerrainHeightMap*, HeightMap);
		IL2CPP_STATIC_PROPERTY(TerrainCollision*, Collision);
		IL2CPP_STATIC_PROPERTY(Vector3, Position);

	};

	struct EffectLibrary : Il2CppObject {
		IL2CPP_CLASS("EffectLibrary");

	};

	struct Client : Il2CppObject {
		IL2CPP_CLASS("Client");
		IL2CPP_FIELD(uintptr_t*, authTicket);
			
		const wchar_t* GetOSName()
		{
			if (!this) return L"";

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetOSName"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<wchar_t*>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return L"";
			}
		}
	};

	struct SteamClientWrapper : Il2CppObject {
		IL2CPP_CLASS("SteamClientWrapper");

		void _cctor()
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH(".ctor"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}

		}

		UnityEngine::Texture2D* GetAvatarTexture(uint64_t steamid)
		{
			if (!this)
				return {};

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetAvatarTexture"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<UnityEngine::Texture2D*>(procedure, this, steamid);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}

		System::String* GetSteamUserName(uint64_t steamid)
		{
			if (!this)
				return {};

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetUserName"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::String*>(procedure, this, steamid);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}

	};

	struct CSOption {
		bool IsServer; // 0x0
		bool IsClient;// 0x1
		bool ForwardtoServerOnMissing; // 0x2
		bool PrintOutput; // 0x3
		bool IsUnrestricted; // 0x4
		bool FromRcon; // 0x5
		bool IsFromServer; // 0x6
	};

	struct ConsoleSystem : Il2CppObject {
		IL2CPP_CLASS("ConsoleSystem");

		static System::String* Run(CSOption* option, const char* cmd, uintptr_t* args)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Run"), 3);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::String*>(procedure, option, CIl2Cpp::il2cpp_string_new(cmd), args);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}

		static CSOption* client()
		{
			
			auto option = new CSOption();

			return new CSOption
			{
				option->IsClient = true,
				option->ForwardtoServerOnMissing = true,
				option->PrintOutput = true
			};
		}
	};

	struct Effect : Il2CppObject {
		IL2CPP_CLASS("Effect");

		IL2CPP_FIELD(Vector3, worldPos);
		IL2CPP_FIELD(System::String*, pooledString);
		IL2CPP_STATIC_FIELD(Effect*, reusableInstace);

		void _cctor()
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH(".ctor"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}

		}

		/*
			public Vector3 Up; // 0x58
	public Vector3 worldPos; // 0x64
	public Vector3 worldNrm; // 0x70
	public bool attached; // 0x7C
	public Transform transform; // 0x80
	public GameObject gameObject; // 0x88
	public string pooledString; // 0x90
	public bool broadcast; // 0x98
	private static Effect reusableInstace; // 0x0
		*/
	};

	struct EffectNetwork : Il2CppObject {
		IL2CPP_CLASS("EffectNetwork");

		IL2CPP_STATIC_FIELD(Effect*, effect);
	};

	struct SoundOcclusion : Il2CppObject {
		IL2CPP_CLASS("SoundOcclusion");

		IL2CPP_FIELD(float, occlusionAmount);
		IL2CPP_FIELD(Vector3, soundOffset);
		IL2CPP_FIELD(float, lastOcclusionCheck);
		IL2CPP_FIELD(float, occlusionCheckInterval);
		IL2CPP_FIELD(System::List<UnityEngine::RaycastHit*>*, hits);
		IL2CPP_FIELD(UnityEngine::Ray, ray);


		/*
		private bool <isOccluded>k__BackingField; // 0x1C
	private float occlusionAmount; // 0x20
	private Sound sound; // 0x28
	private OnePoleLowpassFilter lowpass; // 0x30
	private SoundModulation.Modulator gainMod; // 0x38
	private Vector3 soundOffset; // 0x40
	private float lastOcclusionCheck; // 0x4C
	private float occlusionCheckInterval; // 0x50
	private Ray ray; // 0x54
	private List<RaycastHit> hits; // 0x70
		*/
	};


	struct AntiHack : Il2CppObject {
		IL2CPP_CLASS("AntiHack");


		static bool TestNoClipping(Vector3 oldPos, Vector3 newPos, float radius, float backtracking, bool sphereCast, uintptr_t* collider, bool vehicleLayer, BaseEntity* ignoreEntity)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("TestNoClipping"), 8);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool*>(procedure,oldPos, newPos, radius, backtracking, sphereCast, collider, vehicleLayer, ignoreEntity);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return false;
			}
		}
	};
}


enum MessageType // TypeDefIndex: 13298
{
	// Fields
	First = 0,
	Welcome = 1,
	Auth = 2,
	Approved = 3,
	Ready = 4,
	Entities = 5,
	EntityDestroy = 6,
	GroupChange = 7,
	GroupDestroy = 8,
	RPCMessage = 9,
	EntityPosition = 10,
	ConsoleMessage = 11,
	ConsoleCommand = 12,
	Effect = 13,
	DisconnectReason = 14,
	Tick = 15,
	Message = 16,
	RequestUserInformation = 17,
	GiveUserInformation = 18,
	GroupEnter = 19,
	GroupLeave = 20,
	VoiceData = 21,
	EAC = 22,
	EntityFlags = 23,
	World = 24,
	ConsoleReplicatedVars = 25,
	Last = 25
};

namespace Network {
	IL2CPP_NAME_SPACE(SECURE("Network"));

	struct NetRead : Il2CppObject {
		IL2CPP_CLASS("NetRead");

	};
	struct BaseNetwork;

	struct NetWrite : Il2CppObject {
		IL2CPP_CLASS("NetWrite");


		bool Start(BaseNetwork* net)
		{
			if (!this)
				return false;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Start"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, this, net);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return false;
			}
		}

		void String(const char* val)
		{
			if (!this)
				return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("String"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, CIl2Cpp::il2cpp_string_new(val));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		void UInt32(unsigned int val)
		{
			if (!this)
				return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("UInt32"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, val);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		void UInt64(unsigned long val)
		{
			if (!this)
				return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("UInt64"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, val);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		void UInt8(byte val)
		{
			if (!this)
				return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("UInt8"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, val);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		void PacketID(MessageType type)
		{
			if (!this)
				return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("PacketID"),1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, type);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}
	};

	struct BaseNetwork : Il2CppObject {
		IL2CPP_CLASS("BaseNetwork");


		IL2CPP_FIELD(NetWrite*, writeQueue);
		NetWrite* StartWrite()
		{
			if (!this)
				return {};

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("StartWrite"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<NetWrite*>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}
	};

	struct Message : Il2CppObject{
		IL2CPP_CLASS("Message");

		IL2CPP_FIELD(BaseNetwork*, peer);
		IL2CPP_FIELD(NetRead*, read);
		IL2CPP_FIELD(MessageType, type);

	};

	struct Connection : Il2CppObject
	{
		IL2CPP_CLASS("Connection");

		int GetPacketsPerSecond(int test)
		{
			if (!this) return 0;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethodFullArgs(HASH("Facepunch.Network::Network::Connection::GetPacketsPerSecond(Int32): UInt64"));
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<int>(procedure, this, test);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return 0;
			}

		}

		void _cctor()
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH(".ctor"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}

		}
	};

	namespace Raknet {
		IL2CPP_NAME_SPACE(SECURE("Facepunch.Network.Raknet"));

		class Client
		{
		public:
			IL2CPP_CLASS("Client");


			auto IsConnected() -> bool {
				if (!this)
					return false;

				const auto LOS = reinterpret_cast<bool (*)(Client*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("IsConnected"), 0)));
				return LOS(this);
			}
		};
	}

	class Client
	{
	public:
		IL2CPP_CLASS("Client");


		auto get_ServerName() -> System::String* {
			const auto LOS = reinterpret_cast<System::String * (*)(Client*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("get_ServerName"), 0)));
			return LOS(this);
		}
	};
	class Networkable : Il2CppObject
	{
	public:
		IL2CPP_CLASS("Networkable");

		IL2CPP_FIELD(Network::Client*, cl);

	};

	class Net
	{
	public:
		IL2CPP_CLASS("Net");

		IL2CPP_STATIC_PROPERTY(Raknet::Client*, cl);

		//STATIC_FUNCTION("Network::Net::get_cl(): Network.Client", get_cl, Facepunch::Network::Raknet::CClient* ());
	};
}

typedef struct _UncStr {
	char stub[0x10];
	int len;
	wchar_t str[1];
} *pUncStr;

class f_object {
public:
	bool valid;
	DWORD64 entity;
	Vector3 position;
	float dist = 10000.f;
	bool visible = false;

	bool operator<(const f_object& b) { return this->dist < b.dist; }
	bool operator>(const f_object& b) { return this->dist > b.dist; }
	bool operator<=(const f_object& b) { return this->dist <= b.dist; }
	bool operator>=(const f_object& b) { return this->dist >= b.dist; }
	f_object() {
		this->valid = false;
	}
	f_object(Vector3 target) {
		this->valid = false;
		this->position = target;
	}
	static Vector3 GetEntityPosition(std::uint64_t entity) {
		if (!entity) return Vector3();

		uintptr_t plyVis = memory::Read<uintptr_t>(entity + 0x8);
		if (!plyVis) return Vector3();

		uintptr_t visualState = memory::Read<uintptr_t>(plyVis + 0x38);
		if (!visualState) return Vector3();

		Vector3 ret = memory::Read<Vector3>(visualState + 0x90);
		return ret;
	}

	static f_object get_closest_object(Vector3 from, const char* namee, Vector3 ignore = Vector3(), Vector3 ignore2 = Vector3(), Vector3 ignore3 = Vector3(), bool classname = false, const char* classnamee = SECURE(""), float get_dist = 99999.f, bool vis = false, bool y = false) {
		f_object lowest = f_object();

		auto entityList = AssemblyCSharp::BaseNetworkable::clientEntities()->entityList();
		if (entityList) {
			for (int i = 1; i < entityList->vals->count; i++) {
				uintptr_t Entity = *reinterpret_cast<uintptr_t*>(std::uint64_t(entityList->vals->buffer) + (0x20 + (sizeof(void*) * i)));
				if (!Entity) continue;
				uintptr_t Object = *reinterpret_cast<uint64_t*>(Entity + 0x10);
				if (!Object) continue;
				uintptr_t ObjectClass = *reinterpret_cast<uint64_t*>(Object + 0x30);
				if (!ObjectClass) continue;
				pUncStr name = memory::Read<pUncStr>(ObjectClass + 0x60); if (!name) continue;
				char* buff = name->stub;
				f_object res = f_object();
				if (classname) {
					if (win32::strstr((char*)memory::Read<DWORD64>(memory::Read<DWORD64>(memory::Read<DWORD64>(Object + 0x28)) + 0x10), classnamee)) {
						uintptr_t a = memory::Read<UINT64>(ObjectClass + 0x30);
						float dist = GetEntityPosition(a).get_3d_dist(from);
						if (GetEntityPosition(a) != ignore && GetEntityPosition(a) != ignore2 && GetEntityPosition(a) != ignore3) {
							res.valid = dist <= get_dist;
							res.dist = dist;
							res.entity = Entity;
							res.position = GetEntityPosition(a);
							if (res < lowest) lowest = res;
						}
					}
				}
				else {
					if (win32::strstr(buff, namee)) {
						uintptr_t a = memory::Read<uintptr_t>(ObjectClass + 0x30);
						float dist = GetEntityPosition(a).get_3d_dist(from);
						if (GetEntityPosition(a) != ignore && GetEntityPosition(a) != ignore2 && GetEntityPosition(a) != ignore3) {
							if (y) {
								if (GetEntityPosition(a).y > 0) {
									res.valid = dist <= get_dist;
									res.dist = dist;
									res.entity = Entity;
									res.position = GetEntityPosition(a);
									if (res < lowest) lowest = res;
								}
							}
							else {
								res.valid = dist <= get_dist;
								res.dist = dist;
								res.entity = Entity;
								res.position = GetEntityPosition(a);
								if (res < lowest) lowest = res;
							}
						}
					}
				}
			}
			return lowest;
		}
	}
};

inline bool ProjectileHitWater = false;
inline AssemblyCSharp::Projectile* ProjectileHitWaterInstance;
inline AssemblyCSharp::Projectile* FatBulletInstance;

struct TimeAverageValueData
{
public:
	int Calculate()
	{
		float realtimeSinceStartup = UnityEngine::Time::get_realtimeSinceStartup();
		float num = realtimeSinceStartup - refreshTime;
		if (num >= 1.0)
		{
			counterPrev = (int)(counterNext / num + 0.5);
			counterNext = 0;
			refreshTime = realtimeSinceStartup;
			num = 0;
		}
		return (int)(counterPrev * (1.0 - num)) + counterNext;
	}

	void Increment()
	{
		this->Calculate();
		counterNext += 1;
	}

	void Reset()
	{
		counterPrev = 0;
		counterNext = 0;
	}

	float refreshTime;

	int counterPrev;

	int counterNext;
};


inline TimeAverageValueData Total_Counter = { 0, 0, 0 };
inline TimeAverageValueData RPC_Counter = { 0, 0, 0 };
inline TimeAverageValueData RPC_Counter2 = { 0, 0, 0 };
inline TimeAverageValueData RPC_Counter3 = { 0, 0, 0 };
inline TimeAverageValueData RPC_Counter5 = { 0, 0, 0 };

inline TimeAverageValueData Signal_Counter = { 0, 0, 0 };

namespace Steamworks {
	IL2CPP_NAME_SPACE(SECURE("Steamworks"));


	struct SteamId_c {
		uint64_t Value; // 0x0
	};

	struct SteamClient : Il2CppObject {
		IL2CPP_CLASS("SteamClient");

		IL2CPP_STATIC_PROPERTY(SteamId_c, SteamId);

	};
}

namespace RustPlatformSteam {
	IL2CPP_NAME_SPACE(SECURE("Rust.Platform.Steam"));


	struct SteamPlatform : Il2CppObject {
		IL2CPP_CLASS("SteamPlatform");


		void set_UserName(const char* str)
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_UserName"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, CIl2Cpp::il2cpp_string_new(str));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}

		}

		void _cctor()
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH(".ctor"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}

		}
	};
}

namespace Rust_Workshop
{
	IL2CPP_NAME_SPACE(SECURE("Rust.Workshop"));

	struct WorkshopSkin : Il2CppObject {
		IL2CPP_CLASS("WorkshopSkin");

		static void Apply(UnityEngine::GameObject* obj, uint64_t skinId, uintptr_t* ptr)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Apply"), 3);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, obj, skinId, ptr);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}
	};
}

