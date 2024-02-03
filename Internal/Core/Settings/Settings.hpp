#pragma once

#include "../RustSDK/RustStructs.h"

#define MAXIMUM_AIMBOT_ACCURACY 100
#define MINIMUM_AIMBOT_ACCURACY 1


inline bool saveconfig = false;
inline bool loadconfig = false;



namespace options
{

	struct CategoryAimbot
	{
		struct
		{
			bool NewPrediction = false;
			bool SilentAim = false;
			float AimbotFOV = 200.f;
			bool AimbotNPC = false;
			int SelectedAimbone = 0;
			bool ShowFov = false;

			float AimbotAccuracy = 100;
			bool Manipulation = false;
			bool viewOffset = false;
			Vector3 ViewOffsetValue = Vector3(0.f, 0.f, 0.f);
			bool ManipAlways = false;
			RustStructs::KeyCode ManipKey = RustStructs::KeyCode::F;

			bool magicboolet = false;
			bool MemoryAimbot = false;
			RustStructs::KeyCode MemoryAimKey = RustStructs::KeyCode::Mouse1;

			bool TargetTeam = false;
			bool TargetWounded = false;
			bool TargetAnimals = false;
			bool ConnectedToServer = false;
			bool LowVelocity = false;
		} m_aimbot = { };

	};

	struct CategoryVisuals
	{
		struct
		{
			bool ShowVis = true;
			bool ShowMenu = false;
			bool Crosshair = true;
			bool DrawNPC = false;
			bool DrawSleepers = false;
			bool DrawBox = false;
			bool DrawCornerBox = false;
			bool VisCheck = false;
			bool DrawHealthBar = false;
			bool DrawNametag = false;
			bool DrawHeldItem = false;
			bool DrawWounded = false;
			bool DrawTeammates = false;
			bool DrawSafezoneFlag = false;
			bool DrawPlayerInv = false;
			bool BottomHealthBar = false;
			bool DrawDistance = false;
			bool RaidESP = false;
			bool PeterGriffinMode = false;
			bool ChineseTranslation = false;

		} m_visuals = { };
	};

	struct CategoryCommon
	{
		struct
		{
			float last_tick_time = 0.f;
			float max_spoofed_eye_distance = 0.f;
			bool can_manipulate = false;

			bool DrawReloadBar = false;

			bool InstantBullet = false;
			bool ThickBullet = false;
			float ThickBullet_Thickness = 2.f;

			bool HitScan = false;
			float reload_time = 0.f;
			bool NoSpread = false;
			bool ForceAutomatic = false;
			bool InstantSwitch = false;
			bool AnimationDelay = false;
			bool InstantBow = false;
			bool InstantCompoundCharge = false;
			bool InstantHeal = false;
			bool NoMovementPenalty = false;
			bool AutoReload = false;
			bool NoRecoil = false;
			bool DrawWeaponInfo = false;

			bool FastLoot = false;

			bool NoWeaponBob = false;
			bool WalkOnWater = false;

			bool WalkThroughTrees = false;
			bool WalkThroughPlayers = false;
			bool OmniSprint = false;

			bool BigBullet = false;
			bool SilentMelee = false;
			bool AutoSilentMelee = false;
			RustStructs::KeyCode AutoSilentMeleeKey = RustStructs::KeyCode::J;

			bool BrightNight = false;
			bool BrightCave = false;
			bool SkyColor = false;
			bool CloudColor = false;

			bool playerInformation = false;
			RustStructs::KeyCode playerInformationKey = RustStructs::KeyCode::F;

			bool Stars = false;
			bool RemoveShadows = false;
			bool RainbowClouds = false;
			bool NoFog = false;
			bool Spiderman = false;
			bool InfiniteJump = false;
			bool AdminFlags = false;
			bool NoMovementSlowdown = false;
			bool AttackWhilstMounted = false;
			bool AdminCheat = false;
			bool ToggleSprint = false;
			bool CanAttack = false; //not hooked
			bool InstantEoka = false; //no work
			bool HighWalk = false;
			bool MovementLines = false;
			bool RemoveFlyingFlag = false;
			int SelectedHitbox = 0;
			int ManipType = 0;
			int AutoShootType = 0;

			bool HitboxOverride = false;
			bool pierce = false;

			float x = 200.f;
			float y = 200.f;

			bool DrawPlayerChams = false;
			bool ForceField = true;
			bool GradientChams = false;
			//world esp

			bool ShowWorld = false;
			bool StashEsp = false;
			float Max_Stash_Distance = 150;

			bool Skydome = false;

			// Ore
			bool Hemp = false;
			bool OreEsp = false;
			bool StoneOre = false;
			bool MetalOre = false;
			bool SulfurOre = false;
			float Max_Ore_Distance = 150;
			float max_collectable_distance = 150;

			bool HempChams = false;
			bool SulfurChams = false;
			bool StoneChams = false;
			bool MetalChams = false;
			bool StashChams = false;

			float max_flyhack_vertical;
			float vertical_flyhack;
			float max_flyhack_horisontal;
			float horisontal_flyhack;

			int selectedWeaponChams = 0;
			int selectedChams = 0;
			bool Flyhack_Indicators = false;
			bool AntiFlyhack_Kick = false;
			bool AntiDeathBarrier = false;

			bool DrawWeaponChams = false;

			bool Manipulation_Indicator = false;
			bool Manipulation_Dome = false;

			bool StartShooting = false;
			bool AutoShoot = false;
			bool AlwaysAutoshoot = false;
			bool InstantKill = false;
			bool BulletQueue = false;
			bool PowerShot = false;
			RustStructs::KeyCode InstantKillKey = RustStructs::KeyCode::F;
			//int BulletsShot = 5;
			RustStructs::KeyCode AdminCheatKey = RustStructs::KeyCode::C;
			float Simulations = 1000;

			bool HammerSpam = false;
			RustStructs::KeyCode HammerSpamKey = RustStructs::KeyCode::Z;
			int SelectedSpamMaterial = 0;

			float manipsectors = 20;
			float maniprings = 20;

			int manipMode = 0;
			bool Skeleton = false;

			bool DrawBoxesWorld = false;
			bool DrawHealthWorld = false;
			bool ToolCupboard = false;
			bool AnimateMenuBackground = false;
			bool AuthorizedPlayers = false;
			float AuthorizedPlayersDistance = 100;


			bool HempIcon = false;
			bool StoneIcon = false;
			bool MetalIcon = false;
			bool SulfurIcon = false;

			bool ignore_target = false;
			bool Reflect = false;

			bool Outline = false;

			bool BulletTracers = false;
			float TracersDuration = 1.5f;


			bool SilentWalk = false;
			RustStructs::KeyCode SilentWalkKey = RustStructs::KeyCode::N;
			bool LadderShoot = false;
			bool InteractiveDebug = false;
			RustStructs::KeyCode InteractiveKey = RustStructs::KeyCode::F;

			bool Spinbot = false;
			bool AutoUpgrade = false;
			int WorldPg = 0;
			int ColorPg = 0;

			int BuildingGrade = 0;
			bool Suicide = false;
			RustStructs::KeyCode SuicideKey = RustStructs::KeyCode::K;

			bool Speedhack;
			float NoRecoilPercentX = 100.f;
			float NoRecoilPercentY = 100.f;

			float SpeedhackSpeed = 10.f;
			RustStructs::KeyCode SpeedhackKey = RustStructs::KeyCode::C;

			bool WeaponSpam;
			RustStructs::KeyCode WeaponSpamKey = RustStructs::KeyCode::F;

			bool DrawPrediction;
			bool HeliAimbot = false;
			bool HeliMagic = false;
			bool DrawAttackHeli = false;
			bool drawSleepingBag = false;
			bool drawSleepingBagID = false;
			bool AutoFarm = false;

			bool ChatSpam = false;
			RustStructs::KeyCode ChatSpamKey = RustStructs::KeyCode::H;

			bool DisableRecycler = false;
			RustStructs::KeyCode DisableRecyclerKey = RustStructs::KeyCode::J;

			bool PlayerFov = false;
			float FieldOfView = 90.f;
			bool Zoom = false;
			float ZoomAmount = 50.f;
			RustStructs::KeyCode ZoomKey = RustStructs::KeyCode::Mouse1;

			bool TimeChanger = false;
			float Time = 12.f;

			bool DoorSpammer = false;
			bool AutoPickup = false;
			bool RotateBuildings = false;
			bool LongMelee = false;
			bool InstantRevive = false;
			RustStructs::KeyCode InstantReviveKey = RustStructs::KeyCode::E;

			bool HoldVelocity = false;
			RustStructs::KeyCode HoldVelocityKey = RustStructs::KeyCode::E;

			bool FarmBot = false;
			bool HempBot = false;

			bool SignPainter = false;
			RustStructs::KeyCode SignPainterKey = RustStructs::KeyCode::L;
			RustStructs::KeyCode MinicopterKey = RustStructs::KeyCode::F;

			bool MinicopterMagic = false;
			bool ClosestBoneToCrosshair = false;
			bool DroppedItems = false;
			bool Workbench = false;
			bool Teir2 = false;
			bool Teir1 = false;
			bool AutoTurret = false;
			bool AutoTurretRange = true;
			bool ShotgunTrap = false;
			bool BradleyAPC = false;
			bool BradChams = false;
			bool BasicCrate = false;

			bool NormalCrate = false;
			bool MilitaryCrate = false;
			bool EliteCrate = false;
			bool BradleyCrate = false;
			bool HackableCrate = false;

			bool HeliCrate = false;
			bool DieselBarrel = false;
			bool LootBarrel = false;
			bool OilBarrel = false;
			bool Airdrop = false;

			bool Bear = false;
			bool Deer = false;
			bool Wolf = false;
			bool boar = false;
			bool chicken = false;
			bool polarbear = false;
			bool horse = false;
			bool shark = false;

			bool mushroom = false;
			bool pumpkin = false;
			bool corn = false;
			int HitMaterial = 0;
			int pSilentType = 0;
			bool SmallerLocalRadius = false;
			bool AlwaysHeadshotSound = false;
			bool LadderGodMode = false;

			bool FastBullets = false;
			float max_player_distance = 350;
			bool PlayerCorpse = false;
			bool Backpack = false;

			float max_food_distance = 350;
			float max_dropped_distance = 350;
			float max_animal_distance = 350;
			float max_playerdrops_distance = 350;
			float max_crates_distance = 350;
			float max_apc_distance = 350;
			float max_traps_distance = 350;
			float max_world_distance = 350;

			bool KeepAlive = false;
			RustStructs::KeyCode KeepAliveKey = RustStructs::KeyCode::F;

			bool AutoUnhideStashes = false;
			int SelectedFont = 0;
			bool FontPicker = false;

			int SelectedHitsound = 0;
			bool Hitsound = false;
			bool Thickbullet_Indicator = false;
			bool Thickbullet_AutoShoot = false;
			bool waitForInstantHit = false;
			bool DrawBulletTP = false;
			bool ReportPlayer = false;
			bool FixDebugCamera = false;
			RustStructs::KeyCode ReportPlayerKey = RustStructs::KeyCode::R;

			//auto reload shit
			bool just_shot = false;
			bool did_reload = false;
			float time_since_last_shot = 0.0f;
			float fixed_time_last_shot = 0.0f;
			float BulletsQueued = 5;
			float WalkingDesyncAmount = 0.05f;

			float Max_Thickness = 20.f;
			bool TargetLine = false;
			bool ThickBulletLine = false;
			//bool IgoreWhileVisisble = false;
			bool HeliHitboxOverride = false;

			bool DrawRadar = false;
			float RadarSize = 200.f;

			float RadarPosX = 20.f;
			float RadarPosY = 20.f;

			Vector2 RadarPos = { 20.f, 20.f };
			bool ChangeRepeatDelay = false;
			bool HeliBulletQueue = false;
		     bool NameSpoofer = false;
			 bool RandomName = false;
			 bool CustomName = false;

           } m_options = { };
	};

	struct CategoryColors
	{
		struct
		{
			float Fov_Color[4] = { 255.f, 255.f, 255.f, 255.f };
			float Crosshair_Color[4] = { 255.f, 0.0f, 0.0f, 255.f };
			float Sleeper_Color[4] = { 0.0f, 255.f, 255.f, 255.f };
			float DrawBox_Color[4] = { 255.f, 0.0f, 0.0f, 255.f };
			float VisCheck_Color[4] = { 0.f, 255.0f, 0.0f, 255.f };
			float DrawNameTag_Color[4] = { 255.f, 255.f, 255.f, 255.f };
			float DrawDistance_Color[4] = { 255.f, 255.f, 255.f, 255.f };
			float Scientist_color[4] = { 0.f, 17.f, 255.f, 255.f };
			float DrawHeldItem_Color[4] = { 255.f, 255.f, 255.f, 255.f };
			float Wounded_Color[4] = { 245.f, 152.f, 66.f, 255.f };
			float Safezone_Color[4] = { 255.f, 0.0f, 255.f, 255.f };
			float MovementLinesColor[4] = { 255, 0, 76, 255 };
			float TeamMates_Color[4] = { 255.f, 0.0f, 255.f, 255.f };
			float MenuParticleColor[4] = { 255, 0, 76, 255 };
			float background_color2[4] = { 0.1f, 0.1f, 0.1f, 1.f };
			float background_color[4] = { 0.09f, 0.09f, 0.09f, 1.f };
			float MenuColor[4] = { 1.f, 0.f, 0.f, 1.f };
			float TracersColor[4] = { 255.f, 0.0f, 0.0f, 255.f };
			float WeaponChamsColor[4] = { 255.f, 255.f, 255.f, 255.f };

			float HempChamsColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float SulfurChamsColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float StoneChamsColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float MetalChamsColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float StashChamsColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float BradChamsColor[4] = { 255.f, 255.f, 255.f, 255.f };

			float HempColor[4] = { 0.f, 255.f, 0.f, 255.f };
			float StashColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float StoneColor[4] = { 163.f, 158.f, 158.f, 255.f };
			float MetalColor[4] = { 105.f, 104.f, 104.f, 255.f };
			float SulfurColor[4] = { 235.f, 201.f, 52.f, 255.f };
			float PlayerChams_Color[4] = { 255, 255, 255, 255 };
			float ShotgunColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float TurretColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float DroppedItemColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float BradleyColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float BasicCrateColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float NormalCrateColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float EliteCrateColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float MilitaryCrateColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float SupplyDropColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float BradleyCrateColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float HackableCrateColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float HeliCrateColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float LootBarrelColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float OilBarrelColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float DieselBarrelColor[4] = { 255.f, 255.f, 255.f, 255.f };

			float BearColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float PolarBearColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float DeerColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float WolfColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float ChickenColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float BoarColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float HorseColor[4] = { 255.f, 255.f, 255.f, 255.f };

			float MushRoomColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float PumpkinColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float CornColor[4] = { 255.f, 255.f, 255.f, 255.f };

			float CorpseColor[4] = { 255.f, 255.f, 255.f, 255.f };
			float BackPackColor[4] = { 255.f, 255.f, 255.f, 255.f };

			float SkyColor[4] = { 235.f, 52.f, 82.f, 255.f };
			float CloudColor[4] = { 52.f, 235.f, 122.f, 255.f };

			float fontSize = 12.f;

		} m_visuals = { };
	};


}
inline options::CategoryAimbot m_aimbot = {};
inline options::CategoryVisuals m_visuals = { };
inline options::CategoryCommon m_options = { };
inline options::CategoryColors m_colors = { };

struct StorageData
{
	static constexpr auto Signature = HASH("StorageData");

	StorageData(bool initialize)
		: m_signature(initialize ? Signature : 0)
		, m_aimbot()
		, m_visuals()
		, m_options()
		, m_colors()
	{ }

	bool IsValid() const
	{
		return (m_signature == Signature);
	}

	std::uint64_t m_signature = 0;
	options::CategoryAimbot m_aimbot = { };
	options::CategoryVisuals m_visuals = { };
	options::CategoryCommon m_options = { };
	options::CategoryColors m_colors = { };
};

namespace Config
{
	inline bool Load(const std::wstring& name)
	{
		std::wstring directory(SECURE(L"\\SystemRoot\\"));
		directory.append(name);

		if (directory.find(SECURE(L".bin")) == std::wstring::npos)
		{
			// 
			// Add extension to file path
			// 
			directory.append(SECURE(L".bin"));
		}

		win32::File file(directory, true, FILE_READ_DATA);

		if (file.IsOpen())
		{
			StorageData storage(false);

			if (file.Read(&storage, sizeof(storage)))
			{
				if (storage.IsValid())
				{
					std::memcpy(&m_aimbot, &storage.m_aimbot, sizeof(options::CategoryAimbot));
					std::memcpy(&m_visuals, &storage.m_visuals, sizeof(options::CategoryVisuals));
					std::memcpy(&m_options, &storage.m_options, sizeof(options::CategoryCommon));
					std::memcpy(&m_colors, &storage.m_colors, sizeof(options::CategoryColors));

					return true;
				}
				else
				{
					TRACE("%s: Storage data is not valid!", ATOM_FUNCTION);
				}
			}
			else
			{
				TRACE("%s: Couldn't read file!", ATOM_FUNCTION);
			}
		}
		else
		{
			TRACE("%s: Couldn't open file!", ATOM_FUNCTION);
		}

		return false;
	}

	inline bool Save(const std::wstring& name)
	{
		std::wstring directory(SECURE(L"\\SystemRoot\\"));
		directory.append(name);

		if (directory.find(SECURE(L".bin")) == std::wstring::npos)
		{
			// 
			// Add extension to file path
			// 
			directory.append(SECURE(L".bin"));
		}

		win32::File file(directory, true, FILE_READ_DATA | FILE_WRITE_DATA);

		if (file.IsOpen())
		{
			StorageData storage(true);

			std::memcpy(&storage.m_aimbot, &m_aimbot, sizeof(options::CategoryAimbot));
			std::memcpy(&storage.m_visuals, &m_visuals, sizeof(options::CategoryVisuals));
			std::memcpy(&storage.m_options, &m_options, sizeof(options::CategoryCommon));
			std::memcpy(&storage.m_colors, &m_colors, sizeof(options::CategoryColors));

			if (file.Write(&storage, sizeof(storage)))
			{
				return true;
			}
			else
			{
				TRACE("%s: Couldn't write file!", ATOM_FUNCTION);
			}
		}
		else
		{
			TRACE("%s: Couldn't open file!", ATOM_FUNCTION);
		}

		return false;
	}

}