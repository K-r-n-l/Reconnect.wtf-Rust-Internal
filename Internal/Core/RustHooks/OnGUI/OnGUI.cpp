#include "OnGUI.h"
#include "../../Menu/Menu.h"

void DefaultFont()
{
	if (gui_style && gui_skin)
		return;
	
	gui_skin = UnityEngine::GUI::GetSkin();
	gui_style = gui_skin->m_label();


	if (!g_proggy_font_bundle) {

		g_proggy_font_bundle = UnityEngine::AssetBundle::LoadFromFile_Internal(SECURE("C:\\CheatFont.unity3d"), 0, 0);
		if (!g_proggy_font_bundle)
		{
			LI_FIND(MessageBoxA)(0, SECURE("Please put all of the asset files in your C Drive"), SECURE("Bundle Error"), MB_ICONASTERISK);
		}
		
		auto g_font = g_proggy_font_bundle->LoadAsset<uintptr_t>(SECURE("verdana.ttf"), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Font"))));

		gui_skin->m_Font() = memory::ToAddress(g_font);

		gui_style->SetFontSize(9);
	}

	UnityEngine::GUIStyle::SetAlignment(gui_style, 0);
	UnityEngine::GUI::SetColor(Color::White());
}

inline UnityEngine::AssetBundle* test_asst = nullptr;

inline UnityEngine::Material* LineMat;

inline void DrawFov_(Color color, Vector2 Center, float Radius) {
	LineMat = (UnityEngine::Material*)CIl2Cpp::il2cpp_object_new((void*)CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material")));

	UnityEngine::Material::CreateWithShader(LineMat, UnityEngine::Shader::Find((SECURE("Hidden/Internal-Colored"))));
	LineMat->SetInt((SECURE("_SrcBlend")), 5);
	LineMat->SetInt((SECURE("_DstBlend")), 10);
	LineMat->SetInt((SECURE("_Cull")), 0);
	LineMat->SetInt((SECURE("_ZWrite")), 0);

	UnityEngine::GL::PushMatrix();
	LineMat->SetPass(0);
	UnityEngine::GL::Begin(1);
	UnityEngine::GL::Colorz(color.GetUnityColor());
	for (float num = 0.f; num < 6.2831855f; num += 0.05f) {
		UnityEngine::GL::Vertex(Vector3(win32::cos(num) * Radius + Center.x, win32::sin(num) * Radius + Center.y, 0));
		UnityEngine::GL::Vertex(Vector3(win32::cos(num + 0.05f) * Radius + Center.x, win32::sin(num + 0.05f) * Radius + Center.y, 0));
	}
	UnityEngine::GL::End();

	UnityEngine::GL::PopMatrix();
}
inline UnityEngine::Material* draw_material;
void InitGL()
{


	draw_material = (UnityEngine::Material*)CIl2Cpp::il2cpp_object_new((void*)CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material")));
	UnityEngine::Material::CreateWithShader(draw_material, UnityEngine::Shader::Find((SECURE("Hidden/Internal-Colored"))));

	draw_material->SetInt((SECURE("_SrcBlend")), 5);
	draw_material->SetInt((SECURE("_DstBlend")), 10);
	draw_material->SetInt((SECURE("_Cull")), 0);
	draw_material->SetInt((SECURE("_ZWrite")), 0);
	UnityEngine::Object::DontDestroyOnLoad((UnityEngine::Object*)draw_material);
	//printfz("\n draw_material - %p", draw_material);
}
inline Vector2 hotbar_pos;
inline Vector2 window_size2;
inline bool sex2 = false;
inline void DrawPlayerHotbar(UnityEngine::Event* event, const Vector2& pos, const Vector2& window_size) {
	if (m_visuals.m_visuals.DrawPlayerInv)
	{

		if (!sex2) {
			hotbar_pos = pos;
			sex2 = true;
		}

		window_size2 = window_size;



		auto mouse = UnityEngine::Input::GetMousePosition();
		auto width = UnityEngine::Screen::get_width();
		auto height = UnityEngine::Screen::get_height();
		auto menu_event = event->Type();
		auto key_code = UnityEngine::Event::get_keyCode(event);

		mouse_pos.x = UnityEngine::Input::GetMousePosition().x;
		mouse_pos.y = UnityEngine::Screen::get_height() - UnityEngine::Input::GetMousePosition().y;
		auto mouse_state = UnityEngine::Input::GetMouseButton(0);

		if (is_mouse_in_box({ hotbar_pos.x, hotbar_pos.y }, { hotbar_pos.x + window_size2.x, hotbar_pos.y + window_size2.y }) && mouse_state && !hover_element) {
			hotbar_pos.x += mouse_pos.x - old_mouse_pos.x;
			hotbar_pos.y += mouse_pos.y - old_mouse_pos.y;
		}
		else {
			hover_element = false;
		}


		auto camera = Visuals::m_camera;
		if (camera) {
			auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500);
			if (AimbotTarget.m_player)
			{
				auto inventory = AimbotTarget.m_player->inventory();
				if (memory::IsAddressValid(inventory))
				{
					auto containerBelt = inventory->containerBelt();
					if (memory::IsAddressValid(containerBelt))
					{
						auto ItemList = containerBelt->itemList();
						if (memory::IsAddressValid(ItemList))
						{

							if (menu_event == RustStructs::EventType::Repaint)
							{
								/* Its main form */
								fill_box({ hotbar_pos.x, hotbar_pos.y , window_size2.x,window_size2.y }, Color(22.f, 22.f, 22.f, 255.f));
								fill_box({ hotbar_pos.x, hotbar_pos.y , window_size2.x,25 }, Color(28, 28, 28, 255.f));
								//UnityEngine::GL::GradientFillRect({ hotbar_pos.x, hotbar_pos.y }, { window_size.x,25 }, Color(38.f, 40.f, 42.f, 155), Color(22.f, 22.f, 22.f, 55));

								Text(hotbar_pos.x + window_size2.x - window_size2.x / 2 + 2, hotbar_pos.y + 12.5, 200, 20, AimbotTarget.m_player->get_displayName()->c_str(), Color(58.f, 197.f, 129.f, 255.f), true, true);
							}

							float info_y = 0;


							int y = 0;
							for (int i = 0; i < ItemList->_size; i++) {
								auto item = ItemList->_items->m_Items[i];
								if (!item)
									continue;

								if (i > ItemList->_size - 1)
								{
									info_y += 68;
									continue;
								}

								auto amount = item->amount();

								

								auto sprite_the_drink = item->get_iconSprite();
								if (!sprite_the_drink)
									continue;

								auto texture = sprite_the_drink->get_texture();
								if (!texture)
									continue;

								static int cases = 0;
								static float r = 1.00f, g = 0.00f, b = 1.00f;
								switch (cases)
								{
								case 0: { r -= 0.005f; if (r <= 0) cases += 1; break; }
								case 1: { g += 0.005f; b -= 0.005f; if (g >= 1) cases += 1; break; }
								case 2: { r += 0.005f; if (r >= 1) cases += 1; break; }
								case 3: { b += 0.005f; g -= 0.005f; if (b >= 1) cases = 0; break; }
								default: { r = 1.00f; g = 0.00f; b = 1.00f; break; }
								}

								float x = screen_center.x;
								float y = screen_center.y;
								float w = 400.f;
								float h = 5.f;

								//fill_box({ x - w / 2, y - 421, w, 20 }, Color(0.f, 0.f, 0.f, 255.f)); //680, 143, 530, 30

								//fill_box({ x - w / 2 + info_y, y - 421, 60, 60 }, Color(0.f, 0.f, 0.f, 120.f));
								//outline_box(Vector2(x - w / 2 + info_y, y - 421), Vector2(60, 60), Color(r, g, b, 255.f));

								UnityEngine::GUI::SetColor({ 1.f, 1.f, 1.f, 1.f });
								UnityEngine::GUI::DrawTexture({ hotbar_pos.x + info_y + 15, hotbar_pos.y + 30.f, 60, 60 }, texture);

								char szValue[64];
								wchar_t wszValue[64];
								itoass(static_cast<float>(amount), szValue, 10);
								AnsiToUnicode(szValue, wszValue);


								float condition = item->_condition();
								float maxCondition = item->_maxCondition();

								const auto bar_width = 60 + 1.f;
								const auto bar_health = (bar_width * condition) / maxCondition;

								auto bar_color = Color::Green();
							
								fill_box_({ hotbar_pos.x + info_y + 15, hotbar_pos.y + 95.f, bar_health, 4.f }, bar_color);

								//fill_box({ hotbar_pos.x + info_y + 15, hotbar_pos.y + 30.f, 30, condition }, Color::Green());
								//Text(hotbar_pos.x + info_y + 10, hotbar_pos.y + 38.f, 200, 20, SECURE(L"x"), Color(58.f, 197.f, 129.f, 255.f), true, true);
								Text(hotbar_pos.x + info_y + 15, hotbar_pos.y + 38.f, 200, 20, wszValue, Color(58.f, 197.f, 129.f, 255.f), true, true);

								info_y += 68;
							}
							info_y = 0;
						}
					}
				}
			}
		}

		end_window();
	}
}

void ResetTargets()
{
	Visuals::m_local_player = nullptr;
	Visuals::m_camera = nullptr;
	Visuals::m_camera_position = Vector3();
	m_base_projectile = nullptr;

	m_options.m_options.max_flyhack_vertical = 0;
	m_options.m_options.vertical_flyhack = 0;
	m_options.m_options.max_flyhack_horisontal = 0;
	m_options.m_options.horisontal_flyhack = 0;

	m_options.m_options.DrawPlayerChams = false;
	m_options.m_options.did_reload = false;
	m_options.m_options.just_shot = false;
	m_options.m_options.time_since_last_shot = 0.0f;
	m_options.m_options.fixed_time_last_shot = 0.0f;

	m_options.m_options.can_manipulate = false;
	m_options.m_options.StartShooting = false;
	m_options.m_options.Manipulation_Indicator = false;

	RPC_Counter.Reset();
}

void ResetBundles()
{
	force_field_bundle->Unload(true);
	force_field_bundle = nullptr;

	colorchams_bundle->Unload(true);
	colorchams_bundle = nullptr;

	TestBundlee->Unload(true);
	TestBundlee = nullptr;

	peter_texture2 = nullptr;
	peter_texture1 = nullptr;

	forcefield_shader = nullptr;
	forcefield_material = nullptr;

	colorchams_shader = nullptr;
	colorchams_material = nullptr;
	//LineMat = nullptr;
}

void ResetLists()
{
	vector_list.clear();
	vector_list_Temp.clear();

	PlayerVectorList.clear();
	PlayerVectorListTemp.clear();
}

bool IsInGame()
{
	//if (m_force_exit)
	//	return false;

	const auto Network = Network::Net::get_cl();
	
	if (!Network->IsConnected())
	{
		ResetLists();
		ResetBundles();

		const auto LocalPlayer = AssemblyCSharp::LocalPlayer::get_Entity();

		if (memory::IsAddressValid(LocalPlayer))
			LocalPlayer->net() = nullptr;

		ResetTargets();

		return false;
	}

	const auto LocalPlayer = AssemblyCSharp::LocalPlayer::get_Entity();

	if (!memory::IsAddressValid(LocalPlayer) || !memory::IsAddressValid(LocalPlayer->net()))
	{
		ResetTargets();
		ResetLists();
		ResetBundles();
		return false;
	}

	//if (LocalPlayer->IsAlive() || LocalPlayer->IsSleeping() || LocalPlayer->get_isMounted())
	//{
	//	//States::Runtime::Misc::Player::flyhack_kick_disable_time = std::chrono::system_clock::now();
	//}

	return LocalPlayer->IsAlive() && !LocalPlayer->IsSleeping();
}

UnityEngine::AssetBundle* testbndl2 = nullptr;
inline bool niggg = false;

void Hooks::OnGUI(AssemblyCSharp::ExplosionsFPS* a1)
{
	DefaultFont();

	//InitGL();

	white_texture = UnityEngine::Texture2D::get_whiteTexture();

	screen_center = { UnityEngine::Screen::get_width() / 2.f, UnityEngine::Screen::get_height() / 2.f };
	screen_size = { (float)UnityEngine::Screen::get_width(), (float)UnityEngine::Screen::get_height() };

	auto m_Event = UnityEngine::Event::Current();


	if (m_Event->Type() == RustStructs::EventType::KeyDown && UnityEngine::Input::GetKeyDown(RustStructs::KeyCode::Insert))
		m_visuals.m_visuals.ShowMenu = !m_visuals.m_visuals.ShowMenu;

	if (m_Event->Type() != RustStructs::EventType::KeyDown && m_visuals.m_visuals.ShowMenu)
		Menu::RenderMenu(m_Event);

	auto mouse = UnityEngine::Input::GetMousePosition();

	auto width = UnityEngine::Screen::get_width();
	auto height = UnityEngine::Screen::get_height();


	Vector2 menu_size = { 500, 380 } , mouse_pos = { mouse.x, height - mouse.y };

	if (UnityEngine::Input::GetKey(RustStructs::Mouse0)) {
		auto z = UnityEngine::rect_t{ hotbar_pos.x, hotbar_pos.y, menu_size.x, 30 };

		if (z.contains(mouse_pos))
		{
			hotbar_pos = (hotbar_pos + (mouse_pos - hotbar_pos) - Vector2(250, 15));
		}
	}
	float bar_width = 200.f;
	float x = screen_center.x;
	float y = screen_center.y;
	float w = 400.f;
	float h = 5.f;

	if (m_Event->Type() == RustStructs::EventType::Repaint)
	{
		if (IsInGame())
			m_aimbot.m_aimbot.ConnectedToServer = true;
		else
			m_aimbot.m_aimbot.ConnectedToServer = false;

		if (m_aimbot.m_aimbot.ConnectedToServer)
			Text(5.f, 1.f, 200, 20, SECURE(L"Connected To Server"), Color::White(), false, true, 18.f);
		
		if (!m_aimbot.m_aimbot.ConnectedToServer)
			Text(5.f, 1.f, 200, 20, SECURE(L"Disconnected"), Color::White(), false, true, 18.f);


		float RadarSize = m_options.m_options.RadarSize;
		float RadarMid = RadarSize / 2;
		float RadarPosX = m_options.m_options.RadarPosX;
		float RadarPosY = m_options.m_options.RadarPosY;

		float mid_x = RadarPosX + RadarSize / 2;
		float mid_y = RadarPosY + RadarSize / 2;


		auto MouseX = UnityEngine::Input::GetMousePosition().x;
		auto MouseY = UnityEngine::Screen::get_height() - UnityEngine::Input::GetMousePosition().y;

		if (MouseX >= mid_x - RadarSize && MouseX <= mid_x + RadarSize) {
			if (MouseY >= mid_y - RadarSize && MouseY <= mid_y + RadarSize) {
				if (mouse_state && !hover_element) {
					m_options.m_options.RadarPosX = MouseX - (RadarSize / 2);
					m_options.m_options.RadarPosY = MouseY - (RadarSize / 2);
				}
			}
		}

		/* Radar */
		if (m_options.m_options.DrawRadar)
		{
			//MainBox
			fill_box({ m_options.m_options.RadarPosX, m_options.m_options.RadarPosY , RadarSize, RadarSize }, Color(45, 50, 54, 25.f));
			outline_box({ m_options.m_options.RadarPosX, m_options.m_options.RadarPosY }, { RadarSize, RadarSize }, Color(0, 0, 0, 255.f));
			//Lines
			UnityEngine::GL::Line(Vector2(m_options.m_options.RadarPosX + RadarMid, m_options.m_options.RadarPosY + RadarMid), Vector2(RadarPosX, RadarPosY), Color::Black(), 1.f);
			UnityEngine::GL::Line(Vector2(m_options.m_options.RadarPosX + RadarMid, m_options.m_options.RadarPosY + RadarMid), Vector2(RadarPosX + RadarSize, RadarPosY), Color::Black(), 1.f);
			UnityEngine::GL::Line(Vector2(m_options.m_options.RadarPosX + RadarMid, m_options.m_options.RadarPosY + RadarMid), Vector2(RadarPosX + RadarMid, RadarPosY + RadarSize), Color::Black(), 1.f);
			// Middle
			fill_box({ m_options.m_options.RadarPosX + RadarMid - 2, m_options.m_options.RadarPosY + RadarMid - 2, 5, 5 }, Color(255, 255, 255, 255.f));
		}

		auto Fov_Color = Color{ m_colors.m_visuals.Fov_Color[0], m_colors.m_visuals.Fov_Color[1], m_colors.m_visuals.Fov_Color[2], m_colors.m_visuals.Fov_Color[3] };
		auto Crosshair_Color = Color{ m_colors.m_visuals.Crosshair_Color[0], m_colors.m_visuals.Crosshair_Color[1], m_colors.m_visuals.Crosshair_Color[2], m_colors.m_visuals.Crosshair_Color[3] };

		if (m_aimbot.m_aimbot.ShowFov)
			DrawFov_(Fov_Color, screen_center, m_aimbot.m_aimbot.AimbotFOV);

		if (m_visuals.m_visuals.Crosshair)
		{
			vertical_line(Vector2{ (float)(screen_size.x / 2), (float)(screen_size.y / 2 + 1) }, 7.f, Crosshair_Color);
			vertical_line(Vector2{ (float)(screen_size.x / 2), (float)((screen_size.y / 2) - 7) }, 7.f, Crosshair_Color);
			horizontal_line(Vector2{ (float)(screen_size.x / 2 + 1), (float)(screen_size.y / 2) }, 7.f, Crosshair_Color);
			horizontal_line(Vector2{ (float)((screen_size.x / 2) - 7),(float)(screen_size.y / 2) }, 7.f, Crosshair_Color);
		}

		if (!Hooks::ClientInputhk.IsHooked())
		{
			Hooks::ClientInputhk.VirtualFunctionHook(SECURE("BasePlayer"), HASH("ClientInput"), &Hooks::ClientInput, SECURE(""), 1);
		}
		
		if (!Hooks::ProjectileUpdatehk.IsHooked())
		{
			Hooks::ProjectileUpdatehk.PointerSwapHook(SECURE("Projectile"), HASH("Update"), &Hooks::ProjectileUpdate, SECURE(""), 0);
		}
		
		if (!Hooks::OnAttackedhk.IsHooked())
		{
			Hooks::OnAttackedhk.VirtualFunctionHook(SECURE("BasePlayer"), HASH("OnAttacked"), &Hooks::OnAttacked, SECURE(""), 1);
		}
		
		
		if (!Hooks::DoAttackhk.IsHooked())
		{
			Hooks::DoAttackhk.VirtualFunctionHook(SECURE("FlintStrikeWeapon"), HASH("DoAttack"), &Hooks::DoAttack, SECURE(""), 0);
		}

		/*
		Hooks::ClientInputhk.InitializeVMT(SECURE("BasePlayer"), SECURE("ClientInput"), &Hooks::ClientInput, SECURE(""), 1);
		Hooks::ProjectileUpdatehk.InitializePTR(&Hooks::ProjectileUpdate, SECURE(""), SECURE("Projectile"), HASH("Update"), 0);
		Hooks::OnAttackedhk.InitializeVMT(SECURE("BasePlayer"), SECURE("OnAttacked"), &Hooks::OnAttacked, SECURE(""), 1);
		Hooks::DoAttackhk.InitializeVMT(SECURE("FlintStrikeWeapon"), SECURE("DoAttack"), &Hooks::DoAttack, SECURE(""), 0);
		*/

	

		if (m_aimbot.m_aimbot.ConnectedToServer)
		{
			if (UnityEngine::Input::GetKey(RustStructs::Keypad1))
			{
				AssemblyCSharp::PlayerModel().RebuildAll();
			}
		}

		int yoffset = 48;

		if (m_aimbot.m_aimbot.ConnectedToServer)
		{
			if (AssemblyCSharp::LocalPlayer::get_Entity() != nullptr)
			{
				g_visuals.CacheEntites();
				g_visuals.DrawEntities();

				g_visuals.CachePlayers();
				g_visuals.DrawPlayers();

				//if (UnityEngine::Input::GetKey(RustStructs::F))
				//{
				//	auto localPlaya = Visuals::m_local_player;

				//	if (localPlaya)
				//	{

				//		auto camera = Visuals::m_camera;
				//		if (camera) {
				//			auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500);
				//			if (AimbotTarget.m_player)
				//			{

				//				auto forwardDir = localPlaya->lookingAtPoint();
				//				auto dir = AimbotTarget.m_player->ClosestPoint(forwardDir);
				//				Vector2 outPos;
				//				//auto pointDir = localEyes - forwardDir;
				//				if (UnityEngine::WorldToScreen(dir, outPos))
				//				{
				//					DrawFov_(Fov_Color, outPos, 10.f);
				//				}
				//			}
				//		}
				//	}
				//}


				if (m_options.m_options.TargetLine)
				{
					auto camera = Visuals::m_camera;
					if (camera) {
						auto AimbotTarget = AssemblyCSharp::BasePlayer::GetTarget(camera->get_positionz(), 500);
						if (AimbotTarget.m_player)
						{
							auto targetPos = AimbotTarget.m_position;

							if (!targetPos.IsZero())
							{

								Vector2 w2sPos;

								if (UnityEngine::WorldToScreen(targetPos, w2sPos))
								{
									UnityEngine::GL::Line(Vector2(UnityEngine::Screen::get_width() / 2.f, UnityEngine::Screen::get_height() / 2.f), w2sPos, Color::White(), 1.f);
								}
							}

						}

					}
				}

				float info_y = 0;
				DrawPlayerHotbar(m_Event, { x - w / 2 + info_y, y - 521 }, { 450.f, 100.f });

				float center_x = (float)(screen_size.x) / 2, center_y = (float)(screen_size.y) / 2;


				if (m_options.m_options.ThickBullet && m_options.m_options.Thickbullet_Indicator)
				{
					Text(center_x, center_y - 58, 20, 20, SECURE(L"Bullet TP"), Color::Red(), true, true, 12.f);
				}

				if (m_options.m_options.AutoShootType == 0)
				{

					if (m_options.m_options.ThickBullet && m_options.m_options.Thickbullet_Indicator && m_aimbot.m_aimbot.Manipulation && m_options.m_options.Manipulation_Indicator && m_aimbot.m_aimbot.ManipKey)
					{
						Text(center_x, center_y - 48, 20, 20, SECURE(L"Manipulated"), Color::Red(), true, true, 12.f);
					}


					if (m_aimbot.m_aimbot.Manipulation && UnityEngine::Input::GetKey(m_aimbot.m_aimbot.ManipKey))
					{
						auto desynctime = UnityEngine::Time::get_realtimeSinceStartup() - AssemblyCSharp::LocalPlayer::get_Entity()->lastSentTickTime();
						auto desyncpercentage = (((desynctime / 0.85f) * 100.0f) + 1.f) / 100;

						float red, green, blue;
						float Num = desyncpercentage;
						float bars = 0;

						if (desyncpercentage < 0.1)
							Num = 0;

						if (Num != 0) {
							if (Num < 0.5) {
								red = Num * 2.f * 255.f;
								green = 255.f;
								blue = 0.f;
							}
							else {
								red = 255.f;
								green = (2.f - 2.f * Num) * 255.f;
								blue = 0.f;
							}

							if (Num > 1.f)
								Num = 1.f;


							float xzzzz = bars++;


							fill_box({ screen_center.x - 50, screen_center.y + yoffset - 1,100, 3 }, Color(0, 0, 0, 255.f));
							fill_box({ screen_center.x - 50, screen_center.y + yoffset + 1,100, 3 }, Color(0, 0, 0, 255.f));
							fill_box({ screen_center.x - 49, screen_center.y + yoffset, 100 * desynctime, 3 }, Color((int)(red), (int)(green), (int)(blue), 255.f));


							yoffset += 10;
						}
					}
				}
				else if (m_options.m_options.AutoShootType == 1)
				{
					auto playerInput = AssemblyCSharp::LocalPlayer::get_Entity()->input();

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

									if (m_options.m_options.ThickBullet && m_options.m_options.Thickbullet_Indicator && m_aimbot.m_aimbot.Manipulation && m_options.m_options.Manipulation_Indicator && m_aimbot.m_aimbot.ManipKey)
									{
										Text(center_x, center_y - 48, 20, 20, SECURE(L"Manipulated"), Color::Red(), true, true, 12.f);
									}


									if (m_aimbot.m_aimbot.Manipulation)
									{
										auto desynctime = UnityEngine::Time::get_realtimeSinceStartup() - AssemblyCSharp::LocalPlayer::get_Entity()->lastSentTickTime();
										auto desyncpercentage = (((desynctime / 0.85f) * 100.0f) + 1.f) / 100;

										float red, green, blue;
										float Num = desyncpercentage;
										float bars = 0;

										if (desyncpercentage < 0.1)
											Num = 0;

										if (Num != 0) {
											if (Num < 0.5) {
												red = Num * 2.f * 255.f;
												green = 255.f;
												blue = 0.f;
											}
											else {
												red = 255.f;
												green = (2.f - 2.f * Num) * 255.f;
												blue = 0.f;
											}

											if (Num > 1.f)
												Num = 1.f;


											float xzzzz = bars++;


											fill_box({ screen_center.x - 50, screen_center.y + yoffset - 1,100, 3 }, Color(0, 0, 0, 255.f));
											fill_box({ screen_center.x - 50, screen_center.y + yoffset + 1,100, 3 }, Color(0, 0, 0, 255.f));
											fill_box({ screen_center.x - 49, screen_center.y + yoffset, 100 * desynctime, 3 }, Color((int)(red), (int)(green), (int)(blue), 255.f));


											yoffset += 10;
										}
									}
								}

							}

						}

					}
				}

				//	if (m_visuals.m_visuals.DrawPlayerInv)
				//	{

				//	}

				if (m_options.m_options.Flyhack_Indicators)
				{
					float bar_width = 200.f;
					float x = screen_center.x;
					float y = screen_center.y;
					float w = 400.f;
					float h = 5.f;

					const auto horizontal_percentage = minm(m_options.m_options.horisontal_flyhack / m_options.m_options.max_flyhack_horisontal, 1.f);
					const auto vertical_percentage = minm(m_options.m_options.vertical_flyhack / m_options.m_options.max_flyhack_vertical, 1.f);


					fill_box({ x - w / 2, y - 441, w, h }, Color(0.f, 0.f, 0.f, 255.f));
					fill_box({ x - w / 2, y - 441, w * vertical_percentage, h }, Color(255.f, 0.f, 0.f, 255.f));
					//fill_box({ x - w / 2, y - 441, w, h }, Color(0.f, 0.f, 0.f, 255.f));



					fill_box({ x - w / 2, y - 431, w, h }, Color(0.f, 0.f, 0.f, 255.f));
					fill_box({ x - w / 2, y - 431, w * horizontal_percentage, h }, Color(255.f, 0.f, 0.f, 255.f));
					//fill_box({ x - w / 2, y - 431, w, h }, Color(0.f, 0.f, 0.f, 255.f));
				}

				if (m_options.m_options.AutoReload)
				{

					if (auto LocalPlayer = AssemblyCSharp::LocalPlayer::get_Entity())
					{
						if (LocalPlayer->lifestate() != RustStructs::Dead)
						{
							if (auto BaseProjectile = Visuals::m_local_player->GetHeldEntityCast<AssemblyCSharp::BaseProjectile>())
							{
								if (BaseProjectile->IsA(AssemblyCSharp::BaseProjectile::StaticClass()))
								{
									if (auto HeldItem = Visuals::m_local_player->ActiveItem())
									{
										if (!BaseProjectile->HasReloadCooldown())
										{
											if (m_options.m_options.did_reload == false && m_options.m_options.time_since_last_shot <= (m_options.m_options.reload_time - (m_options.m_options.reload_time / 10)) && m_options.m_options.time_since_last_shot > 0)
											{
												float time_full = (m_options.m_options.reload_time - (m_options.m_options.reload_time / 10));
												float time_left = m_options.m_options.time_since_last_shot;
												auto percentage = (((time_left / time_full) * 100.0f) + 1.f) / 100;
												float bars = 0;
												float red, green, blue;
												float Num = percentage;

												if (Num < 0.5) {
													red = Num * 2.f * 255.f;
													green = 255.f;
													blue = 0.f;
												}
												else {
													red = 255.f;
													green = (2.f - 2.f * Num) * 255.f;
													blue = 0.f;
												}


												if (Num > 0.01) {
													float xzzzz = bars++;

													if (auto mag = BaseProjectile->primaryMagazine())
													{
														if (auto def = mag->ammoType())
														{
															if (auto test = def->FindIconSprite(HeldItem->info()->itemid()))
															{
																if (auto texture = test->get_texture())
																{
																	auto rect = test->get_rect();
																	UnityEngine::GUI::SetColor(Color::White());
																	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(screen_center.x - 25, screen_center.y + 68, rect.wid / 4.5f, rect.hei / 4.5f), texture);
																}
															}
														}
													}
													fill_box({ screen_center.x - 50, screen_center.y + yoffset - 1,100, 3 }, Color(0, 0, 0, 255.f));
													fill_box({ screen_center.x - 50, screen_center.y + yoffset + 1,100, 3 }, Color(0, 0, 0, 255.f));
													fill_box({ screen_center.x - 49, screen_center.y + yoffset, 100 * (time_left / time_full), 3 }, Color((int)(red), (int)(green), (int)(blue), 255.f));
													yoffset += 10;
												}
											}
										}
										else
										{
											m_options.m_options.time_since_last_shot = 0.f;
											m_options.m_options.just_shot = false;
											m_options.m_options.did_reload = true;
										}
									
									}
								}
								else
								{
									m_options.m_options.time_since_last_shot = 0.f;
									m_options.m_options.just_shot = false;
									m_options.m_options.did_reload = true;
								}
							}
						}
					}
				}
			}
		}

		

		auto cl = Network::Net::get_cl();

		if (memory::IsAddressValid(cl))
		{
			if (!cl->IsConnected())
			{
				Visuals::m_local_player = nullptr;
				Visuals::m_camera = nullptr;
				Visuals::m_camera_position = Vector3();
				m_base_projectile = nullptr;
				vector_list.clear();
				vector_list_Temp.clear();
				PlayerVectorList.clear();
				PlayerVectorListTemp.clear();
				force_field_bundle->Unload(true);
				force_field_bundle = nullptr;

				colorchams_bundle->Unload(true);
				colorchams_bundle = nullptr;

				forcefield_shader = nullptr;
				forcefield_material = nullptr;

				colorchams_shader = nullptr;
				colorchams_material = nullptr;

				if (m_options.m_options.AntiFlyhack_Kick || m_options.m_options.Flyhack_Indicators)
				{
					//m_options.m_options.AntiFlyhack_Kick = false;
					//m_options.m_options.Flyhack_Indicators = false;

					m_options.m_options.max_flyhack_vertical = 0;
					m_options.m_options.vertical_flyhack = 0;
					m_options.m_options.max_flyhack_horisontal = 0;
					m_options.m_options.horisontal_flyhack = 0;
				}
			}

		}

		//Unhooking Section
		if (UnityEngine::Input::GetKey(RustStructs::End))
		{
			AssemblyCSharp::PlayerModel().RebuildAll();
			Hooks::ClientInputhk.Unhook();
			Hooks::OnNetworkMessagehk.Unhook();

			gui_style = nullptr;
			gui_skin = nullptr;
			white_texture = nullptr;
			
			screen_center = {0.f,0.f };
			screen_size = { 0.f,0.f };

			g_proggy_font_bundle->Unload(true);
			force_field_bundle->Unload(true);
			force_field_bundle = nullptr;
			g_proggy_font_bundle = nullptr;
			colorchams_bundle->Unload(true);
			colorchams_bundle = nullptr;
			g_default_font_bundle->Unload(true);
			g_default_font_bundle = nullptr;

		
			TestBundlee->Unload(true);
			TestBundlee = nullptr;

			MenuIconBundles->Unload(true);
			MenuIconBundles = nullptr;

			peter_texture2 = nullptr;
			peter_texture1 = nullptr;

			forcefield_shader = nullptr;
			forcefield_material = nullptr;

			colorchams_shader = nullptr;
			colorchams_material = nullptr;

			Visuals::m_local_player = nullptr;
			Visuals::m_camera = nullptr;
			Visuals::m_camera_position = Vector3();
			m_base_projectile = nullptr;
			vector_list.clear();
			vector_list_Temp.clear();
			PlayerVectorList.clear();
			PlayerVectorListTemp.clear();
			Hooks::Update_hk.Unhook();
			Hooks::PPS_WriteToStreamhk.Unhook();
			Hooks::OnInputhk.Unhook();
			Hooks::ProjectileUpdatehk.Unhook();
			Hooks::PPA_WriteToStreamhk.Unhook();

			Hooks::TryToMovehk.Unhook();
			Hooks::DoFixedUpdatehk.Unhook();
			Hooks::PlayerWalkMovementhk.Unhook();
			Hooks::OnAttackedhk.Unhook();

			Hooks::BlockSprinthk.Unhook();
			Hooks::DoAttackhk.Unhook();

			Hooks::PlayerProjectileRicochethk.Unhook();

			Hooks::OnGUIhk.Unhook();
			Hooks::SCRUpdatehk.Unhook();

		}
	}


	
	return;
}