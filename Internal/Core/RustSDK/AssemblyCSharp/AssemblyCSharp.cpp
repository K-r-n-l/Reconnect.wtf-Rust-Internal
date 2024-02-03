#include "AssemblyCSharp.h"
#include "../../Settings/Settings.hpp"
namespace AssemblyCSharp {

	bool BaseCombatEntity::IsDead()
	{
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsDead"), 0);
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

	bool BaseCombatEntity::IsAlive()
	{
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsAlive"), 0);
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

	Vector3 HitTest::HitPointWorld() {
		if (!this) return Vector3();
		const auto do_hit = reinterpret_cast<Vector3(*)(HitTest*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("HitPointWorld"), 0)));
		return do_hit(this);
	}

	Vector3 HitTest::HitNormalWorld() {
		if (!this) return Vector3();
		const auto do_hit = reinterpret_cast<Vector3(*)(HitTest*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("HitNormalWorld"), 0)));
		return do_hit(this);
	}

	Object* GameManifest::GUIDToObject(System::String* guid) {
		const auto rand_velo = reinterpret_cast<Object * (*)(System::String*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(CIl2Cpp::FindClass(SECURE(""), SECURE("GameManifest")), HASH("GUIDToObject"), 1)));
		return rand_velo(guid);
	}
	float ItemModProjectile::GetRandomVelocity() {
		const auto rand_velo = reinterpret_cast<float (*)(ItemModProjectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("GetRandomVelocity"), 0)));
		return rand_velo(this);
	}

	bool Projectile::IsAlive() {
		return (this->integrity() > 0.001f && this->traveledDistance() < this->maxDistance());
	}


	void Projectile::UpdateVelocity(float deltaTime)
	{
		const auto do_hit = reinterpret_cast<void (*)(Projectile*, float)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("UpdateVelocity"), 1)));
		return do_hit(this, deltaTime);
	}

	bool Projectile::DoRicochet(HitTest* test, Vector3 point, Vector3 normal)
	{
		const auto do_hit = reinterpret_cast<bool (*)(Projectile*, HitTest*, Vector3, Vector3)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("DoRicochet"), 3)));
		return do_hit(this, test, point, normal);
	}


	bool Projectile::Reflect(int seed, Vector3 point, Vector3 normal)
	{
		const auto do_hit = reinterpret_cast<bool (*)(Projectile*, int,Vector3,Vector3)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("Reflect"), 3)));
		return do_hit(this, seed,point,normal);
	}

	bool Projectile::IsWaterMaterial(Il2CppString* name)
	{

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsWaterMaterial"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool*>(procedure, name);
		}
		else
		{
			TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
			return {};
		}
	}

	bool Projectile::DoHit(HitTest* test, Vector3 point, Vector3 normal)
	{
		const auto do_hit = reinterpret_cast<bool (*)(Projectile*, HitTest*, Vector3, Vector3)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("DoHit"), 3)));
		return do_hit(this, test, point, normal);
	}

	auto Projectile::Retire() -> void
	{
		if (!this) return;

		const auto is_dead = reinterpret_cast<void (*)(Projectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("Retire"), 0)));
		return is_dead(this);
	}

	auto Projectile::get_isAlive() -> bool
	{
		if (!this) return false;

		const auto is_dead = reinterpret_cast<bool (*)(Projectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("get_isAlive"), 0)));
		return is_dead(this);
	}

	auto Projectile::isAuthoritative() -> bool
	{
		if (!this) return false;

		const auto is_dead = reinterpret_cast<bool (*)(Projectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("get_isAuthoritative"), 0)));
		return is_dead(this);
	}

	System::String* ItemDefinition::GetDisplayName(Item* item)
	{
		if (!this) return {};

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetDisplayName"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<System::String*>(procedure, this, item);
		}
		else
		{
			TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
			return {};
		}
	}

	UnityEngine::Sprite* ItemDefinition::FindIconSprite(int id) {
		if (!this) return {};

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("FindIconSprite"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<UnityEngine::Sprite*>(procedure, this, id);
		}
		else
		{
			TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
			return {};
		}
	}

	BaseEntity* Item::GetHeldEntity() {
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
			return memory::Call<BaseEntity*>(procedure, this);
		}

		TRACE("%s: procedure not a valid!", FN);
		return {};
	}

	System::String* Item::GetItemName()
	{
		const auto item_definition = this->info();
		if (!memory::IsAddressValid(item_definition))
		{
			return { };
		}

		const auto display_phrase = item_definition->displayName();
		if (!memory::IsAddressValid(display_phrase))
		{
			return { };
		}

		const auto english = display_phrase->english();
		if (!memory::IsAddressValid(english))
		{
			return { };
		}

		return english;
	}

	System::String* Item::GetItemShortName()
	{
		const auto definition = info();
		if (memory::IsAddressValid(definition))
		{
			const auto short_name = definition->shortname();
			if (memory::IsAddressValid(short_name))
			{
				return short_name;
			}
		}

		return { };
	}

	void BaseEntity::SetSkin(uint64_t skin_id)
	{
		skinID() = skin_id;
	}

	void Item::SetSkin(uint64_t skin_id)
	{
		skin() = skin_id;
	}

	Item* ItemContainer::GetItem(int id)
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return nullptr;

		auto item_list = this->itemList();
		if (!(item_list))
			return nullptr;

		auto itmes = item_list->_items;

		uintptr_t items = *reinterpret_cast<uintptr_t*>(item_list + 0x10);
		if (!(items))
			return nullptr;


		return *reinterpret_cast<Item**>(items + 0x20 + (id * 0x8));
	}

	Item* PlayerInventory::FindItemUID(int id) {
		if (!this) return {};
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("FindItemByUID"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<Item*>(procedure, this, id);
		}

		TRACE("%s: procedure not a valid!", FN);
		return {};
	}

	ItemContainer* PlayerInventory::get_belt()
	{
		uintptr_t entity = reinterpret_cast<uintptr_t>(this);

		if (!(entity))
			return nullptr;

		auto address = this->containerBelt(); //il2cpp::value(xorstr_("PlayerInventory"), xorstr_("containerBelt"));
		if (!address)
			return nullptr;

		return address;
	}

	std::uint32_t BasePlayer::GetHeldItemID() {
		if (!this) return 0;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetHeldItemID"), 0);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<std::uint32_t>(procedure, this);
		}

		TRACE("%s: procedure not a valid!", FN);
		return 0;
	}

	auto BasePlayer::ActiveItem() -> Item*
	{
		auto item_id = GetHeldItemID();
		if (memory::IsAddressValid(item_id))
		{
			if (memory::IsAddressValid(item_id) && memory::IsAddressValid(inventory()))
			{
				return inventory()->FindItemUID(item_id);
			}
		}

		return nullptr;
	}

	Item* BasePlayer::GetHeldItemSafe()
	{
		if (!this) return {};
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetHeldItem"), 0);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<Item*>(procedure, this);
		}

		TRACE("%s: procedure not a valid!", FN);
		return {};
	}

	void Door::KnockDoor(AssemblyCSharp::BasePlayer* player)
	{
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Menu_KnockDoor"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, player);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	bool BasePlayer::IsLocalPlayer()
	{
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsLocalPlayer"), 0);
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

	bool BasePlayer::HasFlag(uint16_t flag)
	{
		if (!this) return false;

		return (playerFlags() & flag) == flag;
	}

	bool BasePlayer::IsReceivingSnapshot()
	{
		return HasFlag(8);
	}

	bool BasePlayer::IsConnected()
	{
		return HasFlag(256);
	}

	bool BasePlayer::IsSleeper()
	{
		return HasFlag(16);
	}

	bool BasePlayer::IsAdmin()
	{
		return HasFlag(4);
	}

	bool BasePlayer::IsSleeping()
	{
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsSleeping"), 0);
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

	UnityEngine::Transform* BasePlayer::get_bone_transform(int bone_id) {
		const auto model = this->model();
		if (memory::IsAddressValid(model))
		{
			const auto bone_transforms = model->boneTransforms();
			if (memory::IsAddressValid(bone_transforms))
			{
				const auto bone_transform = bone_transforms->m_Items[bone_id];
				if (memory::IsAddressValid(bone_transform))
				{
					return bone_transform;
				}
			}
		}
	}

	auto GamePhysics::LineOfSightRadius(Vector3 p0, Vector3 p1, int layerMask, float radius, float padding, BaseEntity* ignoreEntity) -> bool
	{
		const auto LOS = reinterpret_cast<bool (*)(Vector3 p0, Vector3 p1, int layerMask, float radius, float padding, BaseEntity * ignoreEntity)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("LineOfSightRadius"), 6)));
		return LOS(p0, p1, layerMask, radius, padding, ignoreEntity);
	}

	auto GamePhysics::LineOfSightRadius_3(Vector3 p0, Vector3 p1, int layerMask, float padding, BaseEntity* ignoreEntity) -> bool
	{
		const auto LOS = reinterpret_cast<bool (*)(Vector3 p0, Vector3 p1, int layerMask, float padding, BaseEntity * ignoreEntity)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("LineOfSightRadius"), 5)));
		return LOS(p0, p1, layerMask, padding, ignoreEntity);
	}

	auto GamePhysics::LineOfSight(Vector3 p0, Vector3 p1, int layerMask, BaseEntity* ignoreEntity) -> bool
	{
		const auto LOS = reinterpret_cast<bool (*)(Vector3 p0, Vector3 p1, int layerMask, BaseEntity * ignoreEntity)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("LineOfSight"), 4)));
		return LOS(p0, p1, layerMask, ignoreEntity);
	}

	System::String* BasePlayer::get_displayName() {
		if (!this) return {};
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("get_displayName"), 0);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<System::String*>(procedure, this);
		}

		TRACE("%s: procedure not a valid!", FN);
		return {};
	}

	std::string* BasePlayer::get_displayNameTest() {
		if (!this) return {};
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("get_displayName"), 0);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<std::string*>(procedure, this);
		}

		TRACE("%s: procedure not a valid!", FN);
		return {};
	}

	bool PlayerModel::isNpc() {
		if (!this) return false;

		static std::size_t offset = 0;
		const auto field = CIl2Cpp::FindField(this->StaticClass(), HASH("<IsNpc>k__BackingField"));
		if (memory::IsAddressValid(field))
		{
			offset = CIl2Cpp::il2cpp_field_get_offset(field);
		}

		return *reinterpret_cast<bool*>(memory::ToAddress(this) + offset);
	}

	void BuildingBlock::UpgradeToGrade(RustStructs::BuildingGrade grade, int xd, BasePlayer* player) {
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("UpgradeToGrade"), 3);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, grade, xd, player);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	bool BuildingBlock::CanChangeToGrade(RustStructs::BuildingGrade grade, int xd, BasePlayer* player) {
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("CanChangeToGrade"), 3);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool>(procedure, this, grade, xd, player);
		}

		TRACE("%s: procedure not a valid!", FN);
		return false;
	}

	bool BuildingBlock::CanAffordUpgrade(RustStructs::BuildingGrade grade, int xd, BasePlayer* player) {
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("CanAffordUpgrade"), 3);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool>(procedure, this, grade, xd, player);
		}

		TRACE("%s: procedure not a valid!", FN);
		return false;
	}

	bool BuildingBlock::IsUpgradeBlocked() {
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsUpgradeBlocked"), 0);
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

	Vector3 BaseEntity::ClosestPoint(Vector3 position)
	{
		if (!this) return Vector3(0.f, 0.f, 0.f);
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("ClosestPoint"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<Vector3>(procedure, this, position);
		}

		TRACE("%s: procedure not a valid!", FN);
		return Vector3(0.f, 0.f, 0.f);
	}

	void BaseEntity::OnSignal(RustStructs::Signal a, char* str = SECURE("")) {
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("OnSignal"), 2);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, a, CIl2Cpp::il2cpp_string_new(str));
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	void BaseEntity::SendSignalBroadcast(RustStructs::Signal a, char* str = SECURE("")) {
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SendSignalBroadcast"), 2);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, a, CIl2Cpp::il2cpp_string_new(str));
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	void BaseEntity::ServerRPC(const char* msg) {
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("ServerRPC"), 1);
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

	void PlayerEyes::set_position(Vector3 pos) {
		if (!this) return;

		auto get_pos = reinterpret_cast<void(*)(PlayerEyes*, Vector3)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), constant::Hash(SECURE("set_position"), true), 1)));
		return get_pos(this, pos);
	}

	Vector3 PlayerEyes::MovementRight() {
		if (!this) return Vector3(0.f, 0.f, 0.f);

		auto get_pos = reinterpret_cast<Vector3(*)(PlayerEyes*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), constant::Hash(SECURE("MovementRight"), true), 0)));
		return get_pos(this);
	}

	Vector3 PlayerEyes::MovementForward() {
		if (!this) return Vector3(0.f, 0.f, 0.f);

		auto get_pos = reinterpret_cast<Vector3(*)(PlayerEyes*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), constant::Hash(SECURE("MovementForward"), true), 0)));
		return get_pos(this);
	}

	Vector3 PlayerEyes::BodyForward() {
		if (!this) return Vector3(0.f, 0.f, 0.f);

		auto get_pos = reinterpret_cast<Vector3(*)(PlayerEyes*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), constant::Hash(SECURE("BodyForward"), true), 0)));
		return get_pos(this);
	}


	Vector3 PlayerEyes::BodyRight() {
		if (!this) return Vector3(0.f, 0.f, 0.f);

		auto get_pos = reinterpret_cast<Vector3(*)(PlayerEyes*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), constant::Hash(SECURE("BodyRight"), true), 0)));
		return get_pos(this);
	}

	void PlayerEyes::SetBodyRotation(Vector4 rot)
	{
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_bodyRotation"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, rot);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	Vector3 BaseMelee::GetInheritedVelocity(AssemblyCSharp::BasePlayer* player, Vector3 dir)
	{
		if (!this) return Vector3();
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetInheritedVelocity"), 2);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<Vector3>(procedure, this, player,dir);
		}

		TRACE("%s: procedure not a valid!", FN);
		return Vector3();
	}

	Vector4 PlayerEyes::bodyRotation() {
		auto body_rotation = memory::Read<Vector4>(this + 0x44);

		if (body_rotation.empty())
			return Vector4(0.f, 0.f, 0.f, 0.f);

		return body_rotation;
	}

	auto BasePlayer::BoundsPadding() -> float {
		const auto get_is_ducked = reinterpret_cast<float (*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("BoundsPadding"), 0)));
		return get_is_ducked(this);
	}

	auto BasePlayer::GetJumpHeight() -> float {
		if (!this) return 0.f;
		const auto is_dead = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("GetJumpHeight"), 0)));
		return is_dead(this);
	}


	float BaseProjectile::GetProjectileVelocityScale(bool max) {
		const auto rand_velo = reinterpret_cast<float (*)(BaseProjectile*, bool)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("GetProjectileVelocityScale"), 1)));
		return rand_velo(this, max);
	}


	auto BaseNetworkable::PlayerDestroyed() -> bool
	{
		if (!this) return false;
		return *reinterpret_cast<bool*>(this + 0x38);
	}

	bool BaseNetworkable::PlayerValid() {
		if (!this) return false;
		return !this->PlayerDestroyed() && this->net() != nullptr;
	}

	auto BaseEntity::GetWorldVelocity() -> Vector3 {
		if (!this) return Vector3(0.f, 0.f, 0.f);

		const auto is_dead = reinterpret_cast<Vector3(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("GetWorldVelocity"), 0)));
		return is_dead(this);
	}

	auto BaseEntity::GetParentVelocity() -> Vector3 {

		if (!this) return Vector3(0.f, 0.f, 0.f);

		const auto is_dead = reinterpret_cast<Vector3(*)(BaseEntity*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("GetParentVelocity"), 0)));
		return is_dead(this);
	}

	int BasePlayer::ClosesestToCrosshair(uintptr_t player) {
		int bestBone = 0;
		float closest = FLT_MAX;

		auto base_p = reinterpret_cast<AssemblyCSharp::BasePlayer*>(player);

		float screen_width = UnityEngine::Screen::get_width();
		float screen_height = UnityEngine::Screen::get_height();

		for (auto bone : { (int)RustStructs::bones::head,(int)RustStructs::bones::neck, (int)RustStructs::bones::spine4, (int)RustStructs::bones::spine3,(int)RustStructs::bones::spine2, (int)RustStructs::bones::spine1_scale, 14, 2, 55, 24, 13, 1, 56, 25, 76, 45, 16, 4, (int)RustStructs::bones::r_ankle_scale, (int)RustStructs::bones::l_ankle_scale,(int)RustStructs::bones::r_hand,(int)RustStructs::bones::l_hand }) {
			Vector3 pos3d = base_p->get_bone_transform(bone)->get_position();
			Vector2 pos;
			if (UnityEngine::WorldToScreen(pos3d, pos) == false) continue;
			float length = win32::sqrt(win32::pow((screen_width / 2) - pos.x, 2) + win32::pow((screen_height / 2) - pos.y, 2));
			if (length < closest) {
				closest = length;

				bestBone = bone;
			}
		}
		return bestBone;
	}

	BasePlayer::Target BasePlayer::GetTarget(Vector3 Source, float MaxDist = 1000) {
		__try {
			BasePlayer::Target best_target = BasePlayer::Target();

			auto client_entities = BaseNetworkable::clientEntities();
			if (!memory::IsAddressValid(client_entities))
				return {};

			auto entities = client_entities->entityList();
			if (!memory::IsAddressValid(entities))
				return {};

			auto m_entities = entities->vals;

			if (!memory::IsAddressValid(m_entities))
				return {};

			auto m_entity_array = m_entities->buffer;

			if (!memory::IsAddressValid(m_entity_array))
				return {};

			auto m_camera = UnityEngine::Camera::get_main();
			if (!memory::IsAddressValid(m_camera))
				return {};

			for (std::int32_t index = 0; index < m_entities->count; index++)
			{
				auto base_networkable = m_entity_array->m_Items[index];

				if (!memory::IsAddressValid(base_networkable))
					continue;

				if (m_options.m_options.HeliAimbot && base_networkable->IsA(BaseHelicopter::StaticClass()))
				{
					auto base_player = static_cast<BasePlayer*>(base_networkable);
					if (base_player)
					{
						BasePlayer::Target target;
						target.m_player = base_player;
						target.m_heli = true;

						auto velocity = base_player->GetWorldVelocity();
						target.m_velocity = velocity;

						auto pos = base_player->get_bone_transform(19)->get_position();
						target.m_position = pos;

						auto distance = Source.get_3d_dist(pos);
						target.m_distance = distance;
						if (distance > MaxDist)
							continue;

						auto fov = get_fov(pos);
						target.m_fov = fov;

						if (target < best_target)
							best_target = target;
					}
				}
				else if (base_networkable->IsA(BasePlayer::StaticClass())) {
					auto base_player = static_cast<BasePlayer*>(base_networkable);
					if (!memory::IsAddressValid(base_player))
						continue;

					if (!memory::IsAddressValid(base_player->playerModel()) || !memory::IsAddressValid(base_player->model()))
						continue;

					if (base_player->IsDead() || base_player->PlayerDestroyed() || base_player->IsLocalPlayer() ||
						base_player->playerFlags() & RustStructs::PlayerFlags::Sleeping || base_player->wasDead() ||
						base_player->_health() <= 0.f)
						continue;

					auto isWounded = base_player->playerFlags() & RustStructs::PlayerFlags::Wounded;
					if (isWounded && !m_aimbot.m_aimbot.TargetWounded)
						continue;

					bool isNpc = false;
					if (base_player->playerModel()->get_IsNpc()) {
						if (!m_aimbot.m_aimbot.AimbotNPC)
							continue;
						isNpc = true;
					}

					BasePlayer::Target target;
					target.m_player = base_player;

					if (target.m_player != nullptr)
					{
						if (target.m_player != base_player)
						{
							target.m_player = base_player;
						}
					}

					auto velocity = base_player->playerModel()->newVelocity();
					target.m_velocity = velocity;

					Vector3 pos = Vector3(0.f, 0.f, 0.f);
					int selectedAimbone = m_aimbot.m_aimbot.SelectedAimbone;
					int bone = 47;
					switch (selectedAimbone) {
					case 0: // Head
						pos = base_player->get_bone_transform(RustStructs::bones::head)->get_position();
						bone = RustStructs::bones::head;
						break;
					case 1: // neck
						pos = base_player->get_bone_transform(RustStructs::bones::neck)->get_position();
						bone = RustStructs::bones::neck;
						break;
					case 2: // spine1
						pos = base_player->get_bone_transform(RustStructs::bones::spine1)->get_position();
						bone = RustStructs::bones::spine1;
						break;
					case 3: // Random bone selection
						switch (my_rand() % 9) {
						case 0: // Head
							pos = base_player->get_bone_transform(RustStructs::bones::head)->get_position();
							bone = RustStructs::bones::head;
							break;
						case 1: // pelvis
							pos = base_player->get_bone_transform(RustStructs::bones::pelvis)->get_position();
							bone = RustStructs::bones::pelvis;
							break;
						case 2: // r_hip
							pos = base_player->get_bone_transform(RustStructs::bones::r_hip)->get_position();
							bone = RustStructs::bones::r_hip;
							break;
						case 3: // r_foot
							pos = base_player->get_bone_transform(RustStructs::bones::r_foot)->get_position();
							bone = RustStructs::bones::r_foot;
							break;
						case 4: // spine1
							pos = base_player->get_bone_transform(RustStructs::bones::spine1)->get_position();
							bone = RustStructs::bones::spine1;
							break;
						case 5: // l_hand
							pos = base_player->get_bone_transform(RustStructs::bones::l_hand)->get_position();
							bone = RustStructs::bones::l_hand;
							break;
						case 6: // r_upperarm
							pos = base_player->get_bone_transform(RustStructs::bones::r_upperarm)->get_position();
							bone = RustStructs::bones::r_upperarm;
							break;
						case 7: // l_knee
							pos = base_player->get_bone_transform(RustStructs::bones::l_knee)->get_position();
							bone = RustStructs::bones::l_knee;
							break;
						case 8: // spine4
							pos = base_player->get_bone_transform(RustStructs::bones::spine4)->get_position();
							bone = RustStructs::bones::spine4;
							break;
						}
						break;
					case 4:
						bone = ClosesestToCrosshair(memory::ToAddress(base_player));
						pos = base_player->get_bone_transform(bone)->get_position();
						break;
					}

					target.m_position = pos;
					target.m_bone = bone;
					target.m_npc = isNpc;
					target.m_heli = false;

					auto distance = Source.get_3d_dist(pos);
					target.m_distance = distance;
					if (distance > MaxDist)
						continue;

					auto fov = get_fov(pos);
					target.m_fov = fov;

					if (target < best_target)
						best_target = target;
				}
			}
			return best_target;
		}
		__except (true)
		{
			TRACE(SECURE("\n exception - getaimtargetr"));
		}
	}

	auto BasePlayer::IsWeapon() -> bool {
		if (const auto item = ActiveItem(); memory::IsAddressValid(item))
		{
			const bool bIsWeapon = !item->IsA(AssemblyCSharp::BaseMelee::StaticClass()) &&
				!item->IsA(AssemblyCSharp::ThrownWeapon::StaticClass()) &&
				!item->IsA(AssemblyCSharp::TorchWeapon::StaticClass());


			if (!bIsWeapon)
				return false;

			const auto held_entity = item->GetHeldEntity();
			if (bIsWeapon && memory::IsAddressValid(held_entity) && (
				held_entity->IsA(AssemblyCSharp::BaseProjectile::StaticClass()) ||
				held_entity->IsA(AssemblyCSharp::BowWeapon::StaticClass()) ||
				held_entity->IsA(AssemblyCSharp::CompoundBowWeapon::StaticClass()) ||
				held_entity->IsA(AssemblyCSharp::CrossbowWeapon::StaticClass()) ||
				held_entity->IsA(AssemblyCSharp::FlintStrikeWeapon::StaticClass()) ||
				held_entity->IsA(AssemblyCSharp::BaseLauncher::StaticClass())))
				return true;

		}
	}

	auto BasePlayer::IsMelee() -> bool {

		if (const auto item = ActiveItem(); memory::IsAddressValid(item))
		{
			const auto held_entity = item->GetHeldEntity();

			if (memory::IsAddressValid(held_entity) && !held_entity->IsA(AssemblyCSharp::FlameThrower::StaticClass()) && !held_entity->IsA(AssemblyCSharp::TorchWeapon::StaticClass()) && (
				held_entity->IsA(AssemblyCSharp::ThrownWeapon::StaticClass()) ||
				held_entity->IsA(AssemblyCSharp::BaseMelee::StaticClass())))
				return true;
		}

		return false;
	}

	void BaseMelee::ProcessAttack(HitTest* hit) {
		if (!this) return;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("ProcessAttack"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, hit);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	void BaseMelee::DoThrow() {
		if (!this) return;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("DoThrow"), 0);
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

	bool AttackEntity::IsFullyDeployed()
	{
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsFullyDeployed"), 0);
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

	ItemDefinition* AttackEntity::GetOwnerItemDefinition()
	{
		if (!this) return {};
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetOwnerItemDefinition"), 0);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<ItemDefinition*>(procedure, this);
		}

		TRACE("%s: procedure not a valid!", FN);
		return {};
	}

	bool AttackEntity::HasAttackCooldown() {
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("HasAttackCooldown"), 0);
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

	auto AttackEntity::StartAttackCooldown(float coolDown) -> void
	{
		if (!this) return;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("StartAttackCooldown"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, coolDown);
		}
		else
		{
			TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
			return;
		}
	}

	auto ModelState::SetMounted(bool value) -> void {
		const auto set_mounted = reinterpret_cast<void (*)(ModelState*, bool)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("set_mounted"), 1)));
		return set_mounted(this, value);
	}

	auto ModelState::get_ducked() -> bool {
		const auto get_is_ducked = reinterpret_cast<bool (*)(ModelState*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("get_ducked"), 0)));
		return get_is_ducked(this);
	}

	auto ModelState::SetOnLadder(bool value) -> void {
		const auto set_on_Ladder = reinterpret_cast<void (*)(ModelState*, bool)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("set_onLadder"), 1)));
		return set_on_Ladder(this, value);
	}

	void ModelState::SetSprinting(bool Value)
	{
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_sprinting"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, Value);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	void ModelState::set_flag(RustStructs::ModelState_Flag flag) {
		int flags = this->flags();

		this->flags() = flags |= (int)flag;
	}

	void ModelState::remove_flag(RustStructs::ModelState_Flag flag) {
		int flags = this->flags();
		flags &= ~(int)flag;

		this->flags() = flags;
	}

	bool InputState::IsDown(RustStructs::BUTTON btn)
	{
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsDown"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool>(procedure, this, btn);
		}

		TRACE("%s: procedure not a valid!", FN);
		return false;
	}

	bool InputState::WasJustPressed(RustStructs::BUTTON btn)
	{
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("WasJustPressed"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool>(procedure, this, btn);
		}

		TRACE("%s: procedure not a valid!", FN);
		return false;
	}

	Vector3 AimConeUtil::GetModifiedAimConeDirection(float aimCone, Vector3 inputVec, bool anywhereInside = true)
	{
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetModifiedAimConeDirection"), 3);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<Vector3>(procedure, aimCone, inputVec, anywhereInside);
		}

		TRACE("%s: procedure not a valid!", FN);
		return Vector3(0, 0, 0);
	}

	auto BasePlayer::GetMaxSpeed() -> float {
		const auto get_is_ducked = reinterpret_cast<float (*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("GetMaxSpeed"), 0)));
		return get_is_ducked(this);
	}

	auto BasePlayer::MaxEyeVelocity() -> float
	{
		if (!this) return 0.0f;

		float speed = this->GetMaxSpeed();

		const auto mounted = this->mounted();
		if (mounted)
		{
			return speed * 4.0f;
		}
		return speed;
	}

	bool BaseProjectile::HasReloadCooldown() {
		return UnityEngine::Time().get_time() < nextReloadTime();
	}

	void BaseProjectile::ShotFired() {
		const auto rand_velo = reinterpret_cast<void (*)(BaseProjectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("ShotFired"), 0)));
		return rand_velo(this);
	}

	void BaseProjectile::BeginCycle() {
		const auto rand_velo = reinterpret_cast<void (*)(BaseProjectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("BeginCycle"), 0)));
		return rand_velo(this);
	}

	void BaseProjectile::DidAttackClientside() {
		const auto rand_velo = reinterpret_cast<void (*)(BaseProjectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("DidAttackClientside"), 0)));
		return rand_velo(this);
	}

	void BaseProjectile::UpdateAmmoDisplay() {
		const auto rand_velo = reinterpret_cast<void (*)(BaseProjectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("UpdateAmmoDisplay"), 0)));
		return rand_velo(this);
	}
	void BaseProjectile::DoAttack() {
		const auto rand_velo = reinterpret_cast<void (*)(BaseProjectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("DoAttack"), 0)));
		return rand_velo(this);
	}

	void BaseProjectile::LaunchProjectile() {
		const auto rand_velo = reinterpret_cast<void (*)(BaseProjectile*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("LaunchProjectile"), 0)));
		return rand_velo(this);
	}

	OBB BasePlayer::WorldSpaceBounds()
	{
		if (!this)return {};
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("WorldSpaceBounds"), 0);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<OBB>(procedure, this);
		}

		TRACE("%s: procedure not a valid!", FN);
		return {};
	}

	auto ItemIcon::SetTimedLootAction(int a1, void* a2) -> void {
		const auto LOS = reinterpret_cast<void (*)(ItemIcon*, int, void*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("SetTimedLootAction"), 2)));
		return LOS(this, a1, a2);
	}

	void ItemIcon::RunTimedAction() {
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("RunTimedAction"), 0);
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

	auto ItemIcon::StartTimedLootAction(float delay) -> void {
		const auto LOS = reinterpret_cast<void (*)(ItemIcon*, float)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("StartTimedLootAction"), 1)));
		return LOS(this, delay);
	}

	void PlayerWalkMovement::TeleportTo(Vector3 pos, BasePlayer* player) {
		if (!this) return;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("TeleportTo"), 2);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, pos, player);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	auto PlayerWalkMovement::FallVelocity() -> Vector3 {

		if (!this) return Vector3(0.f, 0.f, 0.f);

		const auto is_dead = reinterpret_cast<Vector3(*)(PlayerWalkMovement*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("FallVelocity"), 0)));
		return is_dead(this);
	}

	void PlayerWalkMovement::Jump(AssemblyCSharp::ModelState* ms)
	{
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Jump"), 2);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, ms, false);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}


	void BaseMovement::TeleportTo(Vector3 pos, BasePlayer* player) {
		if (!this) return;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("TeleportTo"), 2);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, pos, player);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	float BaseMovement::ducking() {
		if (!this) return false;

		static std::size_t offset = 0;
		const auto field = CIl2Cpp::FindField(this->StaticClass(), HASH("<Ducking>k__BackingField"));
		if (memory::IsAddressValid(field))
		{
			offset = CIl2Cpp::il2cpp_field_get_offset(field);
		}

		return *reinterpret_cast<float*>(memory::ToAddress(this) + offset);
	}

	Vector3 BaseMovement::InheritedVelocity() {
		if (!this) return Vector3(0.f, 0.f, 0.f);

		static std::size_t offset = 0;
		const auto field = CIl2Cpp::FindField(this->StaticClass(), HASH("<InheritedVelocity>k__BackingField"));
		if (memory::IsAddressValid(field))
		{
			offset = CIl2Cpp::il2cpp_field_get_offset(field);
		}

		return *reinterpret_cast<Vector3*>(memory::ToAddress(this) + offset);
	}

	float BaseMovement::Crawling() {
		if (!this) return 0.f;

		static std::size_t offset = 0;
		const auto field = CIl2Cpp::FindField(this->StaticClass(), HASH("<Crawling>k__BackingField"));
		if (memory::IsAddressValid(field))
		{
			offset = CIl2Cpp::il2cpp_field_get_offset(field);
		}

		return *reinterpret_cast<float*>(memory::ToAddress(this) + offset);
	}

	void BaseMovement::set_TargetMovement(Vector3 value)
	{
		if (!this) return;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_TargetMovement"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, value);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	void BaseMovement::set_Grounded(float value)
	{
		if (!this) return;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_Grounded"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, value);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	BaseMovement* BasePlayer::GetBaseMovement() {
		auto base_movement = this->movement();

		return (BaseMovement*)base_movement;
	}

	bool BasePlayer::IsSwimming() {
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsSwimming"), 0);
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

	bool BasePlayer::IsRunning() {
		if (!this) return false;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsRunning"), 0);
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

	float BasePlayer::GetRadius() {
		if (!this) return 0.f;
		const auto radius = reinterpret_cast<float(*)(BasePlayer*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("GetRadius"), 0)));
		return radius(this);
	}

	float BasePlayer::GetHeight(bool ducked) {
		if (!this) return 0.f;
		const auto height = reinterpret_cast<float(*)(BasePlayer*, bool)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("GetHeight"), 1)));
		return height(this, ducked);
	}

	void BasePlayer::SetVA(const Vector2& VA)
	{
		if (memory::IsAddressValid(this->input()))
		{
			this->input()->bodyAngles() = VA;
		}
	}

	Vector2 BasePlayer::GetVA()
	{
		if (memory::IsAddressValid(this->input()))
		{
			Vector2 Angles = this->input()->bodyAngles();
			return Angles;
		}
		return Vector2{ 0, 0 };
	}

	Vector2 BasePlayer::GetRA()
	{
		if (memory::IsAddressValid(this->input()))
		{
			Vector2 Angles = this->input()->recoilAngles();
			return Angles;
		}
		return Vector2{ 0, 0 };
	}

	uint64_t StringPool::Get(const char* name)
	{
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethodFullArgs(HASH("Assembly-CSharp::StringPool::Get(String): UInt32"));
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<uint64_t>(procedure, CIl2Cpp::il2cpp_string_new(name));
		}

		TRACE("%s: procedure not a valid!", FN);
		return 0;
	}

	void ViewModel::Play(const char* name, int layer) {
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Play"), 2);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, CIl2Cpp::il2cpp_string_new(name), layer);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}
	auto GamePhysics::Trace(UnityEngine::Ray ray, float radius, UnityEngine::RaycastHit hitInfo, float maxDistance, int layerMask, RustStructs::QueryTriggerInteraction triggerInteraction, BaseEntity* ignoreEntity) -> bool
	{

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Trace"), 7);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool>(procedure, ray, radius, hitInfo, maxDistance, layerMask, triggerInteraction, ignoreEntity);
		}

		TRACE("%s: procedure not a valid!", FN);
		return false;
	}
	auto GamePhysics::Trace2(UnityEngine::Ray* ray, float radius, UnityEngine::RaycastHit hitInfo, float maxDistance, int layerMask, RustStructs::QueryTriggerInteraction triggerInteraction, BaseEntity* ignoreEntity) -> bool
	{

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Trace"), 7);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool>(procedure, ray, radius, hitInfo, maxDistance, layerMask, triggerInteraction, ignoreEntity);
		}

		TRACE("%s: procedure not a valid!", FN);
		return false;
	}
	void BasePlayer::SendProjectileRicochet(ProtoBuf::PlayerProjectileRicochet* ricochet)
	{
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SendProjectileRicochet"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, ricochet);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}
	void BasePlayer::SendProjectileAttack(ProtoBuf::PlayerProjectileAttack* ricochet)
	{
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SendProjectileAttack"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, ricochet);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}
	void BasePlayer::SendProjectileUpdate(ProtoBuf::PlayerProjectileUpdate* ricochet)
	{
		if (!this) return;
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SendProjectileUpdate"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, this, ricochet);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	auto BasePlayer::OnLand(float fVelocity) -> float {
		if (!this) return 0.f;
		const auto is_dead = reinterpret_cast<float(*)(BasePlayer*, float)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("OnLand"), 1)));
		return is_dead(this, fVelocity);
	}

	void BasePlayer::SendClientTick()
	{
		if (!this) return;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SendClientTick"), 0);
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

	void Graphics::SetFov(float value)
	{
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_fov"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<void>(procedure, value);
		}

		TRACE("%s: procedure not a valid!", FN);
		return;
	}

	bool GameTrace::Trace(HitTest* test, int layerMask)
	{
		if (!test)
			return false;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Trace"), 2);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool>(procedure, test, layerMask);
		}

		TRACE("%s: procedure not a valid!", FN);
		return false;
	}

	bool ConsoleNetwork::ClientRunOnServer(Unity_String strCommand)
	{
		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("ClientRunOnServer"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool>(procedure, strCommand);
		}

		TRACE("%s: procedure not a valid!", FN);
		return false;
	}
}

