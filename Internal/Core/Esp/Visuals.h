#pragma once
#include "../RustSDK/AssemblyCSharp/AssemblyCSharp.h"
#include "../RustSDK/UnityEngine/UnityEngine.h"
#include "../RustSDK/System/System.h"

class VectorList {
public:
	VectorList(AssemblyCSharp::BasePlayer* _This)
	{
		if (memory::IsAddressValid(_This))
		{
			this->CachedEntity = _This;
		}
	}

	AssemblyCSharp::BasePlayer* CachedEntity;

	// Equality comparison operator
	bool operator==(const VectorList& other) const {
		// Define your comparison logic here
		return (CachedEntity == other.CachedEntity);
	}
};
inline const wchar_t* ConvertToWideString(const char* str)
{
	// Get the length of the wide string
	size_t length = g_map_data.mbstowcs(NULL, str, 0);

	// Allocate memory for the wide string
	wchar_t* wideStr = new wchar_t[length + 1];

	// Convert the string to wide string
	g_map_data.mbstowcs(wideStr, str, length + 1);

	return wideStr;
}

inline std::vector< VectorList> vector_list;
inline std::vector< VectorList> vector_list_Temp;

class PlayerList {
public:
	PlayerList(AssemblyCSharp::BasePlayer* _This)
	{
		if (memory::IsAddressValid(_This))
		{
			this->CachedPlayer = _This;
		}
	}

	AssemblyCSharp::BasePlayer* CachedPlayer;

	// Equality comparison operator
	bool operator==(const PlayerList& other) const {
		// Define your comparison logic here
		return (CachedPlayer == other.CachedPlayer);
	}
};


inline std::vector< PlayerList> PlayerVectorList;
inline std::vector< PlayerList> PlayerVectorListTemp;

class PositionList {
public:
	PositionList(Vector3 _This)
	{
		CachedPos = _This;
	}

	Vector3 CachedPos;

	// Equality comparison operator
	bool operator==(const PositionList& other) const {
		// Define your comparison logic here
		return (CachedPos == other.CachedPos);
	}
};

inline std::vector< PositionList> pos_list;
inline std::vector< PositionList> pos_list_Temp;

inline UnityEngine::AssetBundle* g_pixel_font_bundle;
inline UnityEngine::AssetBundle* g_proggy_font_bundle;
inline UnityEngine::AssetBundle* g_default_font_bundle;
inline UnityEngine::AssetBundle* force_field_bundle;
inline UnityEngine::AssetBundle* colorchams_bundle;
inline UnityEngine::AssetBundle* hitsound_bundle;

inline UnityEngine::Shader* forcefield_shader;
inline UnityEngine::Material* forcefield_material;

inline UnityEngine::Shader* colorchams_shader;
inline UnityEngine::Material* colorchams_material;
inline UnityEngine::AssetBundle* TestBundlee = nullptr;
inline UnityEngine::AssetBundle* MenuIconBundles = nullptr;
inline UnityEngine::Texture2D* peter_texture1 = nullptr;
inline UnityEngine::Texture2D* peter_texture2 = nullptr;

class Visuals {
public:
	void CacheEntites();
	void CachePlayers();
	void DrawEntities();
	void DrawPlayers();
public:
	static inline UnityEngine::Camera* m_camera = nullptr;
	static inline Vector3 m_camera_position = Vector3(0.f, 0.f, 0.f);
	inline static AssemblyCSharp::BasePlayer* m_local_player = nullptr;
} inline g_visuals;

inline UnityEngine::GUIStyle* gui_style = nullptr;
inline UnityEngine::GUISkin* gui_skin = nullptr;


inline UnityEngine::Texture2D* white_texture;
//inline UnityEngine::AssetBundle* font_bundle = nullptr;
inline void fill_box2(UnityEngine::rect_t pos, Color clr)
{
	if (!white_texture)
		white_texture = UnityEngine::Texture2D::get_whiteTexture();

	UnityEngine::GUI::SetColor(clr.GetUnityColor());
	UnityEngine::GUI::DrawTexture(pos, white_texture);
}

inline void fill_box(UnityEngine::rect_t pos, Color clr)
{
	if (!white_texture)
		white_texture = UnityEngine::Texture2D::get_whiteTexture();

	UnityEngine::GUI::SetColor(clr.GetUnityColor());
	UnityEngine::GUI::DrawTexture(pos, white_texture);
}

inline void outline_box(Vector2 pos, Vector2 size, Color clr)
{
	UnityEngine::GUI::SetColor(clr.GetUnityColor());
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y, 1, size.y), white_texture);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x + size.x, pos.y, 1, size.y), white_texture);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y, size.x, 1), white_texture);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y + size.y, size.x, 1), white_texture);
}

inline void fill_box_(UnityEngine::rect_t pos, Color clr)
{
	if (!white_texture)
		white_texture = UnityEngine::Texture2D::get_whiteTexture();

	UnityEngine::GUI::SetColor(clr);
	UnityEngine::GUI::DrawTexture(pos, white_texture);
}

inline void outline_box_(Vector2 pos, Vector2 size, Color clr)
{
	UnityEngine::GUI::SetColor(clr);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y, 1, size.y), white_texture);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x + size.x, pos.y, 1, size.y), white_texture);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y, size.x, 1), white_texture);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y + size.y, size.x, 1), white_texture);
}

inline void outline_box_half(Vector2 pos, Vector2 size, Color clr)
{
	UnityEngine::GUI::SetColor(clr);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y, 0.5f, size.y), white_texture);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x + size.x, pos.y, 0.5f, size.y), white_texture);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y, size.x, 0.5f), white_texture);
	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y + size.y, size.x, 0.5f), white_texture);
}

inline void vertical_line(Vector2 pos, float size, Color clr)
{
	UnityEngine::GUI::SetColor(clr);

	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y, 1, size), white_texture);
}

inline void horizontal_line(Vector2 pos, float size, Color clr)
{
	UnityEngine::GUI::SetColor(clr);

	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(pos.x, pos.y, size, 1), white_texture);
}




inline auto calculate_text_size(Unity_String text) -> Vector2 {
	if (!gui_style)
		return {};
	const auto content = UnityEngine::GUIContent::Temp(text);
	Vector2 size;
	gui_style->Internal_CalcSize_Injected((UnityEngine::GUIContent*)content, size);
	return size;
}


inline std::vector< std::wstring > test2;

inline void Text(float x, float y, float m_flWidth, float m_flHeight, Unity_String text, Color clr, bool centered = false, bool m_bOutlined = true, float size = 12) {
	gui_style->SetFontSize(size);
	float w = 200.f;
	float h = 200.f;

	if (centered) {
		UnityEngine::GUIStyle::SetAlignment(gui_style, 0x4);
		if (m_bOutlined)
		{
			UnityEngine::GUI::SetColor(Color(0.f, 0.f, 0.f, 255.f));

			UnityEngine::GUI::Label(UnityEngine::rect_t{ x - 101, y - 100, w, h }, text, (uintptr_t)gui_style);
			UnityEngine::GUI::Label(UnityEngine::rect_t{ x - 100, y - 101, w, h }, text, (uintptr_t)gui_style);
			UnityEngine::GUI::Label(UnityEngine::rect_t{ x - 99, y - 100, w, h }, text, (uintptr_t)gui_style);
			UnityEngine::GUI::Label(UnityEngine::rect_t{ x - 100, y - 99, w, h }, text, (uintptr_t)gui_style);

		}
		UnityEngine::GUI::SetColor(clr);

		UnityEngine::GUI::Label(UnityEngine::rect_t{ x - 100, y - 100, w, h }, text, (uintptr_t)gui_style);
	}
	else {
		UnityEngine::GUIStyle::SetAlignment(gui_style, 0x0);
		if (m_bOutlined)
		{
			UnityEngine::GUI::SetColor(Color(0.f, 0.f, 0.f, 255.f));

			UnityEngine::GUI::Label(UnityEngine::rect_t{ x - 1, y - 1, w, h }, text, (uintptr_t)gui_style);
			UnityEngine::GUI::Label(UnityEngine::rect_t{ x + 1, y + 1, w, h }, text, (uintptr_t)gui_style);
			UnityEngine::GUI::Label(UnityEngine::rect_t{ x - 1, y + 1, w, h }, text, (uintptr_t)gui_style);
			UnityEngine::GUI::Label(UnityEngine::rect_t{ x + 1, y - 1, w, h }, text, (uintptr_t)gui_style);

		}

		UnityEngine::GUI::SetColor(clr);

		UnityEngine::GUI::Label(UnityEngine::rect_t{ x, y, w, h }, text, (uintptr_t)gui_style);
	}

}

inline void TextTest(float x, float y, float m_flWidth, float m_flHeight, std::string text, Color clr, bool centered = false, bool m_bOutlined = true, float size = 12) {
	gui_style->SetFontSize(size);
	float w = 200.f;
	float h = 200.f;
	
	if (centered) {
		UnityEngine::GUIStyle::SetAlignment(gui_style, 0x4);
		if (m_bOutlined)
		{
			UnityEngine::GUI::SetColor(Color(0.f, 0.f, 0.f, 255.f)); 

			UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x - 101, y - 100, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);
			UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x - 100, y - 101, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);
			UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x - 99, y - 100, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);
			UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x - 100, y - 99, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);

		}
		UnityEngine::GUI::SetColor(clr);

		UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x - 100, y - 100, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);
	}
	else {
		UnityEngine::GUIStyle::SetAlignment(gui_style, 0x0);
		if (m_bOutlined)
		{
			UnityEngine::GUI::SetColor(Color(0.f, 0.f, 0.f, 255.f));

			UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x - 1, y - 1, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);
			UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x + 1, y + 1, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);
			UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x - 1, y + 1, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);
			UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x + 1, y - 1, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);

		}

		UnityEngine::GUI::SetColor(clr);

		UnityEngine::GUI::LabelTest(UnityEngine::rect_t{ x, y, w, h }, CIl2Cpp::il2cpp_string_new(text.c_str()), (uintptr_t)gui_style);
	}

}

