#include "Menu.h"


void Menu::RenderMenu(UnityEngine::Event* Event)
{
	tab_size = Vector2(100, 35);

	auto mouse = UnityEngine::Input::GetMousePosition();

	auto width = UnityEngine::Screen::get_width();
	auto height = UnityEngine::Screen::get_height();

	Vector2 pos, menu_pos = window_position, menu_size = { 500, 380 }, button_size = { 200, 0 }, mouse_pos = { mouse.x, height - mouse.y };

	if (UnityEngine::Input::GetKey(RustStructs::Mouse0) && m_visuals.m_visuals.ShowMenu) {
		auto z = UnityEngine::rect_t{ window_position.x, window_position.y, menu_size.x, 30 };

		if (z.contains(mouse_pos))
		{
			window_position = (window_position + (mouse_pos - window_position) - Vector2(250, 15));
		}
		lmp = mouse_pos;
	}

	menu_event = Event->Type();

	static float accent_color[4] = { 140, 131, 214, 255 };
	static float accent_color2[4] = { 255, 233, 214, 255 };
	static bool color_test[50];
	main_menu_open = window(Event, SECURE(L"Voyage"), { 50, 50 }, { 560, 626 }, false);

	if (main_menu_open) {
		Tab(SECURE(L"Aimbot"), 0);
		Tab(SECURE(L"Esp"), 1);
		Tab(SECURE(L"Misc"), 2);
		Tab(SECURE(L"Weapon"), 3);
		Tab(SECURE(L"Settings"), 4);

		switch (activetab)
		{
		case 0:

			BeginChild(SECURE(L"Legit"), { 15,85 }, { 260,525 });
			{
				Unity_String aTest2[] = { Unity_String(SECURE(L"Head")), Unity_String(SECURE(L"Neck")),Unity_String(SECURE(L"Chest")),Unity_String(SECURE(L"Random")),Unity_String(SECURE(L"Closest to Crosshair")) };

				CheckBox(SECURE(L"Crosshair"), m_visuals.m_visuals.Crosshair);
				CheckBox(SECURE(L"Draw FOV"), m_aimbot.m_aimbot.ShowFov);
				CheckBox(SECURE(L"Target Line"), m_options.m_options.TargetLine);
				CheckBox(SECURE(L"Memory Aimbot"), m_aimbot.m_aimbot.MemoryAimbot);		
				if (m_aimbot.m_aimbot.MemoryAimbot) m_aimbot.m_aimbot.SilentAim = false;
				CheckBox(SECURE(L"pSilent"), m_aimbot.m_aimbot.SilentAim);

				if (m_aimbot.m_aimbot.SilentAim)
				{
					//Slider(SECURE(L"Bullet Vel"), m_options.m_options.ProjectileVelocity, 0, 100);
					//	Hotkey9(SECURE(L"SilentAim Key"), g_settings.AimbotKey);
					CheckBox(SECURE(L"MagicBullet (Ngun/Bow/PAmmo)"), m_aimbot.m_aimbot.LowVelocity);
					if (m_aimbot.m_aimbot.LowVelocity)
					{
						m_aimbot.m_aimbot.NewPrediction = false;
						m_aimbot.m_aimbot.Manipulation = false;
						m_options.m_options.pierce = false;
						m_options.m_options.ThickBullet = true;
					}
					CheckBox(SECURE(L"New Prediction"), m_aimbot.m_aimbot.NewPrediction);
					ComboBox(SECURE(L"HitBone"), m_aimbot.m_aimbot.SelectedAimbone, aTest2, 5);
					CheckBox(SECURE(L"Target Wounded"), m_aimbot.m_aimbot.TargetWounded);
					CheckBox(SECURE(L"Target Team"), m_aimbot.m_aimbot.TargetTeam);
					CheckBox(SECURE(L"Ignore Target"), m_options.m_options.ignore_target);
					CheckBox(SECURE(L"Target Scientist"), m_aimbot.m_aimbot.AimbotNPC);
					CheckBox(SECURE(L"Target AttackHeli"), m_options.m_options.HeliAimbot);
					if (m_options.m_options.HeliAimbot)
					{
						CheckBox(SECURE(L"Heli Hitbox Override"), m_options.m_options.HeliHitboxOverride);
						CheckBox(SECURE(L"HeliBulletQueue"), m_options.m_options.HeliBulletQueue);

					}
					//CheckBox(SECURE(L"Draw Prediction"), g_settings.DrawPrediction);
					Slider(SECURE(L"Aimbot Accuracy %"), m_aimbot.m_aimbot.AimbotAccuracy, 0, 100);
					Slider(SECURE(L"FOV Scale"), m_aimbot.m_aimbot.AimbotFOV, 0, 1000);

					Slider(SECURE(L"PSilent Simulations"), m_options.m_options.Simulations, 150, 2000);

					

				}
				if (m_aimbot.m_aimbot.MemoryAimbot)
				{
					NewKeybind(SECURE(L"MemoryAimbot Key"), m_aimbot.m_aimbot.MemoryAimKey);
				}
				CheckBox(SECURE(L"HitScan"), m_options.m_options.HitScan);
				if (m_options.m_options.HitScan)
					m_options.m_options.ThickBullet = false;

				Unity_String aTest[] = { Unity_String(SECURE(L"Flesh")), Unity_String(SECURE(L"Glass")),Unity_String(SECURE(L"Metal")),Unity_String(SECURE(L"Water")) };
				ComboBox(SECURE(L"HitMaterial"), m_options.m_options.HitMaterial, aTest, 4);

			

				//m_options.m_options.HitMaterial
			}
			BeginChild(SECURE(L"Rage"), { 285,85 }, { 260,525 });
			{
				CheckBox(SECURE(L"Manipulation"), m_aimbot.m_aimbot.Manipulation);
				if (m_aimbot.m_aimbot.Manipulation)
				{
					m_aimbot.m_aimbot.NewPrediction = true;
					//m_options.m_options.ThickBullet = true;
					Slider(SECURE(L"Rings"), m_options.m_options.maniprings, 5, 100);
					Slider(SECURE(L"Points"), m_options.m_options.manipsectors, 5, 100);
					CheckBox(SECURE(L"Draw Manipulation"), m_options.m_options.Manipulation_Dome);
					
					CheckBox(SECURE(L"InstantKill"), m_options.m_options.InstantKill);
					if (m_options.m_options.InstantKill)
					{
						Slider(SECURE(L"BulletsQueued"), m_options.m_options.BulletsQueued, 1, 8);
						NewKeybind(SECURE(L"InstantKill Key"), m_options.m_options.InstantKillKey);
						//CheckBox(SECURE(L"DoubleTap"), m_options.m_options.PowerShot);
					}
					NewKeybind(SECURE(L"Manipulator Key"), m_aimbot.m_aimbot.ManipKey);

					
				}
				if (!m_aimbot.m_aimbot.Manipulation)
				{
					m_options.m_options.InstantKill = false;
					m_options.m_options.Manipulation_Dome = false;
				}

				CheckBox(SECURE(L"AutoShoot"), m_options.m_options.AutoShoot);
				if (m_options.m_options.AutoShoot)
				{
					CheckBox(SECURE(L"Always Autoshoot"), m_options.m_options.AlwaysAutoshoot);
					CheckBox(SECURE(L"WaitforInstantBullet"), m_options.m_options.waitForInstantHit);
				}

				CheckBox(SECURE(L"Hitbox Override"), m_options.m_options.HitboxOverride);
				if (m_options.m_options.HitboxOverride)
				{
					Unity_String aTest[] = { Unity_String(SECURE(L"Head")), Unity_String(SECURE(L"Chest")),Unity_String(SECURE(L"Random")) };
					ComboBox(SECURE(L"Hitbox"), m_options.m_options.SelectedHitbox, aTest, 3);
				}
				CheckBox(SECURE(L"Bullet TP"), m_options.m_options.ThickBullet);
				if (m_options.m_options.ThickBullet)
				{
					Slider(SECURE(L"Max Thickness (2)"), m_options.m_options.Max_Thickness, 0, 2);
					CheckBox(SECURE(L"Draw ThickBullet"), m_options.m_options.DrawBulletTP);
					m_options.m_options.HitScan = false;
					CheckBox(SECURE(L"Pierce Materials"), m_options.m_options.pierce);
					//CheckBox(SECURE(L"Bullet TP Line"), m_options.m_options.ThickBulletLine);


				}
				CheckBox(SECURE(L"Instant Bullet (Desync 85%)"), m_options.m_options.InstantBullet);
				CheckBox(SECURE(L"Reflect"), m_options.m_options.Reflect);
				if (m_options.m_options.Reflect) {
					m_options.m_options.HitboxOverride = false;
				}
				CheckBox(SECURE(L"MountedMagicBullet"), m_options.m_options.MinicopterMagic);
				if (m_aimbot.m_aimbot.Manipulation)
				{
					Unity_String aTest[] = { Unity_String(SECURE(L"OnKey")), Unity_String(SECURE(L"Auto")) };
					ComboBox(SECURE(L"Manipulation Type"), m_options.m_options.AutoShootType, aTest, 2);
				}
				if (m_options.m_options.MinicopterMagic)
				{
					(SECURE(L"MountedMagicBullet Key"), m_options.m_options.MinicopterKey);
				}
				CheckBox(SECURE(L"TestMagicBullet"), m_aimbot.m_aimbot.magicboolet);
				if (m_aimbot.m_aimbot.Manipulation && !m_aimbot.m_aimbot.LowVelocity && m_options.m_options.ThickBullet)
					m_options.m_options.pierce = true;
			}
			break;
		case 1:
			BeginChild(SECURE(L"Players"), { 15,85 }, { 260,525 });
			{
				//CheckBox(SECURE(L"Enabled"), g_settings.ShowVis);

				//if (g_settings.ShowVis)
				//{
				CheckBox(SECURE(L"Visible Check"), m_visuals.m_visuals.VisCheck);
				CheckBox(SECURE(L"Box"), m_visuals.m_visuals.DrawBox);
				if (m_visuals.m_visuals.DrawBox) m_visuals.m_visuals.DrawCornerBox = false;
				CheckBox(SECURE(L"CornerBox"), m_visuals.m_visuals.DrawCornerBox);		
				if (m_visuals.m_visuals.DrawCornerBox) m_visuals.m_visuals.DrawBox = false;
				CheckBox(SECURE(L"Name"), m_visuals.m_visuals.DrawNametag);
				CheckBox(SECURE(L"Distance"), m_visuals.m_visuals.DrawDistance);
				CheckBox(SECURE(L"Health Bar"), m_visuals.m_visuals.DrawHealthBar);
				CheckBox(SECURE(L"HeldItem"), m_visuals.m_visuals.DrawHeldItem);
				CheckBox(SECURE(L"Teammates"), m_visuals.m_visuals.DrawTeammates);
				CheckBox(SECURE(L"Sleepers"), m_visuals.m_visuals.DrawSleepers);
				CheckBox(SECURE(L"SafeZone"), m_visuals.m_visuals.DrawSafezoneFlag);
				CheckBox(SECURE(L"Wounded"), m_visuals.m_visuals.DrawWounded);
				CheckBox(SECURE(L"Scientist"), m_visuals.m_visuals.DrawNPC);
				CheckBox(SECURE(L"DrawPlayerInv"), m_visuals.m_visuals.DrawPlayerInv);
				CheckBox(SECURE(L"Radar"), m_options.m_options.DrawRadar);
				CheckBox(SECURE(L"PeterGriffinMode"), m_visuals.m_visuals.PeterGriffinMode);

				
				CheckBox(SECURE(L"Player Chams"), m_options.m_options.DrawPlayerChams);
				if (m_options.m_options.DrawPlayerChams)
				{
					Unity_String aTest[] = { Unity_String(SECURE(L"Galaxy")), Unity_String(SECURE(L"Rainbow")),Unity_String(SECURE(L"Flat")),Unity_String(SECURE(L"Hologram")) };
					ComboBox(SECURE(L"PlayerChams"), m_options.m_options.selectedChams, aTest, 4);
				}
				CheckBox(SECURE(L"Weapon Chams"), m_options.m_options.DrawWeaponChams);
				if (m_options.m_options.DrawWeaponChams)
				{
					Unity_String aTest[] = { Unity_String(SECURE(L"Galaxy")), Unity_String(SECURE(L"Rainbow")),Unity_String(SECURE(L"Flat")),Unity_String(SECURE(L"Hologram")) };
					ComboBox(SECURE(L"WeaponChams"), m_options.m_options.selectedWeaponChams, aTest, 4);
				}

				CheckBox(SECURE(L"Bullet Tracers"), m_options.m_options.BulletTracers);
				if (m_options.m_options.BulletTracers)
				{
					//Slider(SECURE(L"Tracer Duration "), m_options.m_options.TracersDuration, 0, 3);
				}

				CheckBox(SECURE(L"Movement Lines"), m_options.m_options.MovementLines);

				Slider(SECURE(L"Max Player Distance"), m_options.m_options.max_player_distance, 0, 500);

				if (m_options.m_options.DrawRadar)
					Slider(SECURE(L"Radar Size"), m_options.m_options.RadarSize, 0, 300);

				Slider(SECURE(L"Font Size"), m_colors.m_visuals.fontSize, 0, 15);

				//}
			}
			BeginChild(SECURE(L"World"), { 285,85 }, { 260,525 });
			{
				Unity_String aTest44[] = { Unity_String(SECURE(L"Collectable")), Unity_String(SECURE(L"Ore")),Unity_String(SECURE(L"Misc")),Unity_String(SECURE(L"SkyDome")),Unity_String(SECURE(L"Npc")),Unity_String(SECURE(L"Barrels")),Unity_String(SECURE(L"Animals")),Unity_String(SECURE(L"Crate")) };
				ComboBox(SECURE(L"World Pg2"), m_options.m_options.WorldPg, aTest44, 8);

				if (m_options.m_options.WorldPg == 0)
				{
					CheckBox(SECURE(L"Hemp"), m_options.m_options.Hemp);
					if (m_options.m_options.Hemp)
					{
						CheckBox(SECURE(L"Hemp Icon"), m_options.m_options.HempIcon);

						CheckBox(SECURE(L"Hemp Chams"), m_options.m_options.HempChams);
					}

					CheckBox(SECURE(L"DroppedItems"), m_options.m_options.DroppedItems);

					CheckBox(SECURE(L"Mushroom"), m_options.m_options.mushroom);
					CheckBox(SECURE(L"Pumpkin"), m_options.m_options.pumpkin);
					CheckBox(SECURE(L"Corn"), m_options.m_options.corn);

					Slider(SECURE(L"Collectables Distance"), m_options.m_options.max_collectable_distance, 0, 500);
					Slider(SECURE(L"Food Distance"), m_options.m_options.max_food_distance, 0, 500);
					Slider(SECURE(L"DroppedItems Distance"), m_options.m_options.max_dropped_distance, 0, 500);

				}
				else if (m_options.m_options.WorldPg == 1)
				{
					CheckBox(SECURE(L"Ores"), m_options.m_options.OreEsp);
					if (m_options.m_options.OreEsp)
					{
						CheckBox(SECURE(L"Stone"), m_options.m_options.StoneOre);
						if (m_options.m_options.StoneOre)
						{
							CheckBox(SECURE(L"Stone Icon"), m_options.m_options.StoneIcon);
							CheckBox(SECURE(L"Stone Chams"), m_options.m_options.StoneChams);
						}

						CheckBox(SECURE(L"Metal"), m_options.m_options.MetalOre);
						if (m_options.m_options.MetalOre)
						{
							CheckBox(SECURE(L"Metal Icon"), m_options.m_options.MetalIcon);

							CheckBox(SECURE(L"Metal Chams"), m_options.m_options.MetalChams);
						}

						CheckBox(SECURE(L"Sulfur"), m_options.m_options.SulfurOre);
						if (m_options.m_options.SulfurOre)
						{
							CheckBox(SECURE(L"Sulfur Icon"), m_options.m_options.SulfurIcon);

							CheckBox(SECURE(L"Sulfur Chams"), m_options.m_options.SulfurChams);
						}

						Slider(SECURE(L"Max Ore Distance "), m_options.m_options.Max_Ore_Distance, 0, 300);
					}
				}
				else if (m_options.m_options.WorldPg == 2)
				{
					CheckBox(SECURE(L"Stash"), m_options.m_options.StashEsp);
					if (m_options.m_options.StashEsp)
					{

						CheckBox(SECURE(L"Stash Chams"), m_options.m_options.StashChams);

						Slider(SECURE(L"Max Stash Distance "), m_options.m_options.Max_Stash_Distance, 0, 300);
					}
					CheckBox(SECURE(L"Workbench"), m_options.m_options.Workbench);

					CheckBox(SECURE(L"Corpse"), m_options.m_options.PlayerCorpse);
					CheckBox(SECURE(L"Backpack"), m_options.m_options.Backpack);

					CheckBox(SECURE(L"ToolCupboard"), m_options.m_options.ToolCupboard);
					if (m_options.m_options.ToolCupboard)
					{
						CheckBox(SECURE(L"Authorized Players"), m_options.m_options.AuthorizedPlayers);
						if (m_options.m_options.AuthorizedPlayers)
						{
							Slider(SECURE(L"Authorized Players Distance"), m_options.m_options.AuthorizedPlayersDistance, 0, 300);
						}
					}

					CheckBox(SECURE(L"AutoTurret"), m_options.m_options.AutoTurret);
					CheckBox(SECURE(L"ShotgunTrap"), m_options.m_options.ShotgunTrap);
						
					Slider(SECURE(L"Traps Distance"), m_options.m_options.max_traps_distance, 0, 500);
					Slider(SECURE(L"PlayerDrops Distance"), m_options.m_options.max_playerdrops_distance, 0, 500);
					Slider(SECURE(L"World Distance"), m_options.m_options.max_world_distance, 0, 500);

					CheckBox(SECURE(L"RaidESP"), m_visuals.m_visuals.RaidESP);


				}
				else if (m_options.m_options.WorldPg == 3)
				{
					CheckBox(SECURE(L"Skydome"), m_options.m_options.Skydome);
					if (m_options.m_options.Skydome)
					{
						CheckBox(SECURE(L"BrightNight"), m_options.m_options.BrightNight);
						CheckBox(SECURE(L"BrightCave"), m_options.m_options.BrightCave);
						CheckBox(SECURE(L"Stars"), m_options.m_options.Stars);
						CheckBox(SECURE(L"Rainbow Clouds"), m_options.m_options.RainbowClouds);
						CheckBox(SECURE(L"SkyColor"), m_options.m_options.SkyColor);
						CheckBox(SECURE(L"CloudColor"), m_options.m_options.CloudColor);
						CheckBox(SECURE(L"TimeChanger"), m_options.m_options.TimeChanger);
						if (m_options.m_options.TimeChanger)
							Slider(SECURE(L"Game Time"), m_options.m_options.Time, 0, 24);
					}

				}
				else if (m_options.m_options.WorldPg == 4)
				{
					CheckBox(SECURE(L"Attack Heli"), m_options.m_options.DrawAttackHeli);
					CheckBox(SECURE(L"BradleyAPC"), m_options.m_options.BradleyAPC);
					if (m_options.m_options.BradleyAPC)
					{
						CheckBox(SECURE(L"Bradley Chams"), m_options.m_options.BradChams);
					}

					Slider(SECURE(L"APC Distance"), m_options.m_options.max_apc_distance, 0, 2500);
				}
				else if (m_options.m_options.WorldPg == 5)
				{
					CheckBox(SECURE(L"Diesel Barrel"), m_options.m_options.DieselBarrel);
					CheckBox(SECURE(L"Oil Barrel"), m_options.m_options.OilBarrel);
					CheckBox(SECURE(L"Loot Barrel"), m_options.m_options.LootBarrel);

					Slider(SECURE(L"Barrels Distance"), m_options.m_options.max_crates_distance, 0, 500);
				}
				else if (m_options.m_options.WorldPg == 6)
				{
					CheckBox(SECURE(L"Bear"), m_options.m_options.Bear);
					CheckBox(SECURE(L"Polar Bear"), m_options.m_options.polarbear);
					CheckBox(SECURE(L"Deer"), m_options.m_options.Deer);
					CheckBox(SECURE(L"Wolf"), m_options.m_options.Wolf);
					CheckBox(SECURE(L"Boar"), m_options.m_options.boar);
					CheckBox(SECURE(L"Chicken"), m_options.m_options.chicken);
					CheckBox(SECURE(L"Horse"), m_options.m_options.horse);

					Slider(SECURE(L"Animals Distance"), m_options.m_options.max_animal_distance, 0, 500);

				}
				else if (m_options.m_options.WorldPg == 7)
				{
					CheckBox(SECURE(L"Basic Crate"), m_options.m_options.BasicCrate);
					CheckBox(SECURE(L"Normal Crate"), m_options.m_options.NormalCrate);
					CheckBox(SECURE(L"Elite Crate"), m_options.m_options.EliteCrate);
					CheckBox(SECURE(L"Military Crate"), m_options.m_options.MilitaryCrate);
					CheckBox(SECURE(L"Bradley Crate"), m_options.m_options.BradleyCrate);
					CheckBox(SECURE(L"Heli Crate"), m_options.m_options.HeliCrate);
					CheckBox(SECURE(L"Hackable Crate"), m_options.m_options.HackableCrate);
					CheckBox(SECURE(L"Airdrop"), m_options.m_options.Airdrop);

					Slider(SECURE(L"Crates Distance"), m_options.m_options.max_crates_distance, 0, 500);
				}
				CheckBox(SECURE(L"Draw Boxes"), m_options.m_options.DrawBoxesWorld);
				CheckBox(SECURE(L"Draw Health Bars"), m_options.m_options.DrawHealthWorld);
			}
			break;
		case 2:
			BeginChild(SECURE(L"Local"), { 15,85 }, { 260,525 });
			{
				CheckBox(SECURE(L"Spiderman"), m_options.m_options.Spiderman);
				CheckBox(SECURE(L"SmallerLocalRadius"), m_options.m_options.SmallerLocalRadius);
				CheckBox(SECURE(L"Infinite Jump"), m_options.m_options.InfiniteJump);
				CheckBox(SECURE(L"Adminflags"), m_options.m_options.AdminFlags);
				CheckBox(SECURE(L"BlockServerCommands"), m_options.m_options.AdminCheat);
				if (m_options.m_options.AdminCheat)
					NewKeybind(SECURE(L"BlockServerCommands Key"), m_options.m_options.AdminCheatKey);
				CheckBox(SECURE(L"No Movement Slowdown"), m_options.m_options.NoMovementSlowdown);
				CheckBox(SECURE(L"Attack Whilst Mounted"), m_options.m_options.AttackWhilstMounted);
				CheckBox(SECURE(L"Toggle Sprint"), m_options.m_options.ToggleSprint);
				CheckBox(SECURE(L"Name Spoofer"), m_options.m_options.NameSpoofer);
				CheckBox(SECURE(L"CanAttack"), m_options.m_options.CanAttack);
				CheckBox(SECURE(L"Suicide"), m_options.m_options.Suicide);
				if (m_options.m_options.Suicide)
					NewKeybind(SECURE(L"Suicide Key"), m_options.m_options.SuicideKey);
				CheckBox(SECURE(L"High Walk"), m_options.m_options.HighWalk);
				CheckBox(SECURE(L"Ladder Shoot"), m_options.m_options.LadderShoot);
				CheckBox(SECURE(L"PlayerFov"), m_options.m_options.PlayerFov);
				if (m_options.m_options.PlayerFov)
				{
					Slider(SECURE(L"Fov Amount "), m_options.m_options.FieldOfView, 0, 180);
				}
				CheckBox(SECURE(L"Zoom"), m_options.m_options.Zoom);
				if (m_options.m_options.Zoom)
				{
					NewKeybind(SECURE(L"Zoom Key"), m_options.m_options.ZoomKey);
					Slider(SECURE(L"Zoom Amount "), m_options.m_options.ZoomAmount, 0, 50);
				}
				CheckBox(SECURE(L"LongMelee"), m_options.m_options.LongMelee);
				CheckBox(SECURE(L"HoldVelocity"), m_options.m_options.HoldVelocity);
				if (m_options.m_options.HoldVelocity)
				{
					NewKeybind(SECURE(L"HoldVelocity Key"), m_options.m_options.HoldVelocityKey);
				}


				CheckBox(SECURE(L"LadderGodMode"), m_options.m_options.LadderGodMode);

			}
			BeginChild(SECURE(L"Other"), { 285,85 }, { 260, 525 });
			{
				CheckBox(SECURE(L"Flyhack Bar"), m_options.m_options.Flyhack_Indicators);
				if (m_options.m_options.Flyhack_Indicators)
					CheckBox(SECURE(L"Anti Flykick"), m_options.m_options.AntiFlyhack_Kick);
				CheckBox(SECURE(L"AntiTerrainKick"), m_options.m_options.AntiDeathBarrier);
				CheckBox(SECURE(L"Walk On Water"), m_options.m_options.WalkOnWater);
				CheckBox(SECURE(L"Walk Through Trees"), m_options.m_options.WalkThroughTrees);
				CheckBox(SECURE(L"Walk Throught Players"), m_options.m_options.WalkThroughPlayers);
				CheckBox(SECURE(L"Omnisprint"), m_options.m_options.OmniSprint);
				CheckBox(SECURE(L"SilentWalk"), m_options.m_options.SilentWalk);
				if (m_options.m_options.SilentWalk)
					NewKeybind(SECURE(L"SilentWalk Key"), m_options.m_options.SilentWalkKey);
				CheckBox(SECURE(L"Instant Loot"), m_options.m_options.FastLoot);
				CheckBox(SECURE(L"Instant Heal"), m_options.m_options.InstantHeal);
				CheckBox(SECURE(L"InstantRevive"), m_options.m_options.InstantRevive);
				if (m_options.m_options.InstantRevive)
				{
					NewKeybind(SECURE(L"InstantRevive Key"), m_options.m_options.InstantReviveKey);
				}
				CheckBox(SECURE(L"Interactive Debug"), m_options.m_options.InteractiveDebug);
				if (m_options.m_options.InteractiveDebug)
					NewKeybind(SECURE(L"Interactive Key"), m_options.m_options.InteractiveKey);
				CheckBox(SECURE(L"Speedhack"), m_options.m_options.Speedhack);
				if (m_options.m_options.Speedhack)
				{
					NewKeybind(SECURE(L"Speedhack Key"), m_options.m_options.SpeedhackKey);
					Slider(SECURE(L"Speedhack Speed "), m_options.m_options.SpeedhackSpeed, 0, 20);
				}
				CheckBox(SECURE(L"Spinbot"), m_options.m_options.Spinbot);
				CheckBox(SECURE(L"DoorSpammer"), m_options.m_options.DoorSpammer);
				CheckBox(SECURE(L"FixDebugCamera"), m_options.m_options.FixDebugCamera);
	
			}
			break;
		case 3:
			BeginChild(SECURE(L"Weapon"), { 15,85 }, { 260,525 });
			{
				//CheckBox(SECURE(L"Draw Reloadbar"), m_options.m_options.DrawReloadBar);
				CheckBox(SECURE(L"Auto Reload"), m_options.m_options.AutoReload);
				CheckBox(SECURE(L"Large Bullets"), m_options.m_options.BigBullet);
				CheckBox(SECURE(L"No Weapon Bob"), m_options.m_options.NoWeaponBob);

		
				//m_options.m_options.NoRecoilPercentX

				CheckBox(SECURE(L"NoRecoil"), m_options.m_options.NoRecoil);
				CheckBox(SECURE(L"NoSpread"), m_options.m_options.NoSpread);
				CheckBox(SECURE(L"ChangeRepeatDelay"), m_options.m_options.ChangeRepeatDelay);

				CheckBox(SECURE(L"InstantEoka"), m_options.m_options.InstantEoka);
				CheckBox(SECURE(L"Automatic"), m_options.m_options.ForceAutomatic);
				CheckBox(SECURE(L"Instant Bow"), m_options.m_options.InstantBow);
				CheckBox(SECURE(L"Instant Compound Charge"), m_options.m_options.InstantCompoundCharge);
				CheckBox(SECURE(L"No Movement Penalty"), m_options.m_options.NoMovementPenalty);
				CheckBox(SECURE(L"No Animation Delay"), m_options.m_options.AnimationDelay);
				CheckBox(SECURE(L"No Deploy Delay"), m_options.m_options.InstantSwitch);
				CheckBox(SECURE(L"HitSounds"), m_options.m_options.Hitsound);
				CheckBox(SECURE(L"WeaponSpam"), m_options.m_options.WeaponSpam);
				if (m_options.m_options.WeaponSpam)
				{
					NewKeybind(SECURE(L"WeaponSpam Key"), m_options.m_options.WeaponSpamKey);
				}
			
			}
			BeginChild(SECURE(L"Other"), { 285,85 }, { 260,525 });
			{

				CheckBox(SECURE(L"SignPainter (C:/image.png)"), m_options.m_options.SignPainter);
				if (m_options.m_options.SignPainter)
				{
					NewKeybind(SECURE(L"SignPainter Key"), m_options.m_options.SignPainterKey);
				}
				CheckBox(SECURE(L"AutoUpgrade"), m_options.m_options.AutoUpgrade);
				if (m_options.m_options.AutoUpgrade)
				{
					Unity_String aTest44[] = { Unity_String(SECURE(L"Twigs")), Unity_String(SECURE(L"Wood")),Unity_String(SECURE(L"Stone")),Unity_String(SECURE(L"Metal")),Unity_String(SECURE(L"HQM")) };
					ComboBox(SECURE(L"BuildingGrade"), m_options.m_options.BuildingGrade, aTest44, 5);
				}
				CheckBox(SECURE(L"Chat Spammer"), m_options.m_options.ChatSpam);
				if (m_options.m_options.ChatSpam)
				{
					NewKeybind(SECURE(L"ChatSpam Key"), m_options.m_options.ChatSpamKey);
				}
				CheckBox(SECURE(L"Disable Recyclers"), m_options.m_options.DisableRecycler);
				if (m_options.m_options.DisableRecycler)
				{
					NewKeybind(SECURE(L"DisableRecycler Key"), m_options.m_options.DisableRecyclerKey);
				}
				CheckBox(SECURE(L"Hammer Spam"), m_options.m_options.HammerSpam);
				if (m_options.m_options.HammerSpam)
				{
					Unity_String aTest2[] = { Unity_String(SECURE(L"Glass")), Unity_String(SECURE(L"Water")) };
					ComboBox(SECURE(L"Material"), m_options.m_options.SelectedSpamMaterial, aTest2, 2);
					NewKeybind(SECURE(L"HammerSpam Key"), m_options.m_options.HammerSpamKey);
				}
				CheckBox(SECURE(L"Silent Farm"), m_options.m_options.AutoFarm);
				CheckBox(SECURE(L"Silent Melee"), m_options.m_options.SilentMelee);
				CheckBox(SECURE(L"Auto Pickup Collectable"), m_options.m_options.AutoPickup);
				CheckBox(SECURE(L"Farmbot - Ores"), m_options.m_options.FarmBot);
				CheckBox(SECURE(L"Farmbot - Hemp"), m_options.m_options.HempBot);
				CheckBox(SECURE(L"Keep Alive"), m_options.m_options.KeepAlive);
				if (m_options.m_options.KeepAlive)
					NewKeybind(SECURE(L"Keep Alive Key"), m_options.m_options.KeepAliveKey);
				CheckBox(SECURE(L"AutoUnhideStashes"), m_options.m_options.AutoUnhideStashes);
			}
			break;
		case 4:
			BeginChild(SECURE(L"Esp"), { 15,85 }, { 260,525 });
			{
				ColorBox(SECURE(L"Accent Color"), color_test[145], accent_color_);
				ColorBox(SECURE(L"Crosshair Color"), color_test[2], m_colors.m_visuals.Crosshair_Color);
				ColorBox(SECURE(L"Fov Color"), color_test[3], m_colors.m_visuals.Fov_Color);

				if (m_options.m_options.ColorPg == 0)
				{
					ColorBox(SECURE(L"Visible Color"), color_test[4], m_colors.m_visuals.VisCheck_Color);
					ColorBox(SECURE(L"PlayerBox Color"), color_test[5], m_colors.m_visuals.DrawBox_Color);
					ColorBox(SECURE(L"Nametag Color"), color_test[6], m_colors.m_visuals.DrawNameTag_Color);
					ColorBox(SECURE(L"HeldItem Color"), color_test[7], m_colors.m_visuals.DrawHeldItem_Color);
					ColorBox(SECURE(L"TeamMates Color"), color_test[50], m_colors.m_visuals.TeamMates_Color);
					ColorBox(SECURE(L"Wounded Color"), color_test[51], m_colors.m_visuals.Wounded_Color);
					ColorBox(SECURE(L"Scientist Color"), color_test[52], m_colors.m_visuals.Scientist_color);
					ColorBox(SECURE(L"Sleeper Color"), color_test[8], m_colors.m_visuals.Sleeper_Color);
					ColorBox(SECURE(L"Safezone Color"), color_test[9], m_colors.m_visuals.Safezone_Color);
					ColorBox(SECURE(L"Corpse Color"), color_test[10], m_colors.m_visuals.CorpseColor);
					ColorBox(SECURE(L"Backack Color"), color_test[11], m_colors.m_visuals.BackPackColor);
					ColorBox(SECURE(L"WeaponChams Color"), color_test[12], m_colors.m_visuals.WeaponChamsColor);
					ColorBox(SECURE(L"PlayerChams Color"), color_test[13], m_colors.m_visuals.PlayerChams_Color);
					ColorBox(SECURE(L"MovementLines Color"), color_test[14], m_colors.m_visuals.MovementLinesColor);
				}
				else if (m_options.m_options.ColorPg == 1)
				{
					ColorBox(SECURE(L"NormalCrate Color"), color_test[15], m_colors.m_visuals.NormalCrateColor);
					ColorBox(SECURE(L"BasicCrate Color"), color_test[16], m_colors.m_visuals.BasicCrateColor);
					ColorBox(SECURE(L"EliteCrate Color"), color_test[17], m_colors.m_visuals.EliteCrateColor);
					ColorBox(SECURE(L"MilitaryCrate Color"), color_test[49], m_colors.m_visuals.MilitaryCrateColor);
					ColorBox(SECURE(L"BradleyCrate Color"), color_test[18], m_colors.m_visuals.BradleyCrateColor);
					ColorBox(SECURE(L"HeliCrate Color"), color_test[19], m_colors.m_visuals.HeliCrateColor);
					ColorBox(SECURE(L"Hackable Color"), color_test[47], m_colors.m_visuals.HackableCrateColor);
					ColorBox(SECURE(L"Airdrop Color"), color_test[48], m_colors.m_visuals.HeliCrateColor);
					ColorBox(SECURE(L"OilBarrel Color"), color_test[20], m_colors.m_visuals.OilBarrelColor);
					ColorBox(SECURE(L"Barrel Color"), color_test[21], m_colors.m_visuals.LootBarrelColor);
					ColorBox(SECURE(L"DieselBarrel Color"), color_test[22], m_colors.m_visuals.DieselBarrelColor);
					ColorBox(SECURE(L"Mushroom Color"), color_test[23], m_colors.m_visuals.MushRoomColor);
					ColorBox(SECURE(L"Pumpkin Color"), color_test[24], m_colors.m_visuals.PumpkinColor);
					ColorBox(SECURE(L"Corn Color"), color_test[25], m_colors.m_visuals.CornColor);

				}
				else if (m_options.m_options.ColorPg == 2)
				{
					ColorBox(SECURE(L"Stash Chams Color"), color_test[26], m_colors.m_visuals.StashChamsColor);
					ColorBox(SECURE(L"Hemp Chams Color"), color_test[27], m_colors.m_visuals.HempChamsColor);
					ColorBox(SECURE(L"Stone Chams Color"), color_test[28], m_colors.m_visuals.StoneChamsColor);
					ColorBox(SECURE(L"Metal Chams Color"), color_test[29], m_colors.m_visuals.MetalChamsColor);
					ColorBox(SECURE(L"Sulfur Chams Color"), color_test[30], m_colors.m_visuals.SulfurChamsColor);

					ColorBox(SECURE(L"Stash Color"), color_test[31], m_colors.m_visuals.StashColor);
					ColorBox(SECURE(L"Hemp Color"), color_test[32], m_colors.m_visuals.HempColor);
					ColorBox(SECURE(L"Stone Color"), color_test[33], m_colors.m_visuals.StoneColor);
					ColorBox(SECURE(L"Metal Color"), color_test[34], m_colors.m_visuals.MetalColor);
					ColorBox(SECURE(L"Sulfur Color"), color_test[35], m_colors.m_visuals.SulfurColor);
					ColorBox(SECURE(L"Tracer Color"), color_test[36], m_colors.m_visuals.TracersColor);
					ColorBox(SECURE(L"Turret Color"), color_test[37], m_colors.m_visuals.TurretColor);
				}
				else if (m_options.m_options.ColorPg == 3)
				{
					ColorBox(SECURE(L"SkyColor"), color_test[53], m_colors.m_visuals.SkyColor);
					ColorBox(SECURE(L"CloudColor"), color_test[54], m_colors.m_visuals.CloudColor);
				}
				else if (m_options.m_options.ColorPg == 4)
				{
					ColorBox(SECURE(L"ShotgunTrap Color"), color_test[38], m_colors.m_visuals.ShotgunColor);
					ColorBox(SECURE(L"DroppedItem Color"), color_test[39], m_colors.m_visuals.DroppedItemColor);
					ColorBox(SECURE(L"Bradley Color"), color_test[40], m_colors.m_visuals.BradleyColor);
					ColorBox(SECURE(L"Bradley Chams Color"), color_test[41], m_colors.m_visuals.BradChamsColor);
					ColorBox(SECURE(L"Wolf Color"), color_test[42], m_colors.m_visuals.WolfColor);
					ColorBox(SECURE(L"Bear Color"), color_test[43], m_colors.m_visuals.BearColor);
					ColorBox(SECURE(L"PolarBear Color"), color_test[44], m_colors.m_visuals.PolarBearColor);
					ColorBox(SECURE(L"Chicken Color"), color_test[45], m_colors.m_visuals.ChickenColor);
					ColorBox(SECURE(L"Horse Color"), color_test[46], m_colors.m_visuals.HorseColor);
					//54
				}
			}
			BeginChild(SECURE(L"World"), { 285,85 }, { 260,525 });
			{
				CheckBoxConfigSave(SECURE(L"Save Config"), saveconfig);
				CheckBoxConfigLoad(SECURE(L"Load Config"), loadconfig);
				//CheckBox(SECURE(L"Pixel Font"), g_settings.PixelFont);

				Unity_String aTest44[] = { Unity_String(SECURE(L"Players")), Unity_String(SECURE(L"World")),Unity_String(SECURE(L"Misc")), Unity_String(SECURE(L"Skydome")), Unity_String(SECURE(L"Other")) };
				ComboBox(SECURE(L"Colors Pg2"), m_options.m_options.ColorPg, aTest44, 5);

				CheckBox(SECURE(L"Font Changer"), m_options.m_options.FontPicker);

				Unity_String aFont[] = { Unity_String(SECURE(L"Arial")), Unity_String(SECURE(L"Tahoma")),Unity_String(SECURE(L"Verdana")),Unity_String(SECURE(L"Roboto")),Unity_String(SECURE(L"Segoe")),Unity_String(SECURE(L"Acme")),Unity_String(SECURE(L"Heebo")),Unity_String(SECURE(L"Comicsans")),Unity_String(SECURE(L"Ubuntu")),Unity_String(SECURE(L"Visitor")), Unity_String(SECURE(L"Regular")) , Unity_String(SECURE(L"Pixel")) };
				ComboBox(SECURE(L"Font"), m_options.m_options.SelectedFont, aFont, 12);

				CheckBox(SECURE(L"中文翻译"), m_visuals.m_visuals.ChineseTranslation);
			

				//ColorPicker(L"test", m_colors.m_visuals.HorseColor, 150.f, 150.f);
			}
			break;
		}
	}

	end_window();
}
