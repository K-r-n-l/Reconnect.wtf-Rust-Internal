#include "Visuals.h"
#include "../Settings/Settings.hpp"
#include <array>
#include "../Menu/Menu.h"

inline std::array<int, 20> valid_bones = {
		1, 2, 3, 5, 6, 14, 15, 17, 18, 21, 23, 24, 25, 26, 27, 48, 55, 56, 57, 76
};




struct box_bounds {
	float left, right, top, bottom;

	bool empty() {
		if (this->left == 0 && this->right == 0 && this->top == 0 && this->bottom == 0)
			return true;

		if (this->left == FLT_MAX || this->right == FLT_MIN || this->top == FLT_MAX || this->bottom == FLT_MIN)
			return true;

		return false;
	}
	static box_bounds null() {
		return { 0, 0, 0, 0 };
	}
};


inline box_bounds get_bounds(AssemblyCSharp::BasePlayer* player, float expand = 0) {
	box_bounds ret = { FLT_MAX, FLT_MIN, FLT_MAX, FLT_MIN };


	const auto bone_transforms = player->model()->boneTransforms();
	if (!bone_transforms)
		return ret;

	for (auto j : valid_bones) {

		const auto bone = bone_transforms->m_Items[j];

		if (!bone)
			continue;

		auto transform = bone;
		if (transform) {
			Vector2 sc;
			auto world_pos = transform->get_position();

			if (j == 48)
				world_pos.y += 0.2f;

			if (UnityEngine::WorldToScreen(world_pos, sc))
			{
				Vector2 bone_screen = sc;

				if (bone_screen.x < ret.left)
					ret.left = bone_screen.x;
				if (bone_screen.x > ret.right)
					ret.right = bone_screen.x;
				if (bone_screen.y < ret.top)
					ret.top = bone_screen.y;
				if (bone_screen.y > ret.bottom)
					ret.bottom = bone_screen.y;
			}
		}
	}

	if (ret.left == FLT_MAX)
		return box_bounds::null();
	if (ret.right == FLT_MIN)
		return box_bounds::null();
	if (ret.top == FLT_MAX)
		return box_bounds::null();
	if (ret.bottom == FLT_MIN)
		return box_bounds::null();

	ret.left -= expand;
	ret.right += expand;
	ret.top -= expand;
	ret.bottom += expand;

	return ret;
};
bool ComputeBox(AssemblyCSharp::BasePlayer* base_player, Vector4& box)
{

	const auto transform = base_player->get_transform();

	if (!memory::IsAddressValid(transform))
	{
		return false;
	}

	const auto& bounds = base_player->bounds();

	const auto center = bounds.m_Center;
	const auto extents = bounds.m_Extents;

	const auto mins = -extents;
	const auto maxs = extents;

	Vector3 corners[8] =
	{
		{ mins[0], mins[1], mins[2] },
		{ mins[0], maxs[1], mins[2] },
		{ maxs[0], maxs[1], mins[2] },
		{ maxs[0], mins[1], mins[2] },
		{ maxs[0], maxs[1], maxs[2] },
		{ mins[0], maxs[1], maxs[2] },
		{ mins[0], mins[1], maxs[2] },
		{ maxs[0], mins[1], maxs[2] },
	};

	const auto position = transform->get_position();
	const auto angles = transform->get_eulerAngles();

	box.x = UnityEngine::Screen::get_width();
	box.y = UnityEngine::Screen::get_height();
	box.z = -1.f;
	box.w = -1.f;

	const auto yaw = UnityEngine::ToRadians(angles[1]);

	const auto sine = -win32::sin(yaw);
	const auto cosine = win32::cos(yaw);

	for (const auto& corner : corners)
	{
		Vector3 world =
		{
			(position[0] + center[0] + (corner[0] * cosine - corner[2] * sine)),
			(position[1] + center[1] + (corner[1])),
			(position[2] + center[2] + (corner[0] * sine + corner[2] * cosine)),
		};

		Vector3 screen = { };

		if (UnityEngine::WorldToScreen2(world, screen))
		{
			box.x = std::min(box.x, screen.x);
			box.y = std::min(box.y, screen.y);
			box.z = std::max(box.z, screen.x);
			box.w = std::max(box.w, screen.y);
		}
	}

	return true;
}

#define M_PI 3.14159265358979323846

//Radar Function
Vector2 WorldToRadar(Vector3 origin, Vector2 position, INT size, FLOAT rotation)
{
	Vector2 clampRotation
	((float)win32::cosf((rotation * (M_PI / 180.f)) + M_PI),
		-(float)win32::sinf((rotation * (M_PI / 180.f)) + M_PI));

	Vector2 clampCoords
	((origin.x * clampRotation.x + origin.z * clampRotation.y),
		(origin.x * clampRotation.y - origin.z * clampRotation.x));

	INT clampSize
	(size / 2.0f);

	Vector2 bVec
	(-clampCoords.x + position.x + clampSize,
		-clampCoords.y + position.y + clampSize);

	bVec.x = -clampCoords.x + position.x + clampSize;
	bVec.y = -clampCoords.y + position.y + clampSize;

	if (bVec.x > (position.x + size)) bVec.x = (position.x + size);
	else if (bVec.x < (position.x)) bVec.x = position.x;

	if (bVec.y > (position.y + size)) bVec.y = (position.y + size);
	else if (bVec.y < (position.y)) bVec.y = position.y;

	return bVec;
}

void AimbotTarget(AssemblyCSharp::BasePlayer* base_player) {
	AssemblyCSharp::BasePlayer::Target best_target = AssemblyCSharp::BasePlayer::Target();

	if (!memory::IsAddressValid(base_player))
		return;

	if (m_aimbot.m_aimbot.SilentAim)
	{
		if (Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>())
		{
			auto target = AssemblyCSharp::BasePlayer::Target();
			Vector3 pos = Vector3(0.f, 0.f, 0.f);

			if (m_aimbot.m_aimbot.SelectedAimbone == 0)
			{
				pos = base_player->get_bone_transform(RustStructs::bones::head)->get_position();
			}
			else if (m_aimbot.m_aimbot.SelectedAimbone == 1)
			{
				pos = base_player->get_bone_transform(RustStructs::bones::neck)->get_position();
			}
			else if (m_aimbot.m_aimbot.SelectedAimbone == 2)
			{
				pos = base_player->get_bone_transform(RustStructs::bones::spine1)->get_position();
			}
			else if (m_aimbot.m_aimbot.SelectedAimbone == 3)
			{
				switch (my_rand() % 9) {
				case 0: // Head
					pos = base_player->get_bone_transform(RustStructs::bones::head)->get_position();
					break;
				case 1: // pelvis
					pos = base_player->get_bone_transform(RustStructs::bones::pelvis)->get_position();
					break;
				case 2: // r_hip
					pos = base_player->get_bone_transform(RustStructs::bones::r_hip)->get_position();
					break;
				case 3: // r_foot
					pos = base_player->get_bone_transform(RustStructs::bones::r_foot)->get_position();
					break;
				case 4: // spine1
					pos = base_player->get_bone_transform(RustStructs::bones::spine1)->get_position();
					break;
				case 5: // l_hand
					pos = base_player->get_bone_transform(RustStructs::bones::l_hand)->get_position();
					break;
				case 6: // r_upperarm
					pos = base_player->get_bone_transform(RustStructs::bones::r_upperarm)->get_position();
					break;
				case 7: // l_knee
					pos = base_player->get_bone_transform(RustStructs::bones::l_knee)->get_position();
					break;
				case 8: // spine4
					pos = base_player->get_bone_transform(RustStructs::bones::spine4)->get_position();
					break;
				}
			}
			else if (m_aimbot.m_aimbot.SelectedAimbone == 4)
			{
				int bone = AssemblyCSharp::BasePlayer::ClosesestToCrosshair(memory::ToAddress(base_player));
				pos = base_player->get_bone_transform(bone)->get_position();
			}

			target.m_position = pos;
			auto distance = Visuals::m_local_player->get_transform()->get_position().get_3d_dist(target.m_position);
			target.m_distance = distance;

			auto fov = AssemblyCSharp::get_fov(target.m_position);
			target.m_fov = fov;

			target.m_player = base_player;

			if (target < best_target)
				best_target = target;

		}
	}
}

inline void DrawCornerBox(const Vector2& startPoint, const Vector2& endPoint, Color boxColor, float boxThickness)
{
	auto x = startPoint.x;
	auto y = startPoint.y;

	auto width = endPoint.x - startPoint.x;
	auto height = endPoint.y - startPoint.y;

	float ratio = 3.0f;

	auto DrawCornerBox = [&](const int32_t posX, const int32_t posY, const int32_t cornerWidth, const int32_t cornerHeight, const bool rightSide, const bool down, Color cornerColor) -> void
	{
		const auto cornerX = rightSide ? posX - cornerWidth : posX;
		const auto cornerY = down ? posY - cornerHeight : posY;
		const auto cornerW = down && rightSide ? cornerWidth + 1 : cornerWidth;

		UnityEngine::GL::Rectangle(Vector2(cornerX + 1, posY + 1), Vector2(cornerW + 1, 1), cornerColor);
		UnityEngine::GL::Rectangle(Vector2(posX + 1, cornerY + 1), Vector2(1, cornerHeight + 1), cornerColor);
	};

	DrawCornerBox(x, y, width / ratio, width / ratio, false, false, boxColor);
	DrawCornerBox(x, y + height, width / ratio, width / ratio, false, true, boxColor);
	DrawCornerBox(x + width, y, width / ratio, width / ratio, true, false, boxColor);
	DrawCornerBox(x + width, y + height, width / ratio, width / ratio, true, true, boxColor);
}


void Visuals::DrawPlayers()
{
	for (auto CachedEntity : PlayerVectorList)
	{
		auto base_player = CachedEntity.CachedPlayer;
		if (!base_player)
			continue;

		const auto position = base_player->get_positionz();
		bool npc = false;

		if (base_player->IsA(AssemblyCSharp::ScientistNPC::StaticClass()) || base_player->IsA(AssemblyCSharp::GingerbreadNPC::StaticClass()) || base_player->IsA(AssemblyCSharp::NPCPlayer::StaticClass()) || base_player->IsA(AssemblyCSharp::TunnelDweller::StaticClass()) || base_player->IsA(AssemblyCSharp::UnderwaterDweller::StaticClass()) || base_player->IsA(AssemblyCSharp::HTNPlayer::StaticClass()))
		{
			if (!m_visuals.m_visuals.DrawNPC)
				continue;

			npc = true;
		}

		if (base_player->IsLocalPlayer())
			continue;

		if (base_player->wasDead())
			continue;

		if (base_player->IsDead())
			continue;

		if (base_player->IsReceivingSnapshot())
			continue;

		if (base_player->IsSleeper())
		{
			if (!m_visuals.m_visuals.DrawSleepers)
				continue;
		}

		//if (m_local_player->IsSameTeam(base_player))
		//{
		//	if (!m_visuals.m_visuals.DrawTeammates)
		//		continue;
		//}

		//bool Team = false;

		//if (base_player->is_teammate(AssemblyCSharp::LocalPlayer::get_Entity()))
		//{
		//	if (!m_visuals.m_visuals.DrawTeammates)
		//		continue;

		//	Team = true;
		//}

		float RadarSize = m_options.m_options.RadarSize;
		float RadarMid = RadarSize / 2;

		float RadarPosX = m_options.m_options.RadarPosX;
		float RadarPosY = m_options.m_options.RadarPosY;

		Vector2 Angles = AssemblyCSharp::LocalPlayer::get_Entity()->input()->bodyAngles();
		Vector3 LocalDistance = AssemblyCSharp::LocalPlayer::get_Entity()->get_transform()->get_position();
		Vector3 PlayerDistance = base_player->get_transform()->get_position();
		Vector2 RadarPoint = WorldToRadar(PlayerDistance - LocalDistance, Vector2(RadarPosX, RadarPosY), m_options.m_options.RadarSize, Angles.y);

		if (base_player->playerFlags() & RustStructs::PlayerFlags::Wounded && !m_visuals.m_visuals.DrawWounded)
			continue;

		if (!memory::IsAddressValid(base_player->model()))
			continue;

		if (!memory::IsAddressValid(base_player->get_transform()))
			continue;

		AimbotTarget(base_player);

		box_bounds bo = get_bounds(base_player, 0.f);
		if (!bo.empty())
		{

			float box_width = bo.right - bo.left;
			float box_height = bo.bottom - bo.top;

			float half = (bo.right - bo.left) / 2;
			Vector2 footPos = { bo.left + (box_width / 2), bo.bottom };
			Vector2 headPos = { bo.left + (box_width / 2), bo.top - 9.54f };

			float current_health_y = 0;
			auto health = base_player->_health();
			auto max_health = base_player->_maxHealth();
			float draw_health = health;

			if (health > max_health)
			{
				draw_health = max_health;
			}

			current_health_y = (box_height / max_health) * draw_health;

			int yoffset = 8;

			auto IsVisible = [&](Vector3 from) {
				Vector3 head_pos_ = base_player->get_bone_transform(48)->get_position();

				for (auto bone : { 48, 2, 3, 4, 15, 14, 26, 57 }) {
					Vector3 TargetPosition;
					if (bone == 48) TargetPosition = head_pos_;
					else TargetPosition = base_player->get_bone_transform(bone)->get_position();
					if (AssemblyCSharp::IsVisible(from, TargetPosition)) {
						return true;
					}
				}
				return false;
			};
			bool IsWounded = base_player->playerFlags() & RustStructs::PlayerFlags::Wounded;

			auto is_Visible = IsVisible(Visuals::m_camera_position);
			auto is_TeamMate = m_local_player->IsSameTeam(base_player);

			auto VisCheckColor = is_Visible ? Color{ m_colors.m_visuals.VisCheck_Color[0], m_colors.m_visuals.VisCheck_Color[1], m_colors.m_visuals.VisCheck_Color[2], m_colors.m_visuals.VisCheck_Color[3] } : Color{ m_colors.m_visuals.DrawBox_Color[0], m_colors.m_visuals.DrawBox_Color[1], m_colors.m_visuals.DrawBox_Color[2], m_colors.m_visuals.DrawBox_Color[3] };
			auto DrawBox_Color = Color{ m_colors.m_visuals.DrawBox_Color[0], m_colors.m_visuals.DrawBox_Color[1], m_colors.m_visuals.DrawBox_Color[2], m_colors.m_visuals.DrawBox_Color[3] };
			auto Visible_Color = Color{ m_colors.m_visuals.DrawBox_Color[0], m_colors.m_visuals.DrawBox_Color[1], m_colors.m_visuals.DrawBox_Color[2], m_colors.m_visuals.DrawBox_Color[3] };
			auto NameTag_Color = Color{ m_colors.m_visuals.DrawNameTag_Color[0], m_colors.m_visuals.DrawNameTag_Color[1], m_colors.m_visuals.DrawNameTag_Color[2], m_colors.m_visuals.DrawNameTag_Color[3] };
			auto HeldItem_Color = Color{ m_colors.m_visuals.DrawHeldItem_Color[0], m_colors.m_visuals.DrawHeldItem_Color[1], m_colors.m_visuals.DrawHeldItem_Color[2], m_colors.m_visuals.DrawHeldItem_Color[3] };
			auto Safezone_Color = Color{ m_colors.m_visuals.Safezone_Color[0], m_colors.m_visuals.Safezone_Color[1], m_colors.m_visuals.Safezone_Color[2], m_colors.m_visuals.Safezone_Color[3] };
			auto Sleeper_Color = Color{ m_colors.m_visuals.Sleeper_Color[0], m_colors.m_visuals.Sleeper_Color[1], m_colors.m_visuals.Sleeper_Color[2], m_colors.m_visuals.Sleeper_Color[3] };
			auto Distance_Color = Color{ m_colors.m_visuals.DrawDistance_Color[0], m_colors.m_visuals.DrawDistance_Color[1], m_colors.m_visuals.DrawDistance_Color[2], m_colors.m_visuals.DrawDistance_Color[3] };
			auto Scientist_Color = Color{ m_colors.m_visuals.Scientist_color[0], m_colors.m_visuals.Scientist_color[1], m_colors.m_visuals.Scientist_color[2], m_colors.m_visuals.Scientist_color[3] };
			auto Wounded_Color = Color{ m_colors.m_visuals.Wounded_Color[0], m_colors.m_visuals.Wounded_Color[1], m_colors.m_visuals.Wounded_Color[2], m_colors.m_visuals.Wounded_Color[3] };
			auto TeamMate_Color = Color{ m_colors.m_visuals.TeamMates_Color[0], m_colors.m_visuals.TeamMates_Color[1], m_colors.m_visuals.TeamMates_Color[2], m_colors.m_visuals.TeamMates_Color[3] };

			Color BoxColor;

			if (base_player->IsSleeper() && m_visuals.m_visuals.DrawSleepers)
				BoxColor = Sleeper_Color;
			else if (npc)
				BoxColor = Scientist_Color;
			else if (base_player->playerFlags() & RustStructs::PlayerFlags::Wounded && m_visuals.m_visuals.DrawWounded)
				BoxColor = Wounded_Color;
			else if (base_player->playerFlags() & RustStructs::PlayerFlags::SafeZone && m_visuals.m_visuals.DrawSafezoneFlag)
				BoxColor = Safezone_Color;
			else
				BoxColor = m_visuals.m_visuals.VisCheck ? VisCheckColor : DrawBox_Color;

			auto distance = m_local_player->get_transform()->get_position().Distance(base_player->get_transform()->get_position());

			if (m_options.m_options.DrawRadar)
			{
				UnityEngine::GL::Rectangle(Vector2(RadarPoint.x, RadarPoint.y), Vector2(5, 5), BoxColor);
			}
				

			if (distance <= m_options.m_options.max_player_distance)
			{

				if (m_visuals.m_visuals.PeterGriffinMode && !IsWounded)
				{
					if (TestBundlee)
					{
						auto asset = TestBundlee->LoadAsset<UnityEngine::Texture2D>(SECURE("ffa8sjf9an.png"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture"))));

						if (asset)
						{
							UnityEngine::GUI::SetColor(Color::White());
							UnityEngine::GUI::DrawTexture({ bo.left, bo.top,box_width + 5, box_height }, asset);
						}
					}
				}

				if (m_visuals.m_visuals.PeterGriffinMode && IsWounded && m_visuals.m_visuals.DrawWounded)
				{
					if (TestBundlee)
					{
						auto asset = TestBundlee->LoadAsset<UnityEngine::Texture2D>(SECURE("cyknzos0ptoa1.png"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture"))));

						if (asset)
						{
							UnityEngine::GUI::SetColor(Color::White());
							UnityEngine::GUI::DrawTexture({ bo.left, bo.top,box_width + 5, box_height }, asset);
						}
					}
				}


				if (m_visuals.m_visuals.DrawBox)
				{
					UnityEngine::GL::Rectangle(Vector2{ bo.left, bo.top }, Vector2{ box_width, box_height }, BoxColor);
				}

				if (m_visuals.m_visuals.DrawCornerBox)
				{
					DrawCornerBox(Vector2(bo.left, bo.top), Vector2(bo.right, bo.bottom), BoxColor, 0.01f);

				}


				if (m_visuals.m_visuals.BottomHealthBar && !m_visuals.m_visuals.DrawHealthBar)
				{
					const auto width = bo.right - bo.left;
					const auto height = box_height;


					const auto bar_width = width + 1.f;
					const auto bar_health = (bar_width / max_health) * draw_health;

					fill_box_({ bo.left, bo.top - 9.54f , bar_width, 4.f }, Color::Black());
					fill_box_({ bo.left, bo.top - 9.54f , bar_health, 4.f }, Color::Green());

				}

				if (m_visuals.m_visuals.DrawHealthBar)
				{
					//UnityEngine::GL::Rectangle(Vector2{ bo.left - 5.0f, bo.top + 1 }, Vector2{ 2.f, box_height }, Color::Black());
					fill_box_({ bo.left - 5.0f, bo.top , 2.f, box_height }, Color::Black());
					fill_box_({ bo.left - 5.0f, bo.bottom , 2.f, -current_health_y }, Color::Green());
				}

				if (m_visuals.m_visuals.DrawNametag)
				{
					auto PlayerString = base_player->get_displayName();
					if (memory::IsAddressValid(PlayerString))
					{						
						if (npc)
							Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, box_height, SECURE(L"Scientist"), NameTag_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						else
							Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, box_height, PlayerString->c_str(), NameTag_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						yoffset += 13;

						

						//if (m_visuals.m_visuals.DrawDistance)
						//{
						//	char buf[256] = { 0 };
						//	crt_sprintf_s(buf, 256, SECURE("%ls [%dm]"), base_player->get_displayName()->c_str(), (int)distance);

						//	if (npc)
						//		Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, box_height, SECURE(L"Scientist"), NameTag_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						//	else
						//		Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, box_height, ConvertToWideString(buf), NameTag_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						//	yoffset += 13;
						//}
						//else
						//{
						//	if (npc)
						//		Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, box_height, SECURE(L"Scientist"), NameTag_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						//	else
						//		Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, box_height, PlayerString->c_str(), NameTag_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						//	yoffset += 13;

						//}
					}
				}

				if (m_visuals.m_visuals.DrawHeldItem)
				{

					const auto item = base_player->ActiveItem();
					if (memory::IsAddressValid(item))
					{
						auto info = item->info();

						if (memory::IsAddressValid(info))
						{
							auto display_name = info->GetDisplayName(item);
							if (memory::IsAddressValid(display_name))
							{
								Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, 20, display_name->c_str(), HeldItem_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
								yoffset += 13;
							}
						}
					}
				}

				if (m_visuals.m_visuals.DrawSleepers)
				{
					if (base_player->playerFlags() & RustStructs::PlayerFlags::Sleeping)
					{
						Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, 20, SECURE(L"Sleeper"), Sleeper_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						yoffset += 13;
					}
				}

				if (m_visuals.m_visuals.DrawSafezoneFlag)
				{
					if (base_player->playerFlags() & RustStructs::PlayerFlags::SafeZone)
					{
						Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, 20, SECURE(L"SafeZone"), Safezone_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						yoffset += 13;
					}
				}


				if (m_visuals.m_visuals.DrawWounded)
				{
					if (base_player->playerFlags() & RustStructs::PlayerFlags::Wounded)
					{
						Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, 20, SECURE(L"Wounded"), Wounded_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						yoffset += 13;
					}
				}

				if (m_visuals.m_visuals.DrawDistance)
				{
					auto PlayerString = base_player->get_displayName();
					if (memory::IsAddressValid(PlayerString))
					{
						/* Convert text */
						char szValue[64];
						wchar_t wszValue[64];
						itoass(static_cast<float>(distance), szValue, 10);
						AnsiToUnicode(szValue, wszValue);

						Text(footPos.x, footPos.y + yoffset, 80.f + half * 2 + 80.f, box_height, wszValue, Distance_Color.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
						yoffset += 13;
					}
				}

				if (m_options.m_options.DrawPlayerChams)
				{
					auto playerModel = base_player->playerModel();
					if (memory::IsAddressValid(playerModel))
					{
						auto _MultiMesh = playerModel->_multiMesh();
						if (memory::IsAddressValid(_MultiMesh))
						{
							auto renderrr = _MultiMesh->get_Renderers();

							if (memory::IsAddressValid(renderrr))
							{
								auto Renderers_Items = renderrr->_items;
								if (memory::IsAddressValid(Renderers_Items))
								{
									auto Renderers_Size = renderrr->_size;
									if (Renderers_Size) {
										for (int i = 0; i < Renderers_Size; i++) {
											auto MainRenderer = Renderers_Items->m_Items[i];

											if (memory::IsAddressValid(MainRenderer)) {

												auto material = MainRenderer->material();

												if (!memory::IsAddressValid(material))
													continue;

												//Unity_String aTest[] = { Unity_String(SECURE(L"Galaxy")), Unity_String(SECURE(L"Rainbow")),Unity_String(SECURE(L"Flat")) ,Unity_String(SECURE(L"Hologram")),Unity_String(SECURE(L"Visible/NonVisible")) };

												switch (m_options.m_options.selectedChams)
												{
												case 0:
												{

													//if (!forcefield_shader)
													forcefield_shader = force_field_bundle->LoadAsset<UnityEngine::Shader>(SECURE("galaxymaterial.shader"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Shader"))));

													//if (!forcefield_material)
													forcefield_material = force_field_bundle->LoadAsset<UnityEngine::Material>(SECURE("galaxymaterial_03.mat"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material"))));


													if (material->shader() != forcefield_shader)
													{
														MainRenderer->set_material(forcefield_material);
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


												/*if (!forcefield_shader)
													forcefield_shader = force_field_bundle->LoadAsset<UnityEngine::Shader>(SECURE("galaxymaterial.shader"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Shader"))));

												if (!forcefield_material)
													forcefield_material = force_field_bundle->LoadAsset<UnityEngine::Material>(SECURE("galaxymaterial_03.mat"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material"))));*/




													//if (!forcefield_shader)
													//	forcefield_shader = force_field_bundle->LoadAsset<UnityEngine::Shader>(SECURE("hologramshader.shader"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Shader"))));

													//if (!forcefield_material)
													//	forcefield_material = force_field_bundle->LoadAsset<UnityEngine::Material>(SECURE("galaxymaterial_03.mat"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material"))));

													//if (!forcefield_material)
													//	forcefield_material = force_field_bundle->LoadAsset<UnityEngine::Material>(SECURE("new material 1.mat"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material"))));

													//rainbow.shader
													//if (material->shader() != forcefield_shader)
													//{
													//	//MainRenderer->set_material(forcefield_material);
													//	material->set_shader(forcefield_shader);
													//}


													//auto Chams_Color = Color{ m_colors.m_visuals.PlayerChams_Color[0], m_colors.m_visuals.PlayerChams_Color[1], m_colors.m_visuals.PlayerChams_Color[2], m_colors.m_visuals.PlayerChams_Color[3] };

													//forcefield_material->SetColor(SECURE("_Color"), Chams_Color.GetUnityColor());
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


void Visuals::DrawEntities()
{
	for (auto CachedEntity : vector_list)
	{
		auto base_networkable = static_cast<AssemblyCSharp::BaseNetworkable*>(CachedEntity.CachedEntity);
		if (!base_networkable)
			continue;

		//if (base_networkable->IsA(AssemblyCSharp::BasePlayer::StaticClass()))
		//{
		//	
		//}
		//else
		{
			const auto prefab_name = base_networkable->get_ShortPrefabName();

			Vector2 screen = { };

			if (memory::IsAddressValid(prefab_name))
			{
				auto name = prefab_name->c_str();

				if (UnityEngine::WorldToScreen(base_networkable->get_positionz(), screen))
				{
					auto test_plr = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

					Vector2 render_pad(0.f, 0.f);
					Vector4 box = { };
					if (ComputeBox(test_plr, box))
					{
						if (box.z < 180.f || box.z < -180.f)
							continue;

						Vector2 screen(box.x, box.y);

						auto Distance = m_local_player->get_transform()->get_position().Distance(base_networkable->get_transform()->get_position());

						const auto width = box.z - box.x;
						const auto height = box.w - box.y;
						if (m_options.m_options.DroppedItems)
						{
							if (base_networkable->IsA(AssemblyCSharp::WorldItem::StaticClass()))
							{
								auto world_item = static_cast<AssemblyCSharp::WorldItem*>(base_networkable);

								if (world_item)
								{
									auto item = world_item->item();
									if (item)
									{
										auto name = item->GetItemName();
										if (name->c_str())
										{
											auto StashColor = Color{ m_colors.m_visuals.DroppedItemColor[0], m_colors.m_visuals.DroppedItemColor[1], m_colors.m_visuals.DroppedItemColor[2], m_colors.m_visuals.DroppedItemColor[3] };

											Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, name->c_str(), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										}
									}
								}
							}
						}

						if (m_options.m_options.Workbench)
						{

							auto prefabname = base_networkable->get_ShortPrefabName();

							if (prefabname)
							{
								if (base_networkable->IsA(AssemblyCSharp::Workbench::StaticClass()))
								{
									auto StashColor = Color{ m_colors.m_visuals.DroppedItemColor[0], m_colors.m_visuals.DroppedItemColor[1], m_colors.m_visuals.DroppedItemColor[2], m_colors.m_visuals.DroppedItemColor[3] };

									Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, prefabname->c_str(), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
								}
							}
						}

					
						if (Distance <= m_options.m_options.max_food_distance)
						{

							if (m_options.m_options.mushroom)
							{
								if (base_networkable->IsA(AssemblyCSharp::CollectibleEntity::StaticClass()))
								{
									if (HASH("mushroom-cluster-6") == constant::Hash(name, false) || HASH("mushroom-cluster-5") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.MushRoomColor[0], m_colors.m_visuals.MushRoomColor[1], m_colors.m_visuals.MushRoomColor[2], m_colors.m_visuals.MushRoomColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Mushroom"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}

							if (m_options.m_options.pumpkin)
							{
								if (base_networkable->IsA(AssemblyCSharp::CollectibleEntity::StaticClass()))
								{
									if (HASH("pumpkin-collectable") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.PumpkinColor[0], m_colors.m_visuals.PumpkinColor[1], m_colors.m_visuals.PumpkinColor[2], m_colors.m_visuals.PumpkinColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Pumpkin"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}

							if (m_options.m_options.corn)
							{
								if (base_networkable->IsA(AssemblyCSharp::CollectibleEntity::StaticClass()))
								{
									if (HASH("corn-collectable") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.CornColor[0], m_colors.m_visuals.CornColor[1], m_colors.m_visuals.CornColor[2], m_colors.m_visuals.CornColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Corn"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}
						}

						if (Distance <= m_options.m_options.max_playerdrops_distance)
						{
							if (m_options.m_options.Backpack)
							{
								if (base_networkable->IsA(AssemblyCSharp::DroppedItemContainer::StaticClass()))
								{
									auto itemContainer = static_cast<AssemblyCSharp::DroppedItemContainer*>(base_networkable);
									if (!itemContainer)
										continue;

									auto _playerName = itemContainer->_playerName();


									if (HASH("item_drop_backpack") == constant::Hash(name, false))
									{

										auto StashColor = Color{ m_colors.m_visuals.BackPackColor[0], m_colors.m_visuals.BackPackColor[1], m_colors.m_visuals.BackPackColor[2], m_colors.m_visuals.BackPackColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Backpack"), StashColor.GetUnityColor(), true, true, m_colors.m_visuals.fontSize);
									}
								}

							}

							if (m_options.m_options.PlayerCorpse)
							{
								if (base_networkable->IsA(AssemblyCSharp::PlayerCorpse::StaticClass()))
								{
									if (HASH("player_corpse") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.CorpseColor[0], m_colors.m_visuals.CorpseColor[1], m_colors.m_visuals.CorpseColor[2], m_colors.m_visuals.CorpseColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Player Corpse"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}

							}
						}

						if (Distance <= m_options.m_options.max_animal_distance)
						{
							if (m_options.m_options.horse)
							{
								if (base_networkable->IsA(AssemblyCSharp::RidableHorse::StaticClass()))
								{
									if (HASH("testridablehorse") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.HorseColor[0], m_colors.m_visuals.HorseColor[1], m_colors.m_visuals.HorseColor[2], m_colors.m_visuals.HorseColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Horse"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}

							}

							if (m_options.m_options.Bear)
							{
								if (base_networkable->IsA(AssemblyCSharp::Bear::StaticClass()))
								{
									auto resource_entity = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

									if (HASH("bear") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.BearColor[0], m_colors.m_visuals.BearColor[1], m_colors.m_visuals.BearColor[2], m_colors.m_visuals.BearColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Bear"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto health = resource_entity->_health();
										const auto health_max = resource_entity->_maxHealth();

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

									}
								}
							}

							//if (HASH("TimedExplosive") == constant::Hash(name, false))
							//{
							//	std::string raid = "Raid : C4 : " + std::to_string(int(Distance)) + "M";
							//	auto StashColor = Color{ m_colors.m_visuals.ChickenColor[0], m_colors.m_visuals.ChickenColor[1], m_colors.m_visuals.ChickenColor[2], m_colors.m_visuals.ChickenColor[3] };

							//	TextTest(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, raid, StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
							//}

							if (m_options.m_options.chicken)
							{
								if (base_networkable->IsA(AssemblyCSharp::Chicken::StaticClass()))
								{
									auto resource_entity = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

									if (HASH("chicken") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.ChickenColor[0], m_colors.m_visuals.ChickenColor[1], m_colors.m_visuals.ChickenColor[2], m_colors.m_visuals.ChickenColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Chicken"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto health = resource_entity->_health();
										const auto health_max = resource_entity->_maxHealth();

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

									}
								}
							}

							if (m_options.m_options.Wolf)
							{
								if (base_networkable->IsA(AssemblyCSharp::Wolf::StaticClass()))
								{
									auto resource_entity = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

									if (HASH("wolf") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.WolfColor[0], m_colors.m_visuals.WolfColor[1], m_colors.m_visuals.WolfColor[2], m_colors.m_visuals.WolfColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Wolf"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto health = resource_entity->_health();
										const auto health_max = resource_entity->_maxHealth();

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

									}
								}
							}

							if (m_options.m_options.boar)
							{
								if (base_networkable->IsA(AssemblyCSharp::Boar::StaticClass()))
								{
									auto resource_entity = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

									if (HASH("boar") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.BoarColor[0], m_colors.m_visuals.BoarColor[1], m_colors.m_visuals.BoarColor[2], m_colors.m_visuals.BoarColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Boar"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto health = resource_entity->_health();
										const auto health_max = resource_entity->_maxHealth();

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

									}
								}
							}

							if (m_options.m_options.Deer)
							{
								if (base_networkable->IsA(AssemblyCSharp::Stag::StaticClass()))
								{
									auto resource_entity = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

									if (HASH("stag") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.DeerColor[0], m_colors.m_visuals.DeerColor[1], m_colors.m_visuals.DeerColor[2], m_colors.m_visuals.DeerColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Deer"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto health = resource_entity->_health();
										const auto health_max = resource_entity->_maxHealth();

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

									}
								}
							}

							if (m_options.m_options.polarbear)
							{
								if (base_networkable->IsA(AssemblyCSharp::Polarbear::StaticClass()))
								{
									auto resource_entity = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

									if (HASH("polarbear") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.PolarBearColor[0], m_colors.m_visuals.PolarBearColor[1], m_colors.m_visuals.PolarBearColor[2], m_colors.m_visuals.PolarBearColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Polar Bear"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto health = resource_entity->_health();
										const auto health_max = resource_entity->_maxHealth();

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

									}
								}
							}
						}

						if (Distance <= m_options.m_options.max_crates_distance)
						{

							if (m_options.m_options.BasicCrate)
							{
								if (base_networkable->IsA(AssemblyCSharp::LootContainer::StaticClass()))
								{
									if (HASH("crate_basic") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.BasicCrateColor[0], m_colors.m_visuals.BasicCrateColor[1], m_colors.m_visuals.BasicCrateColor[2], m_colors.m_visuals.BasicCrateColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Basic Crate"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}


							if (m_options.m_options.NormalCrate)
							{
								if (base_networkable->IsA(AssemblyCSharp::LootContainer::StaticClass()))
								{
									if (HASH("crate_normal_2") == constant::Hash(name, false) || HASH("crate_normal_2_food") == constant::Hash(name, false) || HASH("crate_normal_2_medical") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.NormalCrateColor[0], m_colors.m_visuals.NormalCrateColor[1], m_colors.m_visuals.NormalCrateColor[2], m_colors.m_visuals.NormalCrateColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Normal Crate"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}

							if (m_options.m_options.MilitaryCrate)
							{
								if (base_networkable->IsA(AssemblyCSharp::LootContainer::StaticClass()))
								{
									if (HASH("crate_normal") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.MilitaryCrateColor[0], m_colors.m_visuals.MilitaryCrateColor[1], m_colors.m_visuals.MilitaryCrateColor[2], m_colors.m_visuals.MilitaryCrateColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Military Crate"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}

							if (m_options.m_options.EliteCrate)
							{
								if (base_networkable->IsA(AssemblyCSharp::LootContainer::StaticClass()))
								{
									if (HASH("crate_elite") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.EliteCrateColor[0], m_colors.m_visuals.EliteCrateColor[1], m_colors.m_visuals.EliteCrateColor[2], m_colors.m_visuals.EliteCrateColor[3] };
										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Elite Crate"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}

							if (m_options.m_options.Airdrop)
							{
								if (base_networkable->IsA(AssemblyCSharp::LootContainer::StaticClass()))
								{
									if (HASH("supply_drop") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.SupplyDropColor[0], m_colors.m_visuals.SupplyDropColor[1], m_colors.m_visuals.SupplyDropColor[2], m_colors.m_visuals.SupplyDropColor[3] };
										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Airdrop"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}

							if (m_options.m_options.LootBarrel)
							{
								if (HASH("loot-barrel-1") == constant::Hash(name, false) || HASH("loot-barrel-2") == constant::Hash(name, false))
								{
									auto StashColor = Color{ m_colors.m_visuals.LootBarrelColor[0], m_colors.m_visuals.LootBarrelColor[1], m_colors.m_visuals.LootBarrelColor[2], m_colors.m_visuals.LootBarrelColor[3] };
									Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Barrel"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
								}
							}

							if (m_options.m_options.OilBarrel)
							{
								if (HASH("oil_barrel") == constant::Hash(name, false))
								{
									auto StashColor = Color{ m_colors.m_visuals.OilBarrelColor[0], m_colors.m_visuals.OilBarrelColor[1], m_colors.m_visuals.OilBarrelColor[2], m_colors.m_visuals.OilBarrelColor[3] };
									Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Oil Barrel"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
								}
							}

							if (m_options.m_options.DieselBarrel)
							{
								if (HASH("diesel_collectable") == constant::Hash(name, false))
								{
									auto StashColor = Color{ m_colors.m_visuals.DieselBarrelColor[0], m_colors.m_visuals.DieselBarrelColor[1], m_colors.m_visuals.DieselBarrelColor[2], m_colors.m_visuals.DieselBarrelColor[3] };
									Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Diesel Barrel"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
								}
							}

							if (m_options.m_options.HeliCrate)
							{
								if (base_networkable->IsA(AssemblyCSharp::LootContainer::StaticClass()))
								{
									if (HASH("heli_crate") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.HeliCrateColor[0], m_colors.m_visuals.HeliCrateColor[1], m_colors.m_visuals.HeliCrateColor[2], m_colors.m_visuals.HeliCrateColor[3] };
										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Heli Crate"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}

							if (m_options.m_options.BradleyCrate)
							{
								if (base_networkable->IsA(AssemblyCSharp::LootContainer::StaticClass()))
								{
									if (HASH("bradley_crate") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.BradleyCrateColor[0], m_colors.m_visuals.BradleyCrateColor[1], m_colors.m_visuals.BradleyCrateColor[2], m_colors.m_visuals.BradleyCrateColor[3] };
										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Bradley Crate"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
									}
								}
							}
							if (m_options.m_options.HackableCrate)
							{
								if (base_networkable->IsA(AssemblyCSharp::HackableLockedCrate::StaticClass()))
								{
									auto hackablecrate = reinterpret_cast<AssemblyCSharp::HackableLockedCrate*>(base_networkable);

									if (HASH("codelockedhackablecrate") == constant::Hash(name, false) || HASH("codelockedhackablecrate_oilrig") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.HackableCrateColor[0], m_colors.m_visuals.HackableCrateColor[1], m_colors.m_visuals.HackableCrateColor[2], m_colors.m_visuals.HackableCrateColor[3] };
										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 7.f, 200.f, 20, SECURE(L"Hackable Crate"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto bar_width = width + 1.f;

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}


										auto Timer = hackablecrate->requiredHackSeconds();
										if (memory::IsAddressValid(Timer))
										{
											/* Convert text */
											char szValue[4096];
											wchar_t wszValue[4096];
											itoass(static_cast<float>(Timer), szValue, 10);
											AnsiToUnicode(szValue, wszValue);


											Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 18.f, 200.f, 20, wszValue, StashColor, true,true,  m_colors.m_visuals.fontSize);
											render_pad[1] += 13;
										}

									}
								}
							}
						}

						if (Distance <= m_options.m_options.max_apc_distance)
						{
							if (m_options.m_options.BradleyAPC)
							{
								if (base_networkable->IsA(AssemblyCSharp::BradleyAPC::StaticClass()))
								{
									auto resource_entity = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

									auto StashColor = Color{ m_colors.m_visuals.BradleyColor[0], m_colors.m_visuals.BradleyColor[1], m_colors.m_visuals.BradleyColor[2], m_colors.m_visuals.BradleyColor[3] };

									Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Bradley"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

									const auto health = resource_entity->_health();
									const auto health_max = resource_entity->_maxHealth();

									const auto bar_width = width + 1.f;
									const auto bar_health = (bar_width * health) / health_max;

									auto bar_color = Color::Green();
									if (health > 400.f && health < 300.f)
									{
										bar_color = Color::Orange();
									}
									else if (health < 20.f)
									{
										bar_color = Color::Red();
									}

									if (m_options.m_options.DrawHealthWorld)
									{
										fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
										fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
									}

									if (m_options.m_options.DrawBoxesWorld)
									{
										outline_box_(screen, Vector2(width, height), Color::White());
									}

									if (m_options.m_options.BradChams)
									{
										auto ChamsColor = Color{ m_colors.m_visuals.BradChamsColor[0], m_colors.m_visuals.BradChamsColor[1], m_colors.m_visuals.BradChamsColor[2], m_colors.m_visuals.BradChamsColor[3] };

										auto g_render = base_networkable->GetComponentsInChildren(System::Type::Renderer());
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

												auto g_shader = UnityEngine::Shader::Find(SECURE("Hidden/Internal-Colored"));

												if (!memory::IsAddressValid(g_shader))
													continue;

												if (g_shader != material->shader())
												{
													material->set_shader(g_shader);
												}

												material->SetInt(SECURE("_ZTest"), 8);
												material->SetColor(SECURE("_Color"), ChamsColor.GetUnityColor());
											}
										}

									}
								}
							}
						}

						if (Distance <= m_options.m_options.max_traps_distance)
						{

							if (m_options.m_options.AutoTurret)
							{
								if (base_networkable->IsA(AssemblyCSharp::AutoTurret::StaticClass()))
								{
									if (HASH("autoturret_deployed") == constant::Hash(name, false))
									{
										auto resource_entity = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

										auto StashColor = Color{ m_colors.m_visuals.TurretColor[0], m_colors.m_visuals.TurretColor[1], m_colors.m_visuals.TurretColor[2], m_colors.m_visuals.TurretColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Auto Turret"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto health = resource_entity->_health();
										const auto health_max = resource_entity->_maxHealth();

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

									}

									//if (m_options.m_options.AutoTurretRange)
									//{
									//	auto resource_entity = static_cast<AssemblyCSharp::AutoTurret*>(base_networkable);
									//	auto LocalPlayer = Visuals::m_local_player->get_transform()->get_position();
									//	
									//	float center_x = (float)(screen_size.x) / 2, center_y = (float)(screen_size.y) / 2;

									//	auto camera = Camera::get_main();

									//	auto cam = camera->get_positionz().get_3d_dist(LocalPlayer);

									//	if (cam < resource_entity->sightRange() + 10)
									//		Text(center_x, center_y - 50, 20, 20, SECURE(L"Close To Turret"), Color::Red(), true,true,  m_colors.m_visuals.fontSize.f);
									//}

								}


							}

							if (m_options.m_options.ShotgunTrap)
							{
								if (base_networkable->IsA(AssemblyCSharp::GunTrap::StaticClass()))
								{
									if (HASH("guntrap.deployed") == constant::Hash(name, false))
									{
										auto resource_entity = static_cast<AssemblyCSharp::BasePlayer*>(base_networkable);

										auto StashColor = Color{ m_colors.m_visuals.ShotgunColor[0], m_colors.m_visuals.ShotgunColor[1], m_colors.m_visuals.ShotgunColor[2], m_colors.m_visuals.ShotgunColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Shotgun Trap"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto health = resource_entity->_health();
										const auto health_max = resource_entity->_maxHealth();

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}
									}

								}
							}
						}

						if (base_networkable->IsA(AssemblyCSharp::ResourceEntity::StaticClass()))
						{

							auto resource_entity = static_cast<AssemblyCSharp::ResourceEntity*>(base_networkable);

							auto distance = m_local_player->get_transform()->get_position().Distance(base_networkable->get_transform()->get_position());

							if (distance <= m_options.m_options.Max_Ore_Distance)
							{
								if (m_options.m_options.SulfurOre)
								{
									if (HASH("sulfur-ore") == constant::Hash(name, false) || HASH("ore_sulfur") == constant::Hash(name, false) || HASH("sulfur-collectable.prefab") == constant::Hash(name, false))
									{

										auto StashColor = Color{ m_colors.m_visuals.SulfurColor[0], m_colors.m_visuals.SulfurColor[1], m_colors.m_visuals.SulfurColor[2], m_colors.m_visuals.SulfurColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Sulfur Ore"), StashColor.GetUnityColor(), true, true, 11);

										/* Convert text */
										char szValue[64];
										wchar_t wszValue[64];
										itoass(static_cast<float>(distance), szValue, 10);
										AnsiToUnicode(szValue, wszValue);

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 20.f, 200.f, 20, wszValue, Color::White(), true, true, 11);
																							

										const auto health = resource_entity->health();
										const auto health_max = 500.f;

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.SulfurIcon)
										{
											UnityEngine::Texture2D* texture = nullptr;
											if (!texture)
											{
												texture = MenuIconBundles->LoadAsset<UnityEngine::Texture2D>(SECURE("sulfur.ore.png"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture"))));
											}

											UnityEngine::GUI::SetColor(Color::White());
											UnityEngine::GUI::DrawTexture({ screen[0] + (width * 0.5f) - 15.f, screen[1] + height + render_pad[1] - 35.f, 30.f, 30.f }, texture);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

										if (m_options.m_options.SulfurChams)
										{
											auto ChamsColor = Color{ m_colors.m_visuals.SulfurChamsColor[0], m_colors.m_visuals.SulfurChamsColor[1], m_colors.m_visuals.SulfurChamsColor[2], m_colors.m_visuals.SulfurChamsColor[3] };

											auto g_render = base_networkable->GetComponentsInChildren(System::Type::Renderer());
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

													auto g_shader = UnityEngine::Shader::Find(SECURE("Hidden/Internal-Colored"));

													if (!memory::IsAddressValid(g_shader))
														continue;

													if (g_shader != material->shader())
													{
														material->set_shader(g_shader);
													}
													material->SetInt(SECURE("_ZTest"), 8);
													material->SetColor(SECURE("_Color"), ChamsColor.GetUnityColor());

												}
											}

										}
									}
								}

								if (m_options.m_options.StoneOre)
								{
									if (HASH("stone-ore") == constant::Hash(name, false) || HASH("ore_stone") == constant::Hash(name, false) || HASH("stone-collectable.prefab") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.StoneColor[0], m_colors.m_visuals.StoneColor[1], m_colors.m_visuals.StoneColor[2], m_colors.m_visuals.StoneColor[3] };
										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Stone Ore"), StashColor.GetUnityColor(), true, true, 11);

										/* Convert text */
										char szValue[64];
										wchar_t wszValue[64];
										itoass(static_cast<float>(distance), szValue, 10);
										AnsiToUnicode(szValue, wszValue);

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 20.f, 200.f, 20, wszValue, Color::White(), true, true, 11);

										const auto health = resource_entity->health();
										const auto health_max = 500.f;

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
										}

										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

										if (m_options.m_options.StoneIcon)
										{
											UnityEngine::Texture2D* texture = nullptr;
											if (!texture)
											{
												texture = MenuIconBundles->LoadAsset<UnityEngine::Texture2D>(SECURE("stones.png"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture"))));
											}

											UnityEngine::GUI::SetColor(Color::White());
											UnityEngine::GUI::DrawTexture({ screen[0] + (width * 0.5f) - 15.f, screen[1] + height + render_pad[1] - 35.f, 30.f, 30.f }, texture);
										}

										if (m_options.m_options.StoneChams)
										{
											auto ChamsColor = Color{ m_colors.m_visuals.StoneChamsColor[0], m_colors.m_visuals.StoneChamsColor[1], m_colors.m_visuals.StoneChamsColor[2], m_colors.m_visuals.StoneChamsColor[3] };

											auto g_render = base_networkable->GetComponentsInChildren(System::Type::Renderer());
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

													auto g_shader = UnityEngine::Shader::Find(SECURE("Hidden/Internal-Colored"));

													if (!memory::IsAddressValid(g_shader))
														continue;

													if (g_shader != material->shader())
													{
														material->set_shader(g_shader);
													}

													material->SetInt(SECURE("_ZTest"), 8);
													material->SetColor(SECURE("_Color"), ChamsColor.GetUnityColor());

												}
											}

										}
									}
								}

								if (m_options.m_options.MetalOre)
								{
									if (HASH("metal-ore") == constant::Hash(name, false) || HASH("ore_metal") == constant::Hash(name, false) || HASH("metal-collectable.prefab") == constant::Hash(name, false))
									{
										auto StashColor = Color{ m_colors.m_visuals.MetalColor[0], m_colors.m_visuals.MetalColor[1], m_colors.m_visuals.MetalColor[2], m_colors.m_visuals.MetalColor[3] };

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Metal Ore"), StashColor.GetUnityColor(), true, true, 11);

										/* Convert text */
										char szValue[64];
										wchar_t wszValue[64];
										itoass(static_cast<float>(distance), szValue, 10);
										AnsiToUnicode(szValue, wszValue);

										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 20.f, 200.f, 20, wszValue, Color::White(), true, true, 11);

										const auto health = resource_entity->health();
										const auto health_max = 500.f;

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 400.f && health < 300.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);

										}

										if (m_options.m_options.MetalIcon)
										{
											UnityEngine::Texture2D* texture = nullptr;
											if (!texture)
											{
												texture = MenuIconBundles->LoadAsset<UnityEngine::Texture2D>(SECURE("metal.ore.png"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture"))));
											}

											UnityEngine::GUI::SetColor(Color::White());
											UnityEngine::GUI::DrawTexture({ screen[0] + (width * 0.5f) - 15.f, screen[1] + height + render_pad[1] - 35.f, 30.f, 30.f }, texture);
										}


										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

										if (m_options.m_options.MetalChams)
										{
											auto ChamsColor = Color{ m_colors.m_visuals.MetalChamsColor[0], m_colors.m_visuals.MetalChamsColor[1], m_colors.m_visuals.MetalChamsColor[2], m_colors.m_visuals.MetalChamsColor[3] };

											auto g_render = base_networkable->GetComponentsInChildren(System::Type::Renderer());
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

													auto g_shader = UnityEngine::Shader::Find(SECURE("Hidden/Internal-Colored"));

													if (!memory::IsAddressValid(g_shader))
														continue;

													if (g_shader != material->shader())
													{
														material->set_shader(g_shader);

													}

													material->SetInt(SECURE("_ZTest"), 8);
													material->SetColor(SECURE("_Color"), ChamsColor.GetUnityColor());
												}
											}

										}
									}
								}
							}
						}

						else if (base_networkable->IsA(AssemblyCSharp::StashContainer::StaticClass()))
						{
							auto entity = static_cast<AssemblyCSharp::BaseEntity*>(base_networkable);
							{
								auto flags = entity->flags();

								if (flags & 2048)
								{

									auto distance = m_local_player->get_transform()->get_position().Distance(base_networkable->get_transform()->get_position());


									if (distance <= m_options.m_options.Max_Stash_Distance)
									{
										if (HASH("small_stash_deployed") == constant::Hash(name, false))
										{
											if (m_options.m_options.StashEsp)
											{
												auto StashColor = Color{ m_colors.m_visuals.StashColor[0], m_colors.m_visuals.StashColor[1], m_colors.m_visuals.StashColor[2], m_colors.m_visuals.StashColor[3] };

												Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Stash"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

												if (m_options.m_options.DrawBoxesWorld)
												{
													outline_box_(screen, Vector2(width, height), Color::White());
												}
											}
										}
									}


									if (m_options.m_options.StashChams)
									{
										auto ChamsColor = Color{ m_colors.m_visuals.StashChamsColor[0], m_colors.m_visuals.StashChamsColor[1], m_colors.m_visuals.StashChamsColor[2], m_colors.m_visuals.StashChamsColor[3] };

										auto g_render = base_networkable->GetComponentsInChildren(System::Type::Renderer());
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

												auto g_shader = UnityEngine::Shader::Find(SECURE("Hidden/Internal-Colored"));

												if (!memory::IsAddressValid(g_shader))
													continue;

												if (g_shader != material->shader())
												{
													material->set_shader(g_shader);
												}

												material->SetInt(SECURE("_ZTest"), 8);
												material->SetColor(SECURE("_Color"), ChamsColor.GetUnityColor());
											}
										}

									}
								}
							}
						}
						else if (base_networkable->IsA(AssemblyCSharp::CollectibleEntity::StaticClass()))
						{
							auto entity = static_cast<AssemblyCSharp::BaseEntity*>(base_networkable);
							{

								auto distance = m_local_player->get_transform()->get_position().Distance(base_networkable->get_transform()->get_position());

								if (distance <= m_options.m_options.max_collectable_distance)
								{
									if (HASH("hemp-collectable") == constant::Hash(name, false))
									{
										if (m_options.m_options.Hemp)
										{
											auto StashColor = Color{ m_colors.m_visuals.HempColor[0], m_colors.m_visuals.HempColor[1], m_colors.m_visuals.HempColor[2], m_colors.m_visuals.HempColor[3] };

											Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Hemp"), StashColor.GetUnityColor(), true, true, 11);

											/* Convert text */
											char szValue[64];
											wchar_t wszValue[64];
											itoass(static_cast<float>(distance), szValue, 10);
											AnsiToUnicode(szValue, wszValue);

											Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 20.f, 200.f, 20, wszValue, Color::White(), true, true, 11);

											if (m_options.m_options.DrawBoxesWorld)
											{
												outline_box_(screen, Vector2(width, height), Color::White());
											}


											if (m_options.m_options.HempIcon)
											{

												UnityEngine::Texture2D* texture = nullptr;
												if (!texture)
												{
													texture = MenuIconBundles->LoadAsset<UnityEngine::Texture2D>(SECURE("clone.hemp.png"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture"))));
												}

												UnityEngine::GUI::SetColor(Color::White());
												UnityEngine::GUI::DrawTexture({ screen[0] + (width * 0.5f) - 15.f, screen[1] + height + render_pad[1] - 35.f, 30.f, 30.f }, texture);
											}
										}

										if (m_options.m_options.HempChams)
										{
											auto ChamsColor = Color{ m_colors.m_visuals.HempChamsColor[0], m_colors.m_visuals.HempChamsColor[1], m_colors.m_visuals.HempChamsColor[2], m_colors.m_visuals.HempChamsColor[3] };

											auto g_render = base_networkable->GetComponentsInChildren(System::Type::Renderer());
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

													auto g_shader = UnityEngine::Shader::Find(SECURE("Hidden/Internal-Colored"));

													if (!memory::IsAddressValid(g_shader))
														continue;

													if (g_shader != material->shader())
													{
														material->set_shader(g_shader);
													}

													material->SetInt(SECURE("_ZTest"), 8);
													material->SetColor(SECURE("_Color"), ChamsColor.GetUnityColor());
												}
											}

										}
									}
								}
							}
						}
						else if (base_networkable->IsA(AssemblyCSharp::BuildingPrivlidge::StaticClass()))
						{
							auto testenbt = static_cast<AssemblyCSharp::BaseCombatEntity*>(base_networkable);

							auto distance = m_local_player->get_transform()->get_position().Distance(base_networkable->get_transform()->get_position());

							if (distance <= m_options.m_options.max_world_distance)
							{

								if (m_options.m_options.ToolCupboard)
								{
									auto building_priv = reinterpret_cast<AssemblyCSharp::BuildingPrivlidge*>(base_networkable);
									if (memory::IsAddressValid(building_priv))
									{
										auto StashColor = Color{ m_colors.m_visuals.MetalColor[0], m_colors.m_visuals.MetalColor[1], m_colors.m_visuals.MetalColor[2], m_colors.m_visuals.MetalColor[3] };


										int cachedUpkeepPeriodMinutes = building_priv->UpKeep();
										int days = cachedUpkeepPeriodMinutes / 1440;
										int testt = building_priv->CalculateUpkeepCostFraction();


										Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Tool Cupboard"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);
										//Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 12.f, 200.f, 20, SECURE(L"Tool Cupboard"), StashColor.GetUnityColor(), true,true,  m_colors.m_visuals.fontSize);

										const auto health = testenbt->_health();
										const auto health_max = testenbt->_maxHealth();

										const auto bar_width = width + 1.f;
										const auto bar_health = (bar_width * health) / health_max;

										auto bar_color = Color::Green();
										if (health > 60.f && health < 80.f)
										{
											bar_color = Color::Orange();
										}
										else if (health < 20.f)
										{
											bar_color = Color::Red();
										}

										if (m_options.m_options.DrawHealthWorld)
										{
											fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
											fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);

										}


										if (m_options.m_options.DrawBoxesWorld)
										{
											outline_box_(screen, Vector2(width, height), Color::White());
										}

										if (m_options.m_options.AuthorizedPlayers)
										{
											const auto authorized_players = building_priv->authorizedPlayers();
											if (memory::IsAddressValid(authorized_players))
											{
												for (auto it = 0; it < authorized_players->_size; it++)
												{
													const auto current = authorized_players->_items->m_Items[it];
													if (memory::IsAddressValid(current))
													{
														const auto str = current->username();
														if (memory::IsAddressValid(str))
														{

															auto distance = m_camera->get_positionz().get_3d_dist(base_networkable->get_positionz());

															if (distance <= m_options.m_options.AuthorizedPlayersDistance)
															{

																Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 18.f, 200.f, 20, str->c_str(), StashColor, true,true,  m_colors.m_visuals.fontSize);
																render_pad[1] += 13;
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
						else if (base_networkable->IsA(AssemblyCSharp::BaseHelicopter::StaticClass()))
						{
							auto distance = m_local_player->get_transform()->get_position().Distance(base_networkable->get_transform()->get_position());

							if (distance <= m_options.m_options.max_apc_distance)
							{

								if (m_options.m_options.DrawAttackHeli)
								{
									if (base_networkable->IsA(AssemblyCSharp::BaseHelicopter::StaticClass()))
									{
										auto base_heli = reinterpret_cast<AssemblyCSharp::BasePlayer*>(base_networkable);
										if (base_heli)
										{

											Text(screen[0] + (width * 0.5f), screen[1] + height + render_pad[1] + 8.f, 200.f, 20, SECURE(L"Attack Helicopter"), Color::White(), true,true,  m_colors.m_visuals.fontSize);

											const auto health = base_heli->_health();
											const auto health_max = base_heli->_maxHealth();

											const auto bar_width = width + 1.f;
											const auto bar_health = (bar_width * health) / health_max;

											auto bar_color = Color::Green();
											if (health > 60.f && health < 80.f)
											{
												bar_color = Color::Orange();
											}
											else if (health < 20.f)
											{
												bar_color = Color::Red();
											}

											if (m_options.m_options.DrawHealthWorld)
											{
												fill_box_({ screen.x - 1.f, screen.y - 8.f , bar_width + 2.f, 6.f }, Color::Black());
												fill_box_({ screen.x, screen.y - 7.f , bar_health, 4.f }, bar_color);
											}


											if (m_options.m_options.DrawBoxesWorld)
											{
												outline_box_(screen, Vector2(width, height), Color::White());
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

void Visuals::CachePlayers()
{
	if (const auto visiblePlayerList = AssemblyCSharp::BasePlayer::visiblePlayerList())
	{
		if (const auto EntityListVals = visiblePlayerList->vals)
		{
			if (const auto EntityListBuffer = EntityListVals->buffer)
			{

				m_local_player = AssemblyCSharp::LocalPlayer::get_Entity();
				if (!m_local_player)
					return;

				m_camera = UnityEngine::Camera::get_main();
				if (!m_camera)
					return;

				m_camera_position = m_camera->get_positionz();
				if (m_camera_position.IsZero())
					return;

				for (std::int32_t index = 0; index < EntityListVals->count; index++)
				{
					auto BasePlayer = EntityListBuffer->m_Items[index];
					if (!BasePlayer)
						continue;

					PlayerVectorListTemp.push_back(PlayerList(static_cast<AssemblyCSharp::BasePlayer*>(BasePlayer)));
				}

				PlayerVectorList = PlayerVectorListTemp;
				PlayerVectorListTemp.clear();
			}
		}
	}
}

void Visuals::CacheEntites()
{
	if (const auto ClientEntites = AssemblyCSharp::BaseNetworkable::clientEntities())
	{
		if (const auto entityList = ClientEntites->entityList())
		{
			if (const auto EntityListVals = entityList->vals)
			{
				if (const auto EntityListBuffer = EntityListVals->buffer)
				{

					m_local_player = AssemblyCSharp::LocalPlayer::get_Entity();
					if (!m_local_player)
						return;

					m_camera = UnityEngine::Camera::get_main();
					if (!m_camera)
						return;

					m_camera_position = m_camera->get_positionz();
					if (m_camera_position.IsZero())
						return;



					for (std::int32_t index = 0; index < EntityListVals->count; index++)
					{
						auto base_networkable = EntityListBuffer->m_Items[index];

						if (memory::IsAddressValid(base_networkable))
						{
							if (base_networkable->IsA(AssemblyCSharp::BaseHelicopter::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::BuildingPrivlidge::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::CollectibleEntity::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::StashContainer::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::ResourceEntity::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::GunTrap::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::AutoTurret::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::BradleyAPC::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::LootContainer::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::RidableHorse::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::Polarbear::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::Stag::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::Bear::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::Wolf::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::Boar::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::PlayerCorpse::StaticClass())		
								|| base_networkable->IsA(AssemblyCSharp::HackableLockedCrate::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::DroppedItemContainer::StaticClass())

								|| base_networkable->IsA(AssemblyCSharp::Workbench::StaticClass())
								//|| base_networkable->IsA(AssemblyCSharp::Recycler::StaticClass())
								//|| base_networkable->IsA(AssemblyCSharp::BuildingBlock::StaticClass())
								|| base_networkable->IsA(AssemblyCSharp::WorldItem::StaticClass()))
							{
								vector_list_Temp.push_back(VectorList(static_cast<AssemblyCSharp::BasePlayer*>(base_networkable)));
							}
							else
							{
								if (m_options.m_options.AutoUpgrade)
								{
									if (base_networkable->IsA(AssemblyCSharp::BuildingBlock::StaticClass()))
									{

										auto block = reinterpret_cast<AssemblyCSharp::BuildingBlock*>(base_networkable);

										if (block)
										{
											auto entity_pos = block->get_positionz();

											if (RPC_Counter.Calculate() >= 5)
											{
												continue;
											}
											
											

											Vector3 local = m_local_player->ClosestPoint(entity_pos);
											if (local.get_3d_dist(entity_pos) <= 5.5f)
											{
												float LastGrade = 0.f;
												if (m_local_player->lastSentTickTime() > LastGrade + 0.35f
													&& block->CanAffordUpgrade((RustStructs::BuildingGrade)m_options.m_options.BuildingGrade, 0, Visuals::m_local_player)
													&& block->CanChangeToGrade((RustStructs::BuildingGrade)m_options.m_options.BuildingGrade, 0, Visuals::m_local_player)
													&& !block->IsUpgradeBlocked()) {
													block->UpgradeToGrade((RustStructs::BuildingGrade)m_options.m_options.BuildingGrade, 0, Visuals::m_local_player);
													RPC_Counter.Increment();
													LastGrade = m_local_player->lastSentTickTime();
												}
											}
											
											

										}
									}
								}


								if (m_options.m_options.DoorSpammer)
								{
									if (base_networkable->IsA(AssemblyCSharp::Door::StaticClass()))
									{
										auto distance = m_local_player->get_transform()->get_position().Distance(base_networkable->get_transform()->get_position());

										auto ent = reinterpret_cast<AssemblyCSharp::Door*>(base_networkable);

										if (distance <= 3.f)
										{
											if (RPC_Counter.Calculate() <= 5)
											{
												ent->KnockDoor(m_local_player);
												RPC_Counter.Increment();
											}
										}

									}
								}

								if (m_options.m_options.DisableRecycler && UnityEngine::Input::GetKey(m_options.m_options.DisableRecyclerKey))
								{
									if (base_networkable->IsA(AssemblyCSharp::Recycler::StaticClass()))
									{
										if (RPC_Counter.Calculate() <= 5)
										{
											auto ent = reinterpret_cast<AssemblyCSharp::BasePlayer*>(base_networkable);
											ent->ServerRPC(SECURE("SVSwitch"));
											RPC_Counter.Increment();
										}
									}
								}


								continue;
							}
						}
					}

					vector_list = vector_list_Temp;
					vector_list_Temp.clear();
				}
			}
		}
	}
	
}