#include "ClientInput.h"
#include "../../Aimbot/Aimbot.h"
#include "../../Esp/Visuals.h"
#include "Manipulation.h"
bool once = false;


inline void get_cyl_points_z2(std::vector<Vector3>& re, float radius = 9.f, unsigned int sectors = 20, unsigned int rings = 20)
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


inline void DoMeleeAttack(AssemblyCSharp::BasePlayer::Target target, AssemblyCSharp::BaseMelee* baseprojectile, bool is_player = false) {
	if (!memory::IsAddressValid(target.m_player))
		return;

	Vector3 local_position = UnityEngine::Camera::get_main()->get_positionz();

	if (baseprojectile->nextAttackTime() <= UnityEngine::Time::get_time())
	{
		if (baseprojectile->timeSinceDeploy() > baseprojectile->deployDelay())
		{
			auto g_hit_test_class = CIl2Cpp::FindClass(SECURE(""), SECURE("HitTest"));
			auto g_hit_test = CIl2Cpp::il2cpp_object_new((void*)g_hit_test_class);

			if (memory::IsAddressValid(g_hit_test))
			{
				auto hit_test = (AssemblyCSharp::HitTest*)g_hit_test;
				if (memory::IsAddressValid(hit_test))
				{
					UnityEngine::Ray ray = UnityEngine::Ray(local_position, (target.m_position - local_position).Normalized());

					UnityEngine::Transform* trans = is_player ? target.m_player->get_bone_transform(48) : target.m_player->get_transform();
					if (!trans)
						return;

					hit_test->MaxDistance() = 1000;
					hit_test->HitTransform() = trans;
					hit_test->AttackRay() = ray;
					hit_test->DidHit() = true;
					hit_test->HitEntity() = target.m_player;
					hit_test->HitPoint() = trans->InverseTransformPoint(target.m_position);
					hit_test->HitNormal() = Vector3(0, 0, 0);
					hit_test->damageProperties() = baseprojectile->damageProperties();

					baseprojectile->StartAttackCooldown(baseprojectile->repeatDelay());
					return baseprojectile->ProcessAttack(hit_test);
				}
			}

		}
	}
};

auto TOD_Sky_Instance = reinterpret_cast<uint64_t(*)()>(0);

void SkyDome()
{
	if (memory::IsAddressValid(AssemblyCSharp::TOD_Sky::GetClassReference()))
	{
		auto Skydome = AssemblyCSharp::TOD_Sky::GetClassReference();

		TOD_Sky_Instance = reinterpret_cast<uint64_t(*)()>(m_game_assembly + 0x386420);

		auto TOD_Sky = TOD_Sky_Instance();

		if (memory::IsAddressValid(TOD_Sky))
		{
			const auto TOD_Night = *reinterpret_cast<uintptr_t*>(TOD_Sky + 0x58); //public TOD_NightParameters Night
			const auto TOD_Day = *reinterpret_cast<uintptr_t*>(TOD_Sky + 0x50); //public TOD_DayParameters Day

			if (m_options.m_options.BrightNight)
			{
				if (memory::IsAddressValid(Skydome->Night()))
				{
					if (memory::IsAddressValid(TOD_Night))
					{
						*(float*)(TOD_Night + 0x48) = 25.f; //LightIntensity
						*(float*)(TOD_Night + 0x50) = 6.f; //AmbientMultiplier
						*(float*)(TOD_Night + 0x54) = 1.f; //ReflectionMultiplier
					}
				}
			}

			if (m_options.m_options.SkyColor)
			{
				if (memory::IsAddressValid(Skydome->Night()) || memory::IsAddressValid(Skydome->Day()))
				{
					uintptr_t NightTime = *reinterpret_cast<uintptr_t*>(TOD_Night + 0x28);
					uintptr_t NightTimeInstance = *reinterpret_cast<uintptr_t*>(NightTime + 0x10);

					uintptr_t DayTime = *reinterpret_cast<uintptr_t*>(TOD_Day + 0x28);
					uintptr_t DayTimeInstance = *reinterpret_cast<uintptr_t*>(DayTime + 0x10);

					auto SkyColor = Color{ m_colors.m_visuals.SkyColor[0], m_colors.m_visuals.SkyColor[1], m_colors.m_visuals.SkyColor[2], m_colors.m_visuals.SkyColor[3] };

					if (memory::IsAddressValid(TOD_Night))
						*(Color*)(NightTimeInstance) = SkyColor.GetUnityColor();

					if (memory::IsAddressValid(TOD_Day))
						*(Color*)(DayTimeInstance) = SkyColor.GetUnityColor();
				}
			}

			if (m_options.m_options.CloudColor)
			{
				if (memory::IsAddressValid(Skydome->Night()) || memory::IsAddressValid(Skydome->Day()))
				{
					uintptr_t NightTime = *reinterpret_cast<uintptr_t*>(TOD_Night + 0x30);
					uintptr_t NightTimeInstance = *reinterpret_cast<uintptr_t*>(NightTime + 0x10);

					uintptr_t DayTime = *reinterpret_cast<uintptr_t*>(TOD_Day + 0x30);
					uintptr_t DayTimeInstance = *reinterpret_cast<uintptr_t*>(DayTime + 0x10);

					auto CloudColor = Color{ m_colors.m_visuals.CloudColor[0], m_colors.m_visuals.CloudColor[1], m_colors.m_visuals.CloudColor[2], m_colors.m_visuals.CloudColor[3] };

					if (memory::IsAddressValid(TOD_Night))
						*(Color*)(NightTimeInstance) = CloudColor.GetUnityColor();

					if (memory::IsAddressValid(TOD_Day))
						*(Color*)(DayTimeInstance) = CloudColor.GetUnityColor();
				}
			}

			if (m_options.m_options.BrightCave)
			{
				if (memory::IsAddressValid(Skydome->Day()))
				{
					Skydome->Day()->AmbientMultiplier() = 1.f;
					Skydome->Day()->ReflectionMultiplier() = 1.f;
				}
			}

			if (m_options.m_options.Stars)
			{
				if (memory::IsAddressValid(Skydome->Stars()))
				{
					Skydome->Stars()->Brightness() = 150.f;
				}
			}
		}

		// rainbow clounds no box
		if (m_options.m_options.RainbowClouds)
		{
			auto components = Skydome->get_Components();
			if (memory::IsAddressValid(components))
			{
				auto BillboardMaterials = components->BillboardMaterials();

				if (BillboardMaterials)
				{
					auto size = BillboardMaterials->max_length;
					for (int i = 0; i < size; i++)
					{
						auto main_renderer = BillboardMaterials->m_Items[i];
						if (!memory::IsAddressValid(main_renderer))
							continue;

						forcefield_shader = force_field_bundle->LoadAsset<UnityEngine::Shader>(SECURE("rainbow.shader"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Shader"))));

						main_renderer->set_shader(forcefield_shader);
					}
				}
			}
		}
		

		//if (m_options.m_options.SkyColor)
		//{
		//	if (memory::IsAddressValid(Skydome->Day()))
		//	{
		//		Skydome->Day()->SkyColor()->m_Ptr() = { 1, 0, 0, 1 };
		//	}
		//}
	}
}

float flyhackDistanceVertical = 0.f;
float flyhackDistanceHorizontal = 0.f;
float flyhackPauseTime;

bool TestFlying(AssemblyCSharp::BasePlayer* _This, Vector3 oldPos, Vector3 newPos)
{

	if (auto LocalPlayer = _This)
	{
		flyhackPauseTime = maxx(0.f, flyhackPauseTime - UnityEngine::Time::get_fixedDeltaTime());
		bool inAir = false;

		float radius = LocalPlayer->GetRadius();
		float height = LocalPlayer->GetHeight(false);

		Vector3 vector = (oldPos + newPos) * 0.5f;
		Vector3 vector2 = vector + Vector3(0.f, radius - 2.f, 0.f);
		Vector3 vector3 = vector + Vector3(0.f, height - radius, 0.f);
		float radius2 = radius - 0.05f;

		bool a = UnityEngine::Physics::CheckCapsule(vector2, vector3, radius2, 1503731969, RustStructs::Ignore);
		inAir = !a;

		if (inAir)
		{
			bool flag = false;

			Vector3 vector4 = newPos - oldPos;
			float num3 = win32::abs(vector4.y);
			float num4 = vector4.Magnitude2D();

			if (vector4.y >= 0.f) {
				flyhackDistanceVertical += vector4.y;
				flag = true;
			}
			if (num3 < num4) {
				flyhackDistanceHorizontal += num4;
				flag = true;
			}
			if (flag) {
				float num5 = maxx((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
				float num6 = _This->GetJumpHeight() + num5;
				if (flyhackDistanceVertical > num6) {
					return true;
				}
				float num7 = maxx((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
				float num8 = 5.f + num7;
				if (flyhackDistanceHorizontal > num8) {
					return true;
				}
			}
		}
		else
		{
			flyhackDistanceHorizontal = 0.f;
			flyhackDistanceVertical = 0.f;
		}
	}
	return false;
}

bool CheckFlyhack(AssemblyCSharp::BasePlayer* _This, bool PreventFlyhack)
{
	bool result;

	if (!_This->lastSentTick())
		return false;

	if (!_This->get_transform())
		return false;

	if (auto LocalPlayer = _This)
	{
		if (auto Transform = LocalPlayer->get_transform())
		{
			if (!_This->IsDead() && !_This->IsSleeper() && !_This->mounted() && !_This->OnLadder())
			{
				if (memory::IsAddressValid(LocalPlayer->lastSentTick()) && memory::IsAddressValid(LocalPlayer->get_transform()))
				{
					Vector3 oldPos = LocalPlayer->lastSentTick()->position();

					Vector3 modelPos = LocalPlayer->get_transform()->get_position();

					result = TestFlying(LocalPlayer, oldPos, modelPos);

					float num5 = maxx((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
					float num6 = _This->GetJumpHeight() + num5;

					m_options.m_options.max_flyhack_vertical = num6;
					m_options.m_options.vertical_flyhack = flyhackDistanceVertical;

					float num7 = maxx((flyhackPauseTime > 0.f) ? 10 : 1.5, 0.f);
					float num8 = 5.f + num7;

					m_options.m_options.max_flyhack_horisontal = num8;
					m_options.m_options.horisontal_flyhack = flyhackDistanceHorizontal;

					if (result && PreventFlyhack)
					{
						const auto last_sent_tick = LocalPlayer->lastSentTick();
						const auto player_movement = LocalPlayer->movement();
						if (player_movement && last_sent_tick) {
							auto LastSentTickPos = last_sent_tick->position();
							reinterpret_cast<AssemblyCSharp::BaseMovement*>(player_movement)->set_TargetMovement(Vector3());
							reinterpret_cast<AssemblyCSharp::BaseMovement*>(player_movement)->TeleportTo(Vector3(LastSentTickPos.x, LastSentTickPos.y - 0.001, LastSentTickPos.z), LocalPlayer);
							if (const auto rigid_body = player_movement->body()) {
								rigid_body->set_velocity(Vector3());
							}
						}

					}
					return result;
				}

			}
			
		}
	}

	return false;
}

inline void DoOreAttack(Vector3 pos, AssemblyCSharp::BaseEntity* p, AssemblyCSharp::BaseMelee* w)
{
	if (!p) return;
	if (!w) return;


	if (w->nextAttackTime() >= UnityEngine::Time::get_time()) return;
	if (w->timeSinceDeploy() < w->deployDelay()) return;

	auto g_hit_test_class = CIl2Cpp::FindClass(SECURE(""), SECURE("HitTest"));
	auto g_hit_test = (AssemblyCSharp::HitTest*)CIl2Cpp::il2cpp_object_new((void*)g_hit_test_class);

	auto trans = p->get_transform();
	if (!trans) return;
	UnityEngine::Ray r = UnityEngine::Ray(AssemblyCSharp::LocalPlayer::get_Entity()->get_transform()->get_position(), (pos - AssemblyCSharp::LocalPlayer::get_Entity()->get_transform()->get_position()).Normalized());

	g_hit_test->MaxDistance() = 1000.f;
	g_hit_test->AttackRay() = r;
	g_hit_test->DidHit() = true;
	g_hit_test->HitTransform() = trans;
	g_hit_test->HitEntity() = p;
	g_hit_test->HitPoint() = trans->InverseTransformPoint(pos);
	g_hit_test->HitNormal() = trans->InverseTransformDirection(pos);
	g_hit_test->damageProperties() = w->damageProperties();
	//w->StartAttackCooldown(0.001f);
	w->StartAttackCooldown(w->repeatDelay());

	return w->ProcessAttack((AssemblyCSharp::HitTest*)g_hit_test);
}

bool RunOnce = false;

#define M_PI 3.14159265358979323846f
#define M_PI_F ((float)(M_PI))
#define RAD2DEG(val) ((float)(val) * (float)(180.0f / M_PI_F))

inline Vector2 CalcAngle(const Vector3& Src, const Vector3& Dst)
{
	Vector3 dir = Src - Dst;
	return Vector2{ RAD2DEG(win32::asin(dir.y / dir.Length())), RAD2DEG(-win32::atan2(dir.x, -dir.z)) };
}

inline void Normalize(float& Yaw, float& Pitch)
{
	if (Pitch < -89) Pitch = -89;
	else if (Pitch > 89) Pitch = 89;
	if (Yaw < -360) Yaw += 360;
	else if (Yaw > 360) Yaw -= 360;
}

#define M_PI_2 1.57079632679489661923

void get_sphere_points(float radius, unsigned int sectors, std::vector<Vector3>& re, float max = 1.6f)
{



	for (float y = -radius; y < radius; y += 0.1f) {
		int points = sectors;
		float step = (M_PI_2) / points;
		float x, z, current = 0;
		for (size_t i = 0; i < points; i++)
		{
			x = win32::sin(current) * radius;
			z = win32::cos(current) * radius;

			re.push_back(Vector3(0, y, 0));
			re.push_back(Vector3(x, y, z));
			re.push_back(Vector3(-x, y, z));
			re.push_back(Vector3(x, y, -z));
			re.push_back(Vector3(-x, y, -z));

			current += step;
		}
	}
}

Vector3 findClosestPosition(Vector3 playerPosition, std::vector<Vector3>& allPositions) {
	double minDistance = 2.0; // Set the maximum radius
	Vector3 closestPosition = playerPosition;

	for (const Vector3& position : allPositions) {
		double dist = playerPosition.Distance(position);
		if (dist <= minDistance) {
			minDistance = dist;
			closestPosition = position;
		}
	}

	return closestPosition;
}

void NewBulletTPChecks(AssemblyCSharp::BasePlayer* local, AssemblyCSharp::BasePlayer* target, Vector3 m_position)
{
	if (!local)
		return;

	if (!target)
		return;

	auto activeWeapon = local->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>();
	if (!activeWeapon)
		return;


	//if (targetpos.IsZero())
	//{
	//	targetpos = m_position;
	//}

	//bool isBow = activeWeapon->IsA(AssemblyCSharp::BowWeapon::StaticClass()) || activeWeapon->IsA(AssemblyCSharp::BaseMelee::StaticClass()) || activeWeapon->IsA(AssemblyCSharp::CompoundBowWeapon::StaticClass()) || activeWeapon->IsA(AssemblyCSharp::CrossbowWeapon::StaticClass());

	auto re_p2 = local->eyes()->get_position() + spoofed_eye_pos;


	auto layer = (int)RustStructs::Layers::ProjectileLineOfSightCheck | (int)RustStructs::Layers::Terrain | (int)RustStructs::Layers::z | (int)RustStructs::Layers::EntityLineOfSightCheck | (int)RustStructs::Layers::Construction | (int)RustStructs::Layers::ConstructionSocket;
	auto Locallayer =/* (int)RustStructs::Layers::ProjectileLineOfSightCheck | */(int)RustStructs::Layers::Terrain | (int)RustStructs::Layers::z/* | (int)RustStructs::Layers::EntityLineOfSightCheck | (int)RustStructs::Layers::Construction*/;

	auto MaxThickness = m_options.m_options.Max_Thickness / 10;

	if (target->mounted())
	{
		MaxThickness = 8.f;
	}

	std::vector<Vector3> arr = {};
	get_sphere_points(MaxThickness, 5, arr);

	float min_distance = 2.f;


	for (auto s : arr) {
		Vector3 point = m_position + s;


		//if (!isBow)
		//{
			if (AssemblyCSharp::IsVisible(local->eyes()->get_position(), m_position))
			{
				targetpos = m_position;
				continue;
			}
		//}

		
		if (!AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position() + spoofed_eye_pos, m_position + s))
		{
			continue;
		}

		//auto NextClosestPositionToPlayer = local->ClosestPoint(m_position + s);

		//UnityEngine::DDraw().Sphere(NextClosestPositionToPlayer, 0.5f, Color::Red(), 0.05f, false);

		//if (!AssemblyCSharp::IsVisible_2(re_p2, point, Locallayer, 0.2f))
		//	continue;

		if (!ValidateEyePos(target, m_position + s))
			continue;

		//if (!AssemblyCSharp::IsVisible_2(target->get_transform()->get_position(), point, layer, 0.f))
		//{
		//	continue;
		//}

		if (m_options.m_options.DrawBulletTP)
			UnityEngine::DDraw().Sphere(m_position + s, 0.5f, Color::Red(), 0.05f, false);

		m_options.m_options.Thickbullet_Indicator = true;
		m_options.m_options.Thickbullet_AutoShoot = true;
		bullettppos = s;
		targetpos = m_position + s;
		return;
	}
	//bullettppos = Vector3(0, 0, 0);
	targetpos = m_position;
	m_options.m_options.Thickbullet_Indicator = false;
	m_options.m_options.Thickbullet_AutoShoot = false;
}

void DoMovement(std::vector<Vector3>& output, Vector3 currentPos, Vector3 currentVelocity, float step = 0.1f, int maxSteps = 20, float gravityModifier = 1.0f)
{
	for (int i = 0; i < maxSteps; i++)
	{
		output.push_back(currentPos);
		//DoMovement

		currentVelocity += UnityEngine::Physics::get_gravity() * gravityModifier * step;
		currentPos = currentPos + currentVelocity * step;

		//Update velocity

		//currentVelocity -= currentVelocity.normalized * (currentVelocity.magnitude * this.drag * step);
	}
	output.push_back(currentPos);
}

std::string findJsonValue(std::string jsonString, std::string key) {
	std::size_t keyPos = jsonString.find(SECURE("\"") + key + SECURE("\""));
	//if (keyPos == std::string::npos) {
	//	return SECURE(""); // Key not found
	//}

	std::size_t valueStartPos = jsonString.find(SECURE(":"), keyPos);
	//if (valueStartPos == std::string::npos) {
	//	return SECURE(""); // Invalid JSON format
	//}

	valueStartPos = jsonString.find_first_not_of(SECURE(" \t\n\r\f\v"), valueStartPos + 1);
	//if (valueStartPos == std::string::npos) {
	//	return SECURE(""); // Invalid JSON format
	//}

	if (jsonString[valueStartPos] == '{') {
		// Nested JSON object, find its closing brace
		std::size_t nestedEndPos = jsonString.find_last_of('}', jsonString.size());
		//if (nestedEndPos == std::string::npos) {
		//	return SECURE(""); // Invalid JSON format
		//}
		return jsonString.substr(valueStartPos, nestedEndPos - valueStartPos + 1);
	}

	std::size_t valueEndPos = jsonString.find_first_of(SECURE(",}"), valueStartPos);
	//if (valueEndPos == std::string::npos) {
	//	return SECURE(""); // Invalid JSON format
	//}

	std::string value = jsonString.substr(valueStartPos, valueEndPos - valueStartPos);
	value.erase(0, value.find_first_not_of(SECURE(" \t\n\r\f\v")));
	value.erase(value.find_last_not_of(SECURE(" \t\n\r\f\v")) + 1);

	return value;
}


int stoi_crt(const std::string& str, size_t* pos = 0, int base = 10) {
	// Check if the string is empty
	if (str.empty()) {
		return 0;
	}

	// Find the position of the first non-whitespace character
	size_t start = str.find_first_not_of(SECURE(" \t\n\v\f\r"));

	// If all characters are whitespaces, throw an exception
	if (start == std::string::npos) {
		return 0;
	}

	// Check for a sign character
	bool negative = false;
	if (str[start] == '+' || str[start] == '-') {
		negative = (str[start] == '-');
		++start;
	}

	// Check the base and handle "0x" prefix for hexadecimal values
	if (base == 0) {
		if (start + 1 < str.length() && str[start] == '0') {
			if (str[start + 1] == 'x' || str[start + 1] == 'X') {
				base = 16;
				start += 2;
			}
			else {
				base = 8;
				++start;
			}
		}
		else {
			base = 10;
		}
	}
	else if (base == 16 && start + 1 < str.length() && str[start] == '0' && (str[start + 1] == 'x' || str[start + 1] == 'X')) {
		start += 2;
	}

	// Convert the string to an integer
	int result = 0;
	for (size_t i = start; i < str.length(); ++i) {
		char c = str[i];
		int digit;
		if (c >= '0' && c <= '9') {
			digit = c - '0';
		}
		else if (base == 16 && c >= 'a' && c <= 'f') {
			digit = c - 'a' + 10;
		}
		else if (base == 16 && c >= 'A' && c <= 'F') {
			digit = c - 'A' + 10;
		}
		else {
			break; // Invalid character encountered
		}

		if (digit >= base) {
			break; // Invalid digit for the given base
		}

		// Check for overflow before updating the result
		if (negative) {
			if (result < (INT32_MIN + digit) / base) {
				return 0;
			}
		}
		else {
			if (result > (INT32_MAX - digit) / base) {
				return 0;
			}
		}

		result = result * base + digit;
	}

	if (pos != 0) {
		*pos = start;
	}

	return negative ? -result : result;
}

ULONG64 SteamID3To64z(ULONG64 id)
{
	return ((1ULL << 56) | (1ULL << 52) | (1ULL << 32) | id);
}

std::string DWORD64ToString(DWORD64 number) {
	if (number == 0) {
		return "0";
	}

	std::string result;
	while (number > 0) {
		char digit = '0' + static_cast<char>(number % 10);
		result = digit + result;
		number /= 10;
	}

	return result;
}


static bool TestInsideTerrain2(Vector3 pos)
{
	bool result;
	if (!AssemblyCSharp::TerrainMeta::get_Terrain())
	{
		result = false;
	}
	else if (!AssemblyCSharp::TerrainMeta::get_HeightMap())
	{
		result = false;
	}
	else if (!AssemblyCSharp::TerrainMeta::get_Collision())
	{
		result = false;
	}
	else
	{

		float terrain_padding = 0.3f;
		float height = AssemblyCSharp::TerrainMeta::get_HeightMap()->GetHeight(pos);
		if (pos.y > height - terrain_padding)
		{
			result = false;
		}
		else
		{
			float num = AssemblyCSharp::TerrainMeta::get_Position().y + AssemblyCSharp::TerrainMeta::get_Terrain()->SampleHeight(pos);
			if (pos.y > num - terrain_padding)
			{
				result = false;
			}
			else if (AssemblyCSharp::TerrainMeta::get_Collision()->GetIgnore(pos, 0.01f))
			{
				result = false;
			}
			else
			{
				result = true;
			}
		}
	}
	return result;
}

bool TestInsideTerrain(Vector3 pos)
{
	if (!AssemblyCSharp::TerrainMeta::get_Terrain()) return false;
	if (!AssemblyCSharp::TerrainMeta::get_HeightMap()) return false;
	if (!AssemblyCSharp::TerrainMeta::get_Terrain()) return false;
	if (!AssemblyCSharp::TerrainMeta::get_Collision()) return false;

	float height = AssemblyCSharp::TerrainMeta::get_HeightMap()->GetHeight(pos);

	if (pos.y > height - 0.3f)
		return false;

	float gaysex = AssemblyCSharp::TerrainMeta::get_Position().y + AssemblyCSharp::TerrainMeta::get_Terrain()->SampleHeight(pos);
	return pos.y <= gaysex - 0.3f && !AssemblyCSharp::TerrainMeta::get_Collision()->GetIgnore(pos, 0.01f);
}

bool IsInsideTerrain(bool prevent = false)
{
	BasePlayer* player = Visuals::m_local_player;
	if (!player)
		return false;

	bool result = TestInsideTerrain(player->get_transform()->get_position());

	if (prevent && result)
		player->ForcePositionTo(player->lastSentTick()->position());
	return result;
}

void Hooks::ClientInput(AssemblyCSharp::BasePlayer* a1, AssemblyCSharp::InputState* a2)
{
	if (!m_aimbot.m_aimbot.ConnectedToServer)
		return Hooks::ClientInputhk.get_original< decltype(&ClientInput)>()(a1, a2);

	if (!a1 || !a2)
		return Hooks::ClientInputhk.get_original< decltype(&ClientInput)>()(a1, a2);

	if (a1->lifestate() & RustStructs::Dead || !a1->IsConnected())
	{
		return Hooks::ClientInputhk.get_original< decltype(&ClientInput)>()(a1, a2);
	}

	//auto IsInGame = CEntityManager::IsInGame();
	//if (!IsInGame)
	//{
	//	m_aimbot.m_aimbot.ConnectedToServer = false;
	//	return Hooks::ClientInputhk.get_original< decltype(&ClientInput)>()(a1, a2);
	//}
	
	if (!Visuals::m_local_player)
		Visuals::m_local_player = a1;

	//if (!force_field_bundle) {
	//	//test file

	//	force_field_bundle = UnityEngine::AssetBundle::LoadFromFile_Internal(SECURE("C:\\Asset.bundle"), 0, 0);

	//	// Load bundle from memory
	//	//constexpr size_t bundleSize = sizeof(forcefield_bundle);
	//	//auto bundleArray = (System::c_system_array<System::Byte*>*)System::il2cpp_array_new(System::Byte::StaticClass(), sizeof(forcefield_bundle));
	//	//std::memcpy(bundleArray->items, forcefield_bundle, bundleSize);
	//	//force_field_bundle = UnityEngine::AssetBundle::LoadFromMemory_Internal(bundleArray, 0, 0);
	//}

	if (!RunOnce)
	{
		Hooks::TryToMovehk.VirtualFunctionHook(SECURE("ItemIcon"), HASH("TryToMove"), &Hooks::TryToMove, SECURE(""), 0);
		Hooks::PlayerWalkMovementhk.VirtualFunctionHook(SECURE("PlayerWalkMovement"), HASH("ClientInput"), &Hooks::PlayerWalkMovement, SECURE(""), 2);
		Hooks::DoFixedUpdatehk.VirtualFunctionHook(SECURE("PlayerWalkMovement"), HASH("DoFixedUpdate"), &Hooks::DoFixedUpdate, SECURE(""), 1);
		Hooks::BlockSprinthk.VirtualFunctionHook(SECURE("BasePlayer"), HASH("BlockSprint"), &Hooks::BlockSprint, SECURE(""), 1);


		fired_projectile placeholder = { nullptr, 0, 1 };
		for (size_t i = 0; i < 32; i++)
			fired_projectiles[i] = placeholder;

		RunOnce = true;
	}

	if (m_options.m_options.AntiDeathBarrier)
	{
		IsInsideTerrain(m_options.m_options.AntiDeathBarrier);
	}

	/*bool isInTerrain = TestInsideTerrain(a1->get_transform()->get_position());

	printfz("\n isInTerrain - %d", isInTerrain);*/
	
	if (!memory::IsAddressValid(memory::ToAddress(Hooks::PPS_WriteToStreamhk.get_original<void*>()))
		|| !memory::IsAddressValid(memory::ToAddress(Hooks::OnInputhk.get_original<void*>()))
		|| !memory::IsAddressValid(memory::ToAddress(Hooks::PPA_WriteToStreamhk.get_original<void*>()))
		|| !memory::IsAddressValid(memory::ToAddress(Hooks::PlayerProjectileRicochethk.get_original<void*>()))
		)

	{
		Hooks::PPS_WriteToStreamhk.VirtualFunctionHook(SECURE("ProjectileShoot"), HASH("WriteToStream"), &Hooks::PPS_WriteToStream, SECURE("ProtoBuf"), 1);
		Hooks::OnInputhk.VirtualFunctionHook(SECURE("BaseMelee"), HASH("OnInput"), &Hooks::OnInput, SECURE(""), 0);
		Hooks::PPA_WriteToStreamhk.VirtualFunctionHook(SECURE("PlayerProjectileAttack"), HASH("WriteToStream"), &Hooks::PPA_WriteToStream, SECURE("ProtoBuf"), 1);
		Hooks::PlayerProjectileRicochethk.VirtualFunctionHook(SECURE("PlayerProjectileRicochet"), HASH("WriteToStream"), &Hooks::PlayerProjectileRicochet, SECURE("ProtoBuf"), 1);
	}

	if (m_options.m_options.BrightCave || m_options.m_options.BrightNight || m_options.m_options.Stars || m_options.m_options.RainbowClouds)
		SkyDome();

	if (m_options.m_options.AutoShoot)
	{

		if (m_options.m_options.waitForInstantHit && m_options.m_options.InstantBullet)
		{
			auto desynctime = UnityEngine::Time::get_realtimeSinceStartup() - AssemblyCSharp::LocalPlayer::get_Entity()->lastSentTickTime();
			auto desyncpercentage = (((desynctime / 0.85f) * 100.0f) + 1.f) / 100;


			if (desyncpercentage >= 0.85f)
			{
				auto baseProjectile = Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>();
				if (baseProjectile)
				{
					auto camera = UnityEngine::Camera::get_main();
					if (camera) {
						auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
						if (AimbotTarget.m_player)
						{
							auto HeldItem = Visuals::m_local_player->ActiveItem();
							if (memory::IsAddressValid(HeldItem))
							{
								auto HeldItemID = HeldItem->info()->itemid();


								if (!baseProjectile->IsA(AssemblyCSharp::BaseMelee::StaticClass()) &&HeldItemID != 442886268 && HeldItemID != 1443579727 && HeldItemID != 1965232394 && HeldItemID != 884424049)
								{

									if (UnityEngine::Input::GetKey(m_aimbot.m_aimbot.ManipKey))
									{
										if (!baseProjectile->HasReloadCooldown())
										{
											if (baseProjectile->timeSinceDeploy() > baseProjectile->deployDelay())
											{
												if (m_options.m_options.StartShooting || m_options.m_options.Thickbullet_AutoShoot)
												{
													if (AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position() + spoofed_eye_pos, targetpos))
													{
														if (!baseProjectile->HasAttackCooldown())
														{
															baseProjectile->DoAttackRecreation();
														}
													}
												}
												else
												{
													if (AssemblyCSharp::IsVisible(AimbotTarget.m_player->get_bone_transform(48)->get_position(), AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position()))
													{
														if (!baseProjectile->HasAttackCooldown())
														{
															baseProjectile->DoAttackRecreation();
														}
													}
												}
											}

										}
									}
									else if (m_options.m_options.AlwaysAutoshoot)
									{
										if (!baseProjectile->HasReloadCooldown())
										{
											if (baseProjectile->timeSinceDeploy() > baseProjectile->deployDelay())
											{
												if (m_options.m_options.StartShooting || m_options.m_options.Thickbullet_AutoShoot)
												{
													if (AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position() + spoofed_eye_pos, targetpos))
													{
														if (!baseProjectile->HasAttackCooldown())
														{
															baseProjectile->DoAttackRecreation();
														}
													}
												}
												else
												{
													if (AssemblyCSharp::IsVisible(AimbotTarget.m_player->get_bone_transform(48)->get_position(), AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position()))
													{
														if (!baseProjectile->HasAttackCooldown())
														{
															baseProjectile->DoAttackRecreation();
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
				}

			}
		}
		else
		{
			auto baseProjectile = Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>();
			if (baseProjectile)
			{
				auto camera = UnityEngine::Camera::get_main();
				if (camera) {
					auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
					if (AimbotTarget.m_player)
					{
						auto HeldItem = Visuals::m_local_player->ActiveItem();
						if (memory::IsAddressValid(HeldItem))
						{
							auto HeldItemID = HeldItem->info()->itemid();


							if (!baseProjectile->IsA(AssemblyCSharp::BaseMelee::StaticClass()) && HeldItemID != 442886268 && HeldItemID != 1443579727 && HeldItemID != 1965232394 && HeldItemID != 884424049)
							{

								if (UnityEngine::Input::GetKey(m_aimbot.m_aimbot.ManipKey))
								{
									if (!baseProjectile->HasReloadCooldown())
									{
										if (baseProjectile->timeSinceDeploy() > baseProjectile->deployDelay())
										{
											if (m_options.m_options.StartShooting || m_options.m_options.Thickbullet_AutoShoot)
											{
												if (AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position() + spoofed_eye_pos, targetpos))
												{
													if (!baseProjectile->HasAttackCooldown())
													{
														baseProjectile->DoAttackRecreation();
													}
												}
											}
											else
											{
												if (AssemblyCSharp::IsVisible(AimbotTarget.m_player->get_bone_transform(48)->get_position(), AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position()))
												{
													if (!baseProjectile->HasAttackCooldown())
													{
														baseProjectile->DoAttackRecreation();
													}
												}
											}
										}

									}
								}
								else if (m_options.m_options.AlwaysAutoshoot)
								{
									if (!baseProjectile->HasReloadCooldown())
									{
										if (baseProjectile->timeSinceDeploy() > baseProjectile->deployDelay())
										{
											if (m_options.m_options.StartShooting || m_options.m_options.Thickbullet_AutoShoot)
											{
												if (AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position() + spoofed_eye_pos, targetpos))
												{
													if (!baseProjectile->HasAttackCooldown())
													{
														baseProjectile->DoAttackRecreation();
													}
												}
											}
											else
											{
												if (AssemblyCSharp::IsVisible(AimbotTarget.m_player->get_bone_transform(48)->get_position(), AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position()))
												{
													if (!baseProjectile->HasAttackCooldown())
													{
														baseProjectile->DoAttackRecreation();
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
			}

		}

	}

	if (m_options.m_options.Flyhack_Indicators)
		CheckFlyhack(a1, m_options.m_options.AntiFlyhack_Kick);

	if (m_options.m_options.ThickBullet)
	{
		auto camera = UnityEngine::Camera::get_main();
		if (memory::IsAddressValid(camera))
		{
			auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
			if (memory::IsAddressValid(m_target.m_player))
			{
				if (m_target.m_player->IsConnected())
				{
					m_target.m_player->get_transform()->set_rotation(Vector4(0.f, 0.f, 0.f, 1.f)); //Fix all player rotations for bullet tp to not have invalids.
				}
				
			}

		}
	}

	if (m_options.m_options.AutoPickup)
	{
		f_object entity = f_object::get_closest_object(a1->get_bone_transform(47)->get_position(),
			SECURE("/collectable/"),
			Vector3(),
			Vector3(),
			Vector3(),
			false
		);
		if (entity.valid) {

			if (RPC_Counter.Calculate() <= 5)
			{
				Vector3 local = a1->ClosestPoint(entity.position);
				if (local.get_3d_dist(entity.position) <= 3.f) {
					auto entity_cast = reinterpret_cast<AssemblyCSharp::BaseEntity*>(entity.entity);
					if (memory::IsAddressValid(entity_cast))
					{
						entity_cast->ServerRPC(SECURE("Pickup"));
						RPC_Counter.Increment();
					}
				}
			}

			
		}
	}

	if (m_options.m_options.AutoFarm)
	{


		auto base_melee = AssemblyCSharp::LocalPlayer::get_Entity()->GetHeldEntityCast<AssemblyCSharp::BaseMelee>();

		if (base_melee->class_name_hash() == HASH("BaseMelee") || base_melee->class_name_hash() == HASH("JackHammer"))
		{
			f_object ore_hot_spot = f_object::get_closest_object(a1->get_bone_transform(47)->get_position(),
				SECURE(""),
				Vector3(),
				Vector3(),
				Vector3(),
				true,
				SECURE("OreHotSpot"));
			if (ore_hot_spot.valid) {
				Vector3 local = a1->ClosestPoint(ore_hot_spot.position);
				if (local.get_3d_dist(ore_hot_spot.position) <= 5.f) {
					DoOreAttack(ore_hot_spot.position, (AssemblyCSharp::BaseEntity*)ore_hot_spot.entity, base_melee);
				}
			}

			f_object barrel_hot_spot = f_object::get_closest_object(a1->get_bone_transform(47)->get_position(),
				SECURE("barrel"),
				Vector3(),
				Vector3(),
				Vector3(),
				false,
				SECURE(""));
			if (barrel_hot_spot.valid) {
				Vector3 local = a1->ClosestPoint(barrel_hot_spot.position);
				if (local.get_3d_dist(barrel_hot_spot.position) <= 4.f) {
					DoOreAttack(barrel_hot_spot.position, (AssemblyCSharp::BaseEntity*)barrel_hot_spot.entity, base_melee);
				}
			}


			f_object tree_entity = f_object::get_closest_object(a1->get_bone_transform(47)->get_position(),
				SECURE(""),
				Vector3(),
				Vector3(),
				Vector3(),
				true,
				SECURE("TreeEntity"));
			if (tree_entity.valid) {
				tree_entity.position = Vector3();
				f_object tree_marker = f_object::get_closest_object(a1->get_bone_transform(47)->get_position(),
					SECURE(""),
					Vector3(),
					Vector3(),
					Vector3(),
					true,
					SECURE("TreeMarker"));
				if (tree_marker.valid) {
					Vector3 locala = a1->ClosestPoint(tree_marker.position);
					if (locala.get_3d_dist(tree_marker.position) <= 3.f) {
						tree_entity.position = tree_marker.position;
						Vector3 local = a1->ClosestPoint(tree_entity.position);
						if (local.get_3d_dist(tree_entity.position) <= 3.f) {
							DoOreAttack(tree_entity.position, (AssemblyCSharp::BaseEntity*)tree_entity.entity, base_melee);
						}
					}
				}
			}

		}
	}

	if (const auto base_projectile = a1->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>())
	{
		if (a1->IsWeapon() || a1->IsMelee())
		{

			if (m_options.m_options.ChangeRepeatDelay)
			{
				base_projectile->repeatDelay() = 0.02f;
			}


			if (m_options.m_options.DrawWeaponChams)
			{
				if (memory::IsAddressValid(AssemblyCSharp::BaseViewModel::get_ActiveModel()))
				{
					AssemblyCSharp::BaseViewModel::get_ActiveModel()->useViewModelCamera() = false;
				}

				auto g_render = AssemblyCSharp::BaseViewModel::get_ActiveModel()->GetComponentsInChildren(System::Type::Renderer());
				if (memory::IsAddressValid(g_render))
				{
					auto size = g_render->max_length;
					for (int i = 0; i < size; i++)
					{
						auto main_renderer = g_render->m_Items[i];
						if (!memory::IsAddressValid(main_renderer))
							continue;

						auto material = main_renderer->material();

						if (!memory::IsAddressValid(material))
							continue;

						if (material->get_name()->Contains(SECURE("sparks2"))
							|| material->get_name()->Contains(SECURE("puff-3"))
							|| material->get_name()->Contains(SECURE("c4_smoke_01"))
							|| material->get_name()->Contains(SECURE("HeavyRefract"))
							|| material->get_name()->Contains(SECURE("pfx_smoke_whispy_1_white_viewmodel"))
							|| material->get_name()->Contains(SECURE("Ak47uIce Specular"))
							//|| Material->name()->Contains(E(L"ak47_barrel_ice"
							//|| Material->name()->Contains(E(L"ak47_maggrip_ice"
							|| material->get_name()->Contains(SECURE("muzzle_embers"))
							|| material->get_name()->Contains(SECURE("c4charge"))
							|| material->get_name()->Contains(SECURE("pfx_smoke_rocket"))
							|| material->get_name()->Contains(SECURE("pfx_smoke_rocket_thicksoftblend"))
							|| material->get_name()->Contains(SECURE("pfx_smoke_rocket_thicksoftblend"))
							|| material->get_name()->Contains(SECURE("muzzle_fumes1"))
							|| material->get_name()->Contains(SECURE("muzzle_fumes2"))
							|| material->get_name()->Contains(SECURE("muzzle_fumes3"))
							|| material->get_name()->Contains(SECURE("wispy-2"))
							|| material->get_name()->Contains(SECURE("quickblast-1"))
							|| material->get_name()->Contains(SECURE("muzzle_flash-front-3x3"))
							|| material->get_name()->Contains(SECURE("muzzle_flash-cross"))
							|| material->get_name()->Contains(SECURE("muzzle_flash-side-1x4")))
							continue;


						switch (m_options.m_options.selectedWeaponChams)
						{
						case 0:
						{

							//if (!forcefield_shader)
							forcefield_shader = force_field_bundle->LoadAsset<UnityEngine::Shader>(SECURE("galaxymaterial.shader"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Shader"))));

							//if (!forcefield_material)
							forcefield_material = force_field_bundle->LoadAsset<UnityEngine::Material>(SECURE("galaxymaterial_03.mat"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material"))));


							if (material->shader() != forcefield_shader)
							{
								main_renderer->set_material(forcefield_material);
								material->set_shader(forcefield_shader);
							}
							break;
						}
						case 1:
						{

							//if (!forcefield_shader)
							forcefield_shader = force_field_bundle->LoadAsset<UnityEngine::Shader>(SECURE("rainbow.shader"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Shader"))));

							if (material->shader() != forcefield_shader)
							{
								material->set_shader(forcefield_shader);
							}

							break;
						}
						case 2:
						{
							//if (!forcefield_shader)
							forcefield_shader = force_field_bundle->LoadAsset<UnityEngine::Shader>(SECURE("flat.shader"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Shader"))));

							if (material->shader() != forcefield_shader)
							{
								material->set_shader(forcefield_shader);
							}

							auto Chams_Color = Color{ m_colors.m_visuals.PlayerChams_Color[0], m_colors.m_visuals.PlayerChams_Color[1], m_colors.m_visuals.PlayerChams_Color[2], m_colors.m_visuals.PlayerChams_Color[3] };

							material->SetColor(SECURE("_Color"), Chams_Color.GetUnityColor());

							break;
						}
						case 3:
						{
							//if (!forcefield_shader)
							forcefield_shader = force_field_bundle->LoadAsset<UnityEngine::Shader>(SECURE("hologramshader.shader"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Shader"))));


							if (material->shader() != forcefield_shader)
							{
								material->set_shader(forcefield_shader);
							}

							break;
						}
						}
					}
				}
			}

			if (a1->IsWeapon())
			{


				m_base_projectile = base_projectile;
				m_options.m_options.reload_time = m_base_projectile->reloadTime();

				if (m_options.m_options.AutoReload)
				{
					if (memory::IsAddressValid(base_projectile))
					{


						if (base_projectile->IsA(AssemblyCSharp::BaseProjectile::StaticClass()) && !base_projectile->HasReloadCooldown())
						{
							if (!m_options.m_options.did_reload)
								m_options.m_options.time_since_last_shot = (UnityEngine::Time::get_fixedTime() - m_options.m_options.fixed_time_last_shot);

							if (m_options.m_options.just_shot && (m_options.m_options.time_since_last_shot > 0.2f))
							{
								base_projectile->ServerRPC(SECURE("StartReload"));
								base_projectile->SendSignalBroadcast(RustStructs::Signal::Reload, SECURE(""));
								m_options.m_options.just_shot = false;
							}
							if (m_options.m_options.time_since_last_shot > (base_projectile->reloadTime() - (base_projectile->reloadTime() / 10))
								&& !m_options.m_options.did_reload)
							{
								base_projectile->ServerRPC(SECURE("Reload"));
								m_options.m_options.did_reload = true;
								m_options.m_options.time_since_last_shot = 0;
							}
						}
						else
						{
							m_options.m_options.did_reload = false;
							m_options.m_options.just_shot = false;
							m_options.m_options.fixed_time_last_shot = 0;
							m_options.m_options.time_since_last_shot = 0;
						}
					}
					else
					{
						m_options.m_options.did_reload = false;
						m_options.m_options.just_shot = false;
						m_options.m_options.fixed_time_last_shot = 0;
						m_options.m_options.time_since_last_shot = 0;
					}
				}


				if (m_options.m_options.NoWeaponBob)
				{
					auto ActiveModel = AssemblyCSharp::BaseViewModel::get_ActiveModel();

					if (memory::IsAddressValid(ActiveModel))
					{
						if (memory::IsAddressValid(ActiveModel->bob()))
						{
							ActiveModel->bob()->bobAmountRun() = { 0.f, 0.f, 0.f, 0.f };
							ActiveModel->bob()->bobAmountWalk() = { 0.f, 0.f, 0.f, 0.f };
						}

						if (memory::IsAddressValid(ActiveModel->sway()))
						{
							ActiveModel->sway()->positionalSwaySpeed() = { 0 };
							ActiveModel->sway()->positionalSwayAmount() = { 0 };
							//ActiveModel->sway()->rotationSwayAmount() = { 0 };
							//ActiveModel->sway()->rotationSwaySpeed() = { 0 };
						}

						if (memory::IsAddressValid(ActiveModel->lower()))
						{
							ActiveModel->lower()->lowerOnSprint() = false;
							ActiveModel->lower()->lowerWhenCantAttack() = false;
							ActiveModel->lower()->shouldLower() = false;
						}
					}
				}

				if (base_projectile->IsA(AssemblyCSharp::BaseProjectile::StaticClass()))
				{
					if (const auto RecoilProperties = base_projectile->recoil())
					{
						if (m_options.m_options.NoRecoil)
						{

							if (const auto newRecoilOverride = RecoilProperties->newRecoilOverride())
							{
								newRecoilOverride->recoilYawMin() = 0;
								newRecoilOverride->recoilYawMax() = 0;
								newRecoilOverride->recoilPitchMin() = 0;
								newRecoilOverride->recoilPitchMax() = 0;
								newRecoilOverride->ADSScale() = 0;
								newRecoilOverride->movementPenalty() = 0;
							}
							else
							{
								RecoilProperties->recoilYawMin() = 0;
								RecoilProperties->recoilYawMax() = 0;
								RecoilProperties->recoilPitchMin() = 0;
								RecoilProperties->recoilPitchMax() = 0;
								RecoilProperties->ADSScale() = 0;
								RecoilProperties->movementPenalty() = 0;
							}
						}

						if (m_options.m_options.ForceAutomatic);
						{
							base_projectile->automatic() = true;
						}

						if (m_options.m_options.NoSpread)
						{
							base_projectile->stancePenalty() = 0.f;
							base_projectile->aimconePenalty() = 0.f;
							base_projectile->aimCone() = 0.f;
							base_projectile->hipAimCone() = 0.f;
							base_projectile->aimconePenaltyPerShot() = 0.f;
						}

						if (m_options.m_options.WeaponSpam && UnityEngine::Input::GetKey(m_options.m_options.WeaponSpamKey))
						{
							base_projectile->SendSignalBroadcast(RustStructs::Signal::Attack, SECURE(""));
						}

						if (m_options.m_options.InstantSwitch)
						{
							base_projectile->deployDelay() = 0.f;
						}

						if (m_options.m_options.AnimationDelay)
						{
							base_projectile->animationDelay() = 0.f;
						}
					}
				}

				if (m_options.m_options.HeliBulletQueue && UnityEngine::Input::GetKey(m_options.m_options.InstantKillKey))
				{
					auto HeldItem = a1->ActiveItem();
					if (memory::IsAddressValid(HeldItem))
					{
						auto HeldItemID = HeldItem->info()->itemid();


						if (HeldItemID != -765183617 && HeldItemID != 1965232394 && HeldItemID != 1443579727 && HeldItemID != 884424049
							&& HeldItemID != -75944661 && HeldItemID != 1588298435 && HeldItemID != -778367295
							&& HeldItemID != 795371088 && HeldItemID != -41440462 && HeldItemID != -1367281941 && HeldItemID != 442886268
							&& HeldItemID != -1123473824)
						{
							float nextshot = m_options.m_options.fixed_time_last_shot + base_projectile->repeatDelay();


							static float last_tick = 0;
							static int TotalCallThisTick = 0;

							if (UnityEngine::Time::get_realtimeSinceStartup() - last_tick < 0.0625f)
							{
								++TotalCallThisTick;
							}
							else
							{
								TotalCallThisTick = 0;
								last_tick = UnityEngine::Time::get_realtimeSinceStartup();
							}

							if (TotalCallThisTick <= 5)
							{
								if (!base_projectile->IsA(AssemblyCSharp::BowWeapon::StaticClass()) && !base_projectile->IsA(AssemblyCSharp::CompoundBowWeapon::StaticClass()) && !base_projectile->IsA(AssemblyCSharp::CrossbowWeapon::StaticClass()))
								{
									auto camera = UnityEngine::Camera::get_main();
									if (camera) {
										auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
										if (a1 && AimbotTarget.m_player && AimbotTarget.m_heli)
										{
											if (AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position(), AimbotTarget.m_player->get_bone_transform(19)->get_position()))
											{


												if (base_projectile->primaryMagazine()->contents() > 0)
												{
													auto desyncTime = UnityEngine::Time::get_realtimeSinceStartup() - AssemblyCSharp::LocalPlayer::get_Entity()->lastSentTickTime();
													auto desyncpercentage = (((desyncTime / 0.85f) * 100.0f) + 1.f) / 100;
													if (RPC_Counter3.Calculate() <= m_options.m_options.BulletsQueued)
													{
														for (int i = 0; i < m_options.m_options.BulletsQueued; i++)
														{
															base_projectile->LaunchProjectile();
															base_projectile->primaryMagazine()->contents()--;
															base_projectile->UpdateAmmoDisplay();
															base_projectile->ShotFired();
															base_projectile->DidAttackClientside();

															RPC_Counter3.Increment();
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

				}

				if (m_options.m_options.InstantKill && UnityEngine::Input::GetKey(m_options.m_options.InstantKillKey))
				{


					auto HeldItem = a1->ActiveItem();
					if (memory::IsAddressValid(HeldItem))
					{
						auto HeldItemID = HeldItem->info()->itemid();


						if (HeldItemID != -765183617 && HeldItemID != 1965232394 && HeldItemID != 1443579727 && HeldItemID != 884424049
							&& HeldItemID != -75944661 && HeldItemID != 1588298435 && HeldItemID != -778367295
							&& HeldItemID != 795371088 && HeldItemID != -41440462 && HeldItemID != -1367281941 && HeldItemID != 442886268
							&& HeldItemID != -1123473824)
						{
							float nextshot = m_options.m_options.fixed_time_last_shot + base_projectile->repeatDelay();


							static float last_tick = 0;
							static int TotalCallThisTick = 0;

							if (UnityEngine::Time::get_realtimeSinceStartup() - last_tick < 0.0625f)
							{
								++TotalCallThisTick;
							}
							else
							{
								TotalCallThisTick = 0;
								last_tick = UnityEngine::Time::get_realtimeSinceStartup();
							}

							if (TotalCallThisTick <= 10)
							{
								if (!base_projectile->IsA(AssemblyCSharp::BowWeapon::StaticClass()) && !base_projectile->IsA(AssemblyCSharp::CompoundBowWeapon::StaticClass()) && !base_projectile->IsA(AssemblyCSharp::CrossbowWeapon::StaticClass()))
								{
									auto camera = UnityEngine::Camera::get_main();
									if (camera) {
										auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
										if (a1 && AimbotTarget.m_player && !AimbotTarget.m_heli)
										{
											if (base_projectile->primaryMagazine()->contents() > 0)
											{
												if (m_options.m_options.StartShooting)
												{
													if (AssemblyCSharp::IsVisible(AimbotTarget.m_player->get_bone_transform(48)->get_position(), AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position() + spoofed_eye_pos))
													{
														auto desyncTime = UnityEngine::Time::get_realtimeSinceStartup() - AssemblyCSharp::LocalPlayer::get_Entity()->lastSentTickTime();
														auto desyncpercentage = (((desyncTime / 0.85f) * 100.0f) + 1.f) / 100;
														if (RPC_Counter3.Calculate() <= m_options.m_options.BulletsQueued)
														{
															for (int i = 0; i < m_options.m_options.BulletsQueued; i++)
															{
																base_projectile->LaunchProjectile();
																base_projectile->primaryMagazine()->contents()--;
																base_projectile->UpdateAmmoDisplay();
																base_projectile->ShotFired();
																base_projectile->DidAttackClientside();

																RPC_Counter3.Increment();
															}
														}
													}
													else
													{
														if (AssemblyCSharp::IsVisible(AimbotTarget.m_player->get_bone_transform(48)->get_position(), AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position()))
														{
															if (m_options.m_options.ThickBullet)
															{
																if (RPC_Counter3.Calculate() <= m_options.m_options.BulletsQueued)
																{
																	for (int i = 0; i < m_options.m_options.BulletsQueued; i++)
																	{
																		base_projectile->LaunchProjectile();
																		base_projectile->primaryMagazine()->contents()--;
																		base_projectile->UpdateAmmoDisplay();
																		base_projectile->ShotFired();
																		base_projectile->DidAttackClientside();
																		RPC_Counter3.Increment();
																	}
																}
															}
														}
													}
												}
												else if (m_options.m_options.Thickbullet_AutoShoot)
												{
													if (AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position() + spoofed_eye_pos, targetpos))
													{
														auto desyncTime = UnityEngine::Time::get_realtimeSinceStartup() - AssemblyCSharp::LocalPlayer::get_Entity()->lastSentTickTime();
														auto desyncpercentage = (((desyncTime / 0.85f) * 100.0f) + 1.f) / 100;

														if (m_options.m_options.ThickBullet)
														{
															if (RPC_Counter3.Calculate() <= m_options.m_options.BulletsQueued)
															{
																for (int i = 0; i < m_options.m_options.BulletsQueued; i++)
																{
																	base_projectile->LaunchProjectile();
																	base_projectile->primaryMagazine()->contents()--;
																	base_projectile->UpdateAmmoDisplay();
																	base_projectile->ShotFired();
																	base_projectile->DidAttackClientside();
																	a1->SendClientTick();
																	RPC_Counter3.Increment();

																}
															}
														}
													}
													else
													{
														if (AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position(), targetpos))
														{
															if (m_options.m_options.ThickBullet)
															{
																if (RPC_Counter3.Calculate() <= m_options.m_options.BulletsQueued)
																{
																	for (int i = 0; i < m_options.m_options.BulletsQueued; i++)
																	{
																		base_projectile->LaunchProjectile();
																		base_projectile->primaryMagazine()->contents()--;
																		base_projectile->UpdateAmmoDisplay();
																		base_projectile->ShotFired();
																		base_projectile->DidAttackClientside();
																		RPC_Counter3.Increment();

																	}
																}
															}
														}
													}

												}
												else
												{
													if (AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position() + spoofed_eye_pos, targetpos))
													{
														auto desyncTime = UnityEngine::Time::get_realtimeSinceStartup() - AssemblyCSharp::LocalPlayer::get_Entity()->lastSentTickTime();
														auto desyncpercentage = (((desyncTime / 0.85f) * 100.0f) + 1.f) / 100;

														if (m_options.m_options.ThickBullet)
														{
															if (RPC_Counter3.Calculate() <= m_options.m_options.BulletsQueued)
															{
																for (int i = 0; i < m_options.m_options.BulletsQueued; i++)
																{
																	base_projectile->LaunchProjectile();
																	base_projectile->primaryMagazine()->contents()--;
																	base_projectile->UpdateAmmoDisplay();
																	base_projectile->ShotFired();
																	base_projectile->DidAttackClientside();
																	a1->SendClientTick();
																	RPC_Counter3.Increment();

																}
															}
														}
													}
												}

											}
										}
										else
										{
											RPC_Counter3.Reset();
										}
									}
								}
							}



						}
					}
				}




				if (m_options.m_options.InstantBow)
				{
					if (base_projectile->IsA(AssemblyCSharp::BowWeapon::StaticClass()))
					{

						if (auto BowWeapon = reinterpret_cast<AssemblyCSharp::BowWeapon*>(base_projectile))
						{
							BowWeapon->attackReady() = true;
							BowWeapon->arrowBack() = 1.f;
						}
					}
				}

				if (m_options.m_options.InstantCompoundCharge)
				{
					if (base_projectile->IsA(AssemblyCSharp::CompoundBowWeapon::StaticClass()))
					{
						if (const auto CompoundBowWeapon = reinterpret_cast<AssemblyCSharp::CompoundBowWeapon*>(base_projectile))
						{
							CompoundBowWeapon->stringHoldDurationMax() = 0.f;
						}
					}
				}

				if (m_options.m_options.NoMovementPenalty)
				{
					if (base_projectile->IsA(AssemblyCSharp::CompoundBowWeapon::StaticClass()))
					{
						if (auto CompoundBowWeapon = reinterpret_cast<AssemblyCSharp::CompoundBowWeapon*>(base_projectile))
						{
							CompoundBowWeapon->movementPenalty() = 0.f;
						}
					}
				}
			}
			else
			{


				if (m_options.m_options.SilentMelee)
				{
					if (memory::IsAddressValid(base_projectile))
					{
						auto camera = UnityEngine::Camera::get_main();
						if (memory::IsAddressValid(camera))
						{
							auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
							if (memory::IsAddressValid(m_target.m_player))
							{

								if (base_projectile->class_name_hash() == HASH("BaseMelee") || base_projectile->class_name_hash() == HASH("JackHammer"))
								{

									if (AssemblyCSharp::IsVisible(AssemblyCSharp::LocalPlayer::get_Entity()->eyes()->get_position(), m_target.m_player->get_bone_transform(48)->get_position()))
									{
										auto world_position = m_target.m_player->get_bone_transform(48)->get_position();
										auto local = AssemblyCSharp::LocalPlayer::get_Entity()->ClosestPoint(world_position);
										auto camera = AssemblyCSharp::LocalPlayer::get_Entity()->get_bone_transform(48)->get_position();

										if (camera.get_3d_dist(world_position) <= 4.5f)
										{
											DoMeleeAttack(m_target, (AssemblyCSharp::BaseMelee*)base_projectile, true);
										}
									}


								}
							}
						}
					}
				}

				if (m_options.m_options.HammerSpam && UnityEngine::Input::GetKey(m_options.m_options.HammerSpamKey))
				{
					auto camera = UnityEngine::Camera::get_main();
					if (camera) {
						auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
						if (a1 && AimbotTarget.m_player)
						{
							auto DistanceToPlayer = AimbotTarget.m_position.Distance(a1->get_positionz());

							if (DistanceToPlayer < 5.f)
							{
								if (base_projectile->IsA(AssemblyCSharp::Hammer::StaticClass()))
								{
									auto hammer = reinterpret_cast<AssemblyCSharp::Hammer*>(base_projectile);

									auto hit_test_class = CIl2Cpp::FindClass(SECURE(""), SECURE("HitTest"));
									if (!hit_test_class)
										return;

									AssemblyCSharp::HitTest* hit_test = (AssemblyCSharp::HitTest*)CIl2Cpp::il2cpp_object_new((void*)hit_test_class);
									if (hit_test)
									{
										hit_test->MaxDistance() = 1000.f;
										hit_test->HitTransform() = AimbotTarget.m_player->get_bone_transform(47);
										hit_test->AttackRay() = UnityEngine::Ray(a1->eyes()->get_position(), (AimbotTarget.m_position - a1->eyes()->get_position()).Normalized());
										hit_test->DidHit() = true;
										hit_test->HitEntity() = AimbotTarget.m_player;

										if (m_options.m_options.SelectedSpamMaterial == 0)
										{
											hit_test->HitMaterial() = CIl2Cpp::il2cpp_string_new(SECURE("Glass"));
										}
										else if (m_options.m_options.SelectedSpamMaterial == 1)
										{
											hit_test->HitMaterial() = CIl2Cpp::il2cpp_string_new(SECURE("Water"));
										}

										hit_test->HitPoint() = AimbotTarget.m_player->model()->eyeBone()->InverseTransformPoint(AimbotTarget.m_player->model()->eyeBone()->get_position() + Vector3(UnityEngine::Random::Range(-0.2f, 0.2f), UnityEngine::Random::Range(-0.2f, 0.2f), UnityEngine::Random::Range(-0.2f, 0.2f)));
										hit_test->HitNormal() = AimbotTarget.m_player->model()->eyeBone()->InverseTransformPoint(AimbotTarget.m_player->model()->eyeBone()->get_position() + Vector3(UnityEngine::Random::Range(-0.2f, 0.2f), UnityEngine::Random::Range(-0.2f, 0.2f), UnityEngine::Random::Range(-0.2f, 0.2f)));
										hit_test->damageProperties() = hammer->damageProperties();


										hammer->StartAttackCooldown(0.f);
										hammer->ProcessAttack(hit_test);

									}
								}
							}
						}

					}
				}

				if (m_options.m_options.InstantHeal)
				{

					float nextActionTime = 0, period = 1.4721;
					auto health = a1->_health();

					auto Held = a1->GetHeldItemSafe();

					if (memory::IsAddressValid(Held))
					{
						auto HeldEntity = Held->heldEntity();
						if (memory::IsAddressValid(HeldEntity))
						{
							if (!win32::strcmp(HeldEntity->class_name(), SECURE("MedicalTool"))) {
								//auto medical = reinterpret_cast<AssemblyCSharp::MedicalTool*>(m_base_projectile);
								auto Time = UnityEngine::Time::get_time();

								if (base_projectile->timeSinceDeploy() > base_projectile->deployDelay() && base_projectile->nextAttackTime() <= Time) {
									if (Time > nextActionTime) {
										nextActionTime = Time + period;
										if (health < 99)
											base_projectile->ServerRPC(SECURE("UseSelf"));
									}
								}
							}
						}
					}

				}
			}
		}

		if (m_aimbot.m_aimbot.Manipulation && m_options.m_options.Manipulation_Dome)
		{
			auto EyePos = a1->eyes()->get_position();

			UnityEngine::DDraw::Sphere(EyePos, spoofed_eye_pos.x, Color::Red(), 0.3f, true);
		}
	}

	UnityEngine::Physics::IgnoreLayerCollision(4, 12, !m_options.m_options.WalkOnWater);
	UnityEngine::Physics::IgnoreLayerCollision(30, 12, m_options.m_options.WalkThroughTrees);
	UnityEngine::Physics::IgnoreLayerCollision(11, 12, m_options.m_options.WalkThroughPlayers);


	if (const auto LocalMovement = a1->movement())
	{
		if (m_options.m_options.Spiderman)
		{
			LocalMovement->groundAngle() = 0.f;
			LocalMovement->groundAngleNew() = 0.f;
		}

		
		

		
		
		
		
		
		
		if (m_options.m_options.InfiniteJump)
		{
			LocalMovement->landTime() = 0.f;
			LocalMovement->jumpTime() = 0.f;
			LocalMovement->groundTime() = 100000.f;
		}

		if (const auto ModelState = a1->modelState())
		{
			if (m_options.m_options.CanAttack)
			{

				ModelState->set_flag(RustStructs::ModelState_Flag::OnGround);
				a1->GetBaseMovement()->set_Grounded(1.f);

			}
		}

		bool Reset_capsuleHeight = false;
		if (m_options.m_options.HighWalk)
		{
			LocalMovement->capsuleHeight() = 5.5f;
		}
		else
			Reset_capsuleHeight = true;

		if (Reset_capsuleHeight)
			LocalMovement->capsuleHeight() = 1.8f;

		if (m_options.m_options.ToggleSprint)
		{
			LocalMovement->sprintForced() = true;
		}

		if (m_options.m_options.HoldVelocity)
		{
			if (UnityEngine::Input::GetKey(m_options.m_options.HoldVelocityKey))
			{
				LocalMovement->groundAngleNew() = 0.f;
				LocalMovement->groundAngle() = 0.f;
				LocalMovement->gravityMultiplier() = 0.f;
				LocalMovement->flying() = 1.f;
			}
			else
			{
				LocalMovement->gravityMultiplier() = 2.5f;
				LocalMovement->flying() = 0.f;
			}

		}
	}


	if (m_options.m_options.InstantRevive && UnityEngine::Input::GetKey(m_options.m_options.InstantReviveKey))
	{
		auto camera = UnityEngine::Camera::get_main();
		if (memory::IsAddressValid(camera))
		{
			auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
			if (memory::IsAddressValid(m_target.m_player))
			{
				m_target.m_player->ServerRPC(SECURE("RPC_Assist"));
			}

		}
	}

	if (m_options.m_options.KeepAlive && UnityEngine::Input::GetKey(m_options.m_options.KeepAliveKey))
	{
		auto camera = UnityEngine::Camera::get_main();
		if (memory::IsAddressValid(camera))
		{
			auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
			if (memory::IsAddressValid(m_target.m_player))
			{
				m_target.m_player->ServerRPC(SECURE("RPC_KeepAlive"));
			}
		}
	}

	if (m_options.m_options.AutoUnhideStashes)
	{
		f_object entity = f_object::get_closest_object(a1->get_bone_transform(47)->get_position(),
			SECURE("/small stash/"),
			Vector3(),
			Vector3(),
			Vector3(),
			false
		);
		if (entity.valid) {
			Vector3 local = a1->ClosestPoint(entity.position);
			if (local.get_3d_dist(entity.position) <= 3.f) {
				auto entity_cast = reinterpret_cast<AssemblyCSharp::BaseEntity*>(entity.entity);
				if (memory::IsAddressValid(entity_cast))
				{
					entity_cast->ServerRPC(SECURE("RPC_WantsUnhide"));
				}
			}
		}
	}

	if (m_options.m_options.LongMelee)
	{
		auto BaseMelee = AssemblyCSharp::LocalPlayer::get_Entity()->GetHeldEntityCast<AssemblyCSharp::BaseMelee>();

		if (BaseMelee->class_name_hash() == HASH("BaseMelee") || BaseMelee->class_name_hash() == HASH("JackHammer"))
		{
			if (memory::IsAddressValid(BaseMelee))
				BaseMelee->maxDistance() = 4.f;
		}
	}

	if (m_options.m_options.PlayerFov || m_options.m_options.Zoom)
	{
		auto g_graphics_ = CIl2Cpp::FindClass(SECURE("ConVar"), SECURE("Graphics"));
		auto instance = std::uint64_t(g_graphics_->static_fields);
		bool zooming = false;

		if (m_options.m_options.Zoom && UnityEngine::Input::GetKey(m_options.m_options.ZoomKey))
			zooming = true;
		else
			zooming = false;

		if (zooming)
			*(float*)(instance + 0x18) = m_options.m_options.ZoomAmount;
		else if (!zooming && m_options.m_options.PlayerFov)
			*(float*)(instance + 0x18) = m_options.m_options.FieldOfView;
		else
			*(float*)(instance + 0x18) = 90.f;
	}

	if (m_options.m_options.TimeChanger)
	{
		auto g_admin_ = CIl2Cpp::FindClass(SECURE("ConVar"), SECURE("Admin"));
		auto instance = std::uint64_t(g_admin_->static_fields);

		*(float*)(instance + 0x0) = m_options.m_options.Time;
	}

	if (m_options.m_options.ChatSpam && UnityEngine::Input::GetKey(m_options.m_options.ChatSpamKey))
	{
		AssemblyCSharp::ConsoleNetwork::ClientRunOnServer(SECURE(L"chat.say N1gg4sInParis"));
	}

	if (m_options.m_options.SignPainter && UnityEngine::Input::GetKey(m_options.m_options.SignPainterKey))
	{
		//AssemblyCSharp::ConsoleNetwork::ClientRunOnServer(SECURE(L"entity.uploadsign C:/image.png"));
		AssemblyCSharp::ConsoleSystem::Run(AssemblyCSharp::ConsoleSystem::client(), SECURE("entity.uploadsign C:/image.png"), nullptr);
	}

	if (m_options.m_options.AdminFlags)
	{
		a1->playerFlags() |= RustStructs::PlayerFlags::IsAdmin;
	}

	if (m_options.m_options.Speedhack)
	{
		if (UnityEngine::Input::GetKey(m_options.m_options.SpeedhackKey))
		{
			AssemblyCSharp::LocalPlayer::get_Entity()->clothingMoveSpeedReduction() = -m_options.m_options.SpeedhackSpeed;
		}
		else
			AssemblyCSharp::LocalPlayer::get_Entity()->clothingMoveSpeedReduction() = 0.05f;
	}

	if (m_options.m_options.Suicide && UnityEngine::Input::GetKey(m_options.m_options.SuicideKey))
		a1->OnLand(-8.0001f - 100);

	if (m_options.m_options.NoMovementSlowdown)
	{
		a1->clothingBlocksAiming() = false;
	}

	if (m_options.m_options.AttackWhilstMounted)
	{
		if (memory::IsAddressValid(a1->mounted()))
		{
			a1->mounted()->canWieldItems() = true;
		}
	}
	
	if (m_options.m_options.AdminCheat)
	{
		if (UnityEngine::Input::GetKey(m_options.m_options.AdminCheatKey))
			a1->GetBaseMovement()->adminCheat() = true;
		else
			a1->GetBaseMovement()->adminCheat() = false;
	}

	if (m_options.m_options.MovementLines)
	{
		auto MColor = Color{ m_colors.m_visuals.MovementLinesColor[0], m_colors.m_visuals.MovementLinesColor[1], m_colors.m_visuals.MovementLinesColor[2], m_colors.m_visuals.MovementLinesColor[3] };


		if (const auto playerModel = a1->playerModel())
		{
			auto PlayerPos = playerModel->position();

			if (const auto LastSentTick = a1->lastSentTick())
			{
				auto LastSentTickPos = LastSentTick->position();

				UnityEngine::DDraw().Line(PlayerPos, LastSentTickPos, MColor, 1.5f, true, false);
			}
			
		}
	}

	//auto Held = a1->GetHeldItemSafe();

	//if (memory::IsAddressValid(Held))
	//{
	//	auto BaseEntity = Held->GetHeldEntity();

	//	if (memory::IsAddressValid(BaseEntity))
	//	{
	//		BaseEntity->skinID() = 1419392688;
	//		Held->skin() = 1419392688;
	//	}
	//}

	if (m_aimbot.m_aimbot.MemoryAimbot)
	{
		if (UnityEngine::Input::GetKey(m_aimbot.m_aimbot.MemoryAimKey))
		{
			auto camera = UnityEngine::Camera::get_main();
			if (camera) {
				if (a1->IsWeapon())
				{
					auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
					if (AimbotTarget.m_player)
					{
						//auto Base_Projectile = Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>();
						//auto AmmoType = Base_Projectile->primaryMagazine();
						//AssemblyCSharp::ItemModProjectile* itemModProjectile = AmmoType->GetComponent<AssemblyCSharp::ItemModProjectile>(System::Type::ItemModProjectile());
						//AssemblyCSharp::Projectile* Projectile = itemModProjectile->projectileObject()->Get()->GetComponent<AssemblyCSharp::Projectile>(System::Type::Projectile());

						//Vector3 a;
						//float travel_time = 0.f;
						//SimulateProjectile(a1, camera->get_positionz(), AimbotTarget.m_position, m_aim_angle, a, travel_time, Projectile, Base_Projectile, itemModProjectile, AimbotTarget.m_velocity);

						//auto LocalPlayer = AssemblyCSharp::LocalPlayer::get_Entity();
						//Vector2 NewAngle = CalcAngle(camera->get_positionz(), AimbotTarget.m_player->get_bone_transform(47)->get_position());
						////Vector2 LocalVa = LocalPlayer->GetVA();
						//Vector2 LocalVa = LocalPlayer->input()->bodyAngles();
						//Vector2 Offset = NewAngle - LocalVa;
						////Vector2 RecoilAng = LocalPlayer->GetRA();
						//Vector2 RecoilAng = LocalPlayer->input()->recoilAngles();

						//Normalize(Offset.y, Offset.x);

						//Vector2 AngleToAim = LocalVa + Offset;
						//AngleToAim = AngleToAim - RecoilAng;

						//Normalize(AngleToAim.y, AngleToAim.x);
						//LocalPlayer->input()->bodyAngles() = AngleToAim;
						
						auto LocalPlayer = AssemblyCSharp::LocalPlayer::get_Entity();
						auto NewAngle = CalcAngle(LocalPlayer->get_bone_transform(47)->get_position(), AimbotTarget.m_player->get_bone_transform(46)->get_position());
						auto LocalVa = LocalPlayer->input()->bodyAngles();
						auto Offset = NewAngle - LocalVa;
						auto RecoilAng = LocalPlayer->input()->recoilAngles();

						//Offset.Normalize();
						Normalize(Offset.y, Offset.x);

						auto AngleToAim = LocalVa + Offset;
						AngleToAim = AngleToAim - RecoilAng;

						//AngleToAim.Normalize();
						Normalize(AngleToAim.y, AngleToAim.x);

						LocalPlayer->input()->bodyAngles() = AngleToAim;

						//LocalPlayer->input()->bodyAngles() = CalculateAngle(camera->get_positionz(), AimbotTarget.m_player->get_bone_transform(46)->get_position());
					}
				}
			}
		}
	}


	///manipulation///

	float timeSinceLastTick = (UnityEngine::Time::get_realtimeSinceStartup() - a1->lastSentTickTime());
	float last_tick_time = maxx(0.f, minm(timeSinceLastTick, 1.f));
	m_options.m_options.last_tick_time = last_tick_time;

	float num = 1.5f;
	float eye_clientframes = 2.0f;
	float eye_serverframes = 2.0f;
	float num2 = eye_clientframes / 60.f;
	float num3 = eye_serverframes * Max3(UnityEngine::Time::get_deltaTime(), UnityEngine::Time::get_smoothDeltaTime(), UnityEngine::Time::get_fixedDeltaTime());
	float num4 = (last_tick_time + num2 + num3) * num;


	float num5 = a1->MaxEyeVelocity() + a1->GetParentVelocity().Magnitude();
	float num6 = a1->BoundsPadding() + num4 * num5;

	m_options.m_options.max_spoofed_eye_distance = num6;

	if (m_options.m_options.AutoShootType == 0)
	{
		if (m_aimbot.m_aimbot.Manipulation && UnityEngine::Input::GetKey(m_aimbot.m_aimbot.ManipKey))
		{
			a1->clientTickInterval() = .99f;
		}
		else {
			a1->clientTickInterval() = 0.05f;
		}

		if (m_aimbot.m_aimbot.Manipulation && UnityEngine::Input::GetKey(m_aimbot.m_aimbot.ManipKey))
		{
			auto camera = UnityEngine::Camera::get_main();
			if (camera) {
				auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
				if (a1 && AimbotTarget.m_player) {
					GenerateManipulationAngles(a1, AimbotTarget.m_player);
				}
				else
				{
					m_options.m_options.can_manipulate = false;
					m_options.m_options.StartShooting = false;
					m_options.m_options.Manipulation_Indicator = false;
				}
			}
		}
		else if (!spoofed_eye_pos.IsZero()) {
			spoofed_eye_pos = Vector3(0, 0, 0);
			m_options.m_options.can_manipulate = false;
			m_options.m_options.StartShooting = false;
			m_options.m_options.Manipulation_Indicator = false;
		}
	}
	else if (m_options.m_options.AutoShootType == 1)
	{

		auto playerInput = a1->input();

		if (playerInput)
		{
			auto inputState = playerInput->state();

			if (playerInput)
			{
				//auto state = inputState-();
				//if (state)
				{


					if (!inputState->IsDown(RustStructs::FORWARD)
						&& !inputState->IsDown(RustStructs::BACKWARD)
						&& !inputState->IsDown(RustStructs::LEFT)
						&& !inputState->IsDown(RustStructs::RIGHT))
					{
						a1->clientTickInterval() = .99f;

						if (m_aimbot.m_aimbot.Manipulation)
						{
							auto camera = UnityEngine::Camera::get_main();
							if (camera) {
								auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
								if (a1 && AimbotTarget.m_player) {
									GenerateManipulationAngles(a1, AimbotTarget.m_player);
								}
								else
								{
									m_options.m_options.can_manipulate = false;
									m_options.m_options.StartShooting = false;
									m_options.m_options.Manipulation_Indicator = false;
								}
							}
						}
						else if (!spoofed_eye_pos.IsZero()) {
							spoofed_eye_pos = Vector3(0, 0, 0);
							m_options.m_options.can_manipulate = false;
							m_options.m_options.StartShooting = false;
							m_options.m_options.Manipulation_Indicator = false;
						}
					}
					else
					{
						a1->clientTickInterval() = 0.05f;
					}					
				}
			}
		}
	}
	///manipulation///

	///generate fatbullet angles///

	if (m_options.m_options.ThickBullet)
	{
		auto camera = UnityEngine::Camera::get_main();
		if (camera) {
			auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
			if (m_target.m_player && !m_target.m_heli) {
				NewBulletTPChecks(a1, m_target.m_player, m_target.m_position);
			}
			else
			{
				targetpos = m_target.m_position;
				bullettppos = Vector3(0, 0, 0);
				m_options.m_options.Thickbullet_Indicator = false;
				m_options.m_options.Thickbullet_AutoShoot = false;
			}
		}
	
	}
	else if (!bullettppos.IsZero()) {
		bullettppos = Vector3(0, 0, 0);
		m_options.m_options.Thickbullet_Indicator = false;
		m_options.m_options.Thickbullet_AutoShoot = false;
	}


	if (m_options.m_options.FixDebugCamera)
	{
		AssemblyCSharp::ConsoleSystem::Run(AssemblyCSharp::ConsoleSystem::client(), SECURE("client.camspeed 1"), nullptr);
	}


	//if (m_options.m_options.ReportPlayer && UnityEngine::Input::GetKey(m_options.m_options.ReportPlayerKey))
	//{
	//	auto camera = UnityEngine::Camera::get_main();
	//	if (camera) {
	//		auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
	//		if (m_target.m_player) {
	//			char buf[64] = { 0 };
	//			crt_sprintf_s(buf, 64, SECURE("client.reportplayer %ls"), m_target.m_player->UserIDString()->c_str());
	//			AssemblyCSharp::ConsoleSystem::Run(AssemblyCSharp::ConsoleSystem::client(), buf, nullptr);
	//		}
	//	}

	//	//AssemblyCSharp::ConsoleSystem::Run(AssemblyCSharp::ConsoleSystem::client(), "version", nullptr)
	//}

	///fat bullet angles end//


	if (m_aimbot.m_aimbot.SilentAim && m_options.m_options.pSilentType == 1)
	{
		auto camera = UnityEngine::Camera::get_main();
		if (camera) {
			auto m_target = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500.f);
			if (m_target.m_player) {
				auto BaseProjectile = Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>();
				auto PrimaryMagazine = BaseProjectile->primaryMagazine();
				auto AmmoType = PrimaryMagazine->ammoType();
				AssemblyCSharp::ItemModProjectile* itemModProjectile = AmmoType->GetComponent<AssemblyCSharp::ItemModProjectile>((System::Type*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE(""), SECURE("ItemModProjectile"))));

				Vector3 Local = a1->get_bone_transform(RustStructs::bones::head)->get_position();

				Vector3 aim_angle = GetAimDirectionToTarget(a1, BaseProjectile, targetpos, m_target.m_velocity, itemModProjectile, Local);

				auto posnormal = (aim_angle - Local).Normalized();
				Vector4 toQuat = Vector4::QuaternionLookRotation(posnormal, Vector3(0, 1, 0));
				a1->eyes()->SetBodyRotation(toQuat);

			}

		}
	}

	Hooks::ClientInputhk.get_original< decltype(&ClientInput)>()(a1, a2);

	//far hear
	//a1->playerFlags() |= 8388608;
	

	if (m_options.m_options.Spinbot)
	{
		Vector3 remain;
		remain.x = 100;
		remain.y = my_rand() % 999 + -999;
		remain.z = 100;

		if (const auto aimAngles = a2)
		{
			if (const auto CurrentAimAngle = aimAngles->current())
			{
				CurrentAimAngle->aimAngles() = remain;
			}
		}
	}
}

namespace Mathf 
{
	static float Clamp01(float value)
	{
		bool flag = value < 0.f;
		float result;
		if (flag)
		{
			result = 0.f;
		}
		else
		{
			bool flag2 = value > 1.f;
			if (flag2)
			{
				result = 1.f;
			}
			else
			{
				result = value;
			}
		}
		return result;
	}

	static float InverseLerp(float a, float b, float value)
	{
		bool flag = a != b;
		float result;
		if (flag)
		{
			result = Mathf::Clamp01((value - a) / (b - a));
		}
		else
		{
			result = 0.f;
		}
		return result;
	}
}

float Distance(Vector3 a, Vector3 b)
{
	float num = a.x - b.x;
	float num2 = a.y - b.y;
	float num3 = a.z - b.z;

	return (float)win32::sqrt((double)(num * num + num2 * num2 + num3 * num3));
}

float EngagementRange()
{
	float AttackRange = 1.f;
	float AttackRangeMultiplier = 1.f;

	return AttackRange * AttackRangeMultiplier;
}

//#define OFFSET_CONSOLE_SYSTEM_COMMAND_TYPEINFO 50078568
//#define OFFSET_CONSOLE_SYSTEM_COMMAND_NAME 0x10
//#define OFFSET_CONSOLE_SYSTEM_COMMAND_ALLOWRUNFROMSERVER 0x57

//void CheckAndDisallowDangerousCommands(AssemblyCSharp::BasePlayer* player, uintptr_t command)
//{
//	uintptr_t pCommandName;
//	//MEMORY::Read((PVOID)make_ptr(command, OFFSET_CONSOLE_SYSTEM_COMMAND_NAME), &pCommandName, sizeof(pCommandName));
//	pCommandName = memory::Read<uintptr_t>(command + OFFSET_CONSOLE_SYSTEM_COMMAND_NAME);
//	if (!memory::IsAddressValid(pCommandName)) return;
//
//	wchar_t command_name[36] = { '\0' };
//	//MEMORY::Read((PVOID)make_ptr(pCommandName, 0x14), &command_name, sizeof(command_name));
//	//command_name = memory::Read<wchar_t>(pCommandName + 0x14);
//
//	//List of dangerous commands:
//	auto command_NoClip = SECURE(L"noclip");
//	auto command_CamSpeed = SECURE(L"camspeed");
//	auto command_sysInfo = SECURE(L"global.sysinfo");
//	auto command_sysUid = SECURE(L"global.sysuid");
//	auto command_god = SECURE(L"god");
//
//	//Is the indexed command dangerous?
//	bool isDangerousCommand = win32::wcscmp(command_name, command_NoClip) == 0;
//
//	if (isDangerousCommand)
//	{
//		bool RunFromServer = false;
//		//MEMORY::Write(&RunFromServer, (PVOID)make_ptr(command, OFFSET_CONSOLE_SYSTEM_COMMAND_ALLOWRUNFROMSERVER), sizeof(RunFromServer));
//		//RunFromServer = memory::Write<uintptr_t>(command + OFFSET_CONSOLE_SYSTEM_COMMAND_ALLOWRUNFROMSERVER);
//		printfz(SECURE("Disallowed command [%S] from running from the server!"), command_name);
//	}
//
//	//null terminate the whole copied string (or rtlzeromemory if you prefer).
//	for (int i = 0; i < 36; i++) command_name[i] = '\0';
//}
//
//void BlockCommands(AssemblyCSharp::BasePlayer* player)
//{
//
//	uintptr_t ConsoleSystemIndex;
//	//MEMORY::Read((PVOID)make_ptr(GA, OFFSET_CONSOLE_SYSTEM_COMMAND_TYPEINFO), &ConsoleSystemIndex, sizeof(ConsoleSystemIndex));
//	ConsoleSystemIndex = memory::Read<uintptr_t>(m_game_assembly + OFFSET_CONSOLE_SYSTEM_COMMAND_TYPEINFO);
//	if (!memory::IsAddressValid(ConsoleSystemIndex)) return;
//	//printfz(SECURE("Console System Index => %llx"), ConsoleSystemIndex);
//
//	uintptr_t ConsoleSystemIndex_StaticFields;
//	//MEMORY::Read((PVOID)make_ptr(ConsoleSystemIndex, 0xB8), &ConsoleSystemIndex_StaticFields, sizeof(ConsoleSystemIndex_StaticFields));
//	ConsoleSystemIndex_StaticFields = memory::Read<uintptr_t>(ConsoleSystemIndex + 0xB8);
//	if (!memory::IsAddressValid(ConsoleSystemIndex_StaticFields)) return;
//	//printfz(SECURE("Console System Index Static Fields => %llx"), ConsoleSystemIndex_StaticFields);
//
//	uintptr_t ConsoleSystemCommands;
//	//MEMORY::Read((PVOID)make_ptr(ConsoleSystemIndex_StaticFields, 0x10), &ConsoleSystemCommands, sizeof(ConsoleSystemCommands));
//	ConsoleSystemCommands = memory::Read<uintptr_t>(ConsoleSystemIndex_StaticFields + 0x10);
//	if (!memory::IsAddressValid(ConsoleSystemCommands)) return;
//
//	for (int x = 0; x < 500; x++) {
//		uintptr_t x_command;
//		//MEMORY::Read((PVOID)make_ptr(ConsoleSystemCommands, x * 0x8), &x_command, sizeof(x_command));
//		x_command = memory::Read<uintptr_t>(ConsoleSystemCommands + x * 0x8);
//		if (!memory::IsAddressValid(x_command)) continue;
//		CheckAndDisallowDangerousCommands(player, x_command);
//	}
//}