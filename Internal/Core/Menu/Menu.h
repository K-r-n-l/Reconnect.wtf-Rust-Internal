#pragma once
#include "../RustSDK/UnityEngine/UnityEngine.h"
#include "../Settings/Settings.hpp"
#include "../RustHooks/OnGUI/OnGUI.h"
#include "../Esp/Visuals.h"
#include <unordered_set>

class Menu {
public:

	static void RenderMenu(UnityEngine::Event* m_event);
	static void ChineseRenderMenu(UnityEngine::Event* m_event);
};



inline void AnsiToUnicode(const char* m_szAnsi, wchar_t* m_pBuffer)
{
	for (int i = 0; ; ++i)
	{
		m_pBuffer[i] = static_cast<char>(m_szAnsi[i]);

		if (!m_szAnsi[i])
			return;
	}
}


inline float accent_color_[4] = { 255, 16, 210, 255 };
inline float menu_alpha;
inline static bool wakeup{ false };
inline static bool sex{ false };
inline Vector2 window_pos{ 50, 50 }, mouse_pos, next_item_pos, next_color_pos;
inline Vector2 window_sizemain;
inline Vector2 old_mouse_pos;
inline bool mouse_state, insert_state;
inline RustStructs::EventType menu_event;
inline bool main_menu_open = false;
inline bool old_mouse_state, old_insert_state;
inline RustStructs::KeyCode key_code;
inline Vector2 widget_mouse_pos, old_widget_mouse_pos, next_tab_pos;
inline bool widget_state, old_widget_state;
inline float color_picker_alpha;
inline bool is_menu_open{ true };
inline bool interacting;
inline 	bool InsertPressed;
inline 	bool LMBPressed;

inline const wchar_t* keybind_str;
inline int* kbref = 0;
inline int* ref_clicked = 0;
inline bool getting_keybind = false;
inline bool combo_clicked = false;
inline bool true_placehold = true;
inline bool false_placehold = false;
inline bool _ti = false;
inline bool inputting_text = false;
inline RustStructs::KeyCode iSelectedg = RustStructs::KeyCode::Insert;
inline Vector2 window_position = { 650, 200 };
inline Vector2 lmp = { 650, 200 };


inline void OnKeyUp(RustStructs::KeyCode key) {
	if (key == RustStructs::KeyCode::Insert) {
		InsertPressed = false;
	}
}

inline void OnKeyDown(RustStructs::KeyCode key) {
	if (!InsertPressed && key == RustStructs::KeyCode::Insert) {
		InsertPressed = true;
		m_visuals.m_visuals.ShowMenu = !m_visuals.m_visuals.ShowMenu;
		if (!m_visuals.m_visuals.ShowMenu) getting_keybind = false;
	}

	if (getting_keybind && (int)(key) != 0)
	{
		*kbref = (int)(key);
		getting_keybind = false;
		//keybind_map[keybind_str] = key;
	}

}
inline bool is_mouse_in_box(const Vector2& pos1, const Vector2& pos2) {
	if (old_mouse_pos.x < pos1.x || old_mouse_pos.y < pos1.y)
		return false;

	if (old_mouse_pos.x > pos2.x || old_mouse_pos.y > pos2.y)
		return false;

	return true;
}

inline float opacity = 0.f;
inline int active_tab = 0;
inline Vector2 tab_size;
inline int last_active_id = 0;
inline bool hover_element = false;
inline bool list_clicked = false;
inline bool window(UnityEngine::Event* event, wchar_t* title, const Vector2& pos, const Vector2& window_size, bool always_open) {
	if (!sex) {
		window_pos = pos;
		sex = true;
	}
	auto mouse = UnityEngine::Input::GetMousePosition();
	auto width = UnityEngine::Screen::get_width();
	auto height = UnityEngine::Screen::get_height();
	window_sizemain = window_size;
	menu_event = event->Type();
	key_code = UnityEngine::Event::get_keyCode(event);
	insert_state = menu_event == RustStructs::EventType::KeyDown && key_code == RustStructs::KeyCode::Insert;
	mouse_pos.x = UnityEngine::Input::GetMousePosition().x;
	mouse_pos.y = UnityEngine::Screen::get_height() - UnityEngine::Input::GetMousePosition().y;
		mouse_state = UnityEngine::Input::GetMouseButton(0);
	if (insert_state && !old_insert_state && !always_open)
		is_menu_open = !is_menu_open;
	if (is_mouse_in_box({ window_pos.x, window_pos.y }, { window_pos.x + window_size.x, window_pos.y + window_size.y }) && mouse_state && main_menu_open && !hover_element) {
		window_pos.x += mouse_pos.x - old_mouse_pos.x;
		window_pos.y += mouse_pos.y - old_mouse_pos.y;
	}
	else {
		hover_element = false;
	}
	if (menu_event == RustStructs::EventType::Repaint)
	{
		/* Its main form */
		fill_box({ window_pos.x, window_pos.y , window_size.x,window_size.y }, Color(22.f, 22.f, 22.f, 255.f));
		fill_box({ window_pos.x, window_pos.y , window_size.x,25 }, Color(28, 28, 28, 255.f));
		//UnityEngine::GL::GradientFillRect({ window_pos.x, window_pos.y }, { window_size.x,25 }, Color(38.f, 40.f, 42.f, 155), Color(22.f, 22.f, 22.f, 55));
		fill_box({ window_pos.x, window_pos.y + 25 ,window_size.x,46 }, Color(28, 28, 28, 255.f));

		//Text(window_pos.x + window_size.x - window_size.x / 2 + 2, window_pos.y + 12.5, 200, 20, (title), Color(58.f, 197.f, 129.f, 255.f), true, true);
	}
	next_tab_pos = { window_pos.x, window_pos.y + 28 };
	return true;
}

inline void end_window() {
	old_insert_state = insert_state;
	old_mouse_pos = mouse_pos;
	old_mouse_state = mouse_state;
}
inline int table = 0;
inline int activetab = 0;
inline auto resolve_icon(int index) -> const char* {

	switch (index)
	{
	case 0:
		return SECURE("accuracy.png");
	case 1:
		return SECURE("eye.png");
	case 2:
		return SECURE("settings.png");
	case 3:
		return SECURE("download (3).png");
	case 4:
		return SECURE("palette.png");
	}

	return SECURE("");
}

inline void Tab(const wchar_t* name, int id, Vector2 tab_size = Vector2(112, 35)) {
	if (is_mouse_in_box({ next_tab_pos.x + 15, next_tab_pos.y + 5 }, { next_tab_pos.x + 15 + tab_size.x,next_tab_pos.y + 5 + tab_size.y }) && mouse_state && !old_mouse_state && !interacting)
	{
		activetab = id;
		hover_element = true;
	}

	bool isHovered = is_mouse_in_box({ next_tab_pos.x + 15, next_tab_pos.y + 5 }, { next_tab_pos.x + 15 + tab_size.x,next_tab_pos.y + 5 + tab_size.y });
	bool isActive = activetab == id;
	if (menu_event == RustStructs::EventType::Repaint)
	{
		Color txt2 = isHovered ? Color(77, 75, 75, 255.f) : Color(accent_color_[0], accent_color_[1], accent_color_[2], accent_color_[3]);
		Color clrr;
		if (isActive)
		{
			clrr = isActive ? Color(77, 75, 75, 255.f) : txt2;
		}
		else
		{
			clrr = isHovered ? Color(77, 75, 75, 255.f) : Color(accent_color_[0], accent_color_[1], accent_color_[2], accent_color_[3]);
		}

		
		//auto texture = MenuIconBundles->LoadAsset<UnityEngine::Texture2D>(resolve_icon(id), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture"))));
		//
		//UnityEngine::GUI::SetColor(clrr.GetUnityColor());
		//UnityEngine::GUI::DrawTexture({ next_tab_pos.x + 5 + tab_size.x / 2 - 30, next_tab_pos.y + tab_size.y / 2 - 30, 45.f, 45.f }, texture);

		////Text(next_tab_pos.x + 5 + tab_size.x / 2 - 5, next_tab_pos.y + tab_size.y / 2, tab_size.x, tab_size.y, (name), txt.GetUnityColor(), true, true);
		//if (id == activetab) {



		//	auto texture = MenuIconBundles->LoadAsset<UnityEngine::Texture2D>(resolve_icon(id), (Il2CppType*)CIl2Cpp::FindType(CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture"))));


		//	UnityEngine::GUI::SetColor(txt);
		//	UnityEngine::GUI::DrawTexture({ next_tab_pos.x + 5 + tab_size.x / 2 - 30, next_tab_pos.y + tab_size.y / 2 - 30, 48.f, 48.f }, texture);

		//	//outline_box({ next_tab_pos.x, next_tab_pos.y }, { tab_size.x - 1, tab_size.y }, Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f));
		Text(next_tab_pos.x + 5 + tab_size.x / 2 - 5, next_tab_pos.y + tab_size.y / 2, tab_size.x, tab_size.y, (name), clrr.GetUnityColor(), true, true);
		//}
	}
	next_tab_pos.x += 112;
}

inline char* itoass(int m_iValue, char* m_pBuffer, int m_iRadix)
{
	auto xtoa = [](int a1, char* a2, int a3, bool a4) -> char
	{
		char* v5; // r10
		char* v6; // r8
		char* v7; // r11
		char* v8; // r9
		unsigned int v9; // edx
		char v10; // al
		char result; // al
		char v12; // cl

		v5 = a2;
		if (a4)
		{
			v5 = a2 + 1;
			*a2 = 45;
			a1 = -a1;
		}
		v6 = v5;
		do
		{
			v7 = v5 + 1;
			v8 = v5;
			v9 = a1 % a3;
			a1 /= a3;
			v10 = 87;
			if (v9 <= 9)
				v10 = 48;
			*v5++ = v9 + v10;
		} while (a1);
		*v7 = 0;
		do
		{
			result = *v6;
			v12 = *v8;
			*v8-- = *v6;
			*v6++ = v12;
		} while (v6 < v8);
		return result;
	};

	bool v3; // r9

	v3 = false;
	if (m_iRadix == 10 && m_iValue < 0)
		v3 = true;
	xtoa(m_iValue, m_pBuffer, m_iRadix, v3);
	return m_pBuffer;
}
inline unsigned long long iCurrentOpenedCombo = 0;
inline Vector2 vecLastCringePosition = { 0, 0 };
inline int iOpenedCringeY = 0;
inline int iLastCringeAmount = 0;
inline bool bComboOpenedInThisFrame = false;
static void DrawCheckCombo(float x, float y, const Color& Color)
{
	fill_box({ x, y , 1, 3 }, Color);
	fill_box({ x - 1, y - 1 , 1, 3 }, Color);
	fill_box({ x - 2, y - 2 , 1, 3 }, Color);
	fill_box({ x - 3, y - 3 , 1, 3 }, Color);
	fill_box({ x, y , 1, 3 }, Color);
	fill_box({ x + 1, y + 1 , 1, 3 }, Color);
	fill_box({ x + 2, y + 2 , 1, 3 }, Color);
	fill_box({ x + 3, y + 1 , 1, 3 }, Color);
	fill_box({ x + 4, y , 1, 3 }, Color);
	fill_box({ x + 5, y - 1 , 1, 3 }, Color);
	fill_box({ x + 6, y - 2 , 1, 3 }, Color);
	fill_box({ x + 7, y - 3 , 1, 3 }, Color);

}

inline const wchar_t* GetKey(RustStructs::KeyCode iKey)
{
	switch (iKey)
	{
	case 0:
		return L"-";
	case 8:
		return L"Backspace";
	case 127:
		return L"Delete";
	case 9:
		return L"Tab";
	case 12:
		return L"Clear";
	case 13:
		return L"Return";
	case 19:
		return L"Pause";
	case 27:
		return L"Escape";
	case 32:
		return L"Space";
	case 256:
		return L"Keypad0";
	case 257:
		return L"Keypad1";
	case 258:
		return L"Keypad2";
	case 259:
		return L"Keypad3";
	case 260:
		return L"Keypad4";
	case 261:
		return L"Keypad5";
	case 262:
		return L"Keypad6";
	case 263:
		return L"Keypad7";
	case 264:
		return L"Keypad8";
	case 265:
		return L"Keypad9";
	case 266:
		return L"KeypadPeriod";
	case 267:
		return L"KeypadDivide";
	case 268:
		return L"KeypadMultiply";
	case 269:
		return L"KeypadMinus";
	case 270:
		return L"KeypadPlus";
	case 271:
		return L"KeypadEnter";
	case 272:
		return L"KeypadEquals";
	case 273:
		return L"UpArrow";
	case 274:
		return L"DownArrow";
	case 275:
		return L"RightArrow";
	case 276:
		return L"LeftArrow";
	case 277:
		return L"Insert";
	case 278:
		return L"Home";
	case 279:
		return L"End";
	case 280:
		return L"PageUp";
	case 281:
		return L"PageDown";
	case 282:
		return L"F1";
	case 283:
		return L"F2";
	case 284:
		return L"F3";
	case 285:
		return L"F4";
	case 286:
		return L"F5";
	case 287:
		return L"F6";
	case 288:
		return L"F7";
	case 289:
		return L"F8";
	case 290:
		return L"F9";
	case 291:
		return L"F10";
	case 292:
		return L"F11";
	case 293:
		return L"F12";
	case 294:
		return L"F13";
	case 295:
		return L"F14";
	case 296:
		return L"F15";
	case 48:
		return L"Alpha0";
	case 49:
		return L"Alpha1";
	case 50:
		return L"Alpha2";
	case 51:
		return L"Alpha3";
	case 52:
		return L"Alpha4";
	case 53:
		return L"Alpha5";
	case 54:
		return L"Alpha6";
	case 55:
		return L"Alpha7";
	case 56:
		return L"Alpha8";
	case 57:
		return L"Alpha9";
	case 33:
		return L"Exclaim";
	case 34:
		return L"DoubleQuote";
	case 35:
		return L"Hash";
	case 36:
		return L"Dollar";
	case 37:
		return L"Percent";
	case 38:
		return L"Ampersand";
	case 39:
		return L"Quote";
	case 40:
		return L"LeftParen";
	case 41:
		return L"RightParen";
	case 42:
		return L"Asterisk";
	case 43:
		return L"Plus";
	case 44:
		return L"Comma";
	case 45:
		return L"Minus";
	case 46:
		return L"Period";
	case 47:
		return L"Slash";
	case 58:
		return L"Colon";
	case 59:
		return L"Semicolon";
	case 60:
		return L"Less";
	case 61:
		return L"Equals";
	case 62:
		return L"Greater";
	case 63:
		return L"Question";
	case 64:
		return L"At";
	case 91:
		return L"LeftBracket";
	case 92:
		return L"Backslash";
	case 93:
		return L"RightBracket";
	case 94:
		return L"Caret";
	case 95:
		return L"Underscore";
	case 96:
		return L"BackQuote";
	case 97:
		return L"A";
	case 98:
		return L"B";
	case 99:
		return L"C";
	case 100:
		return L"D";
	case 101:
		return L"E";
	case 102:
		return L"F";
	case 103:
		return L"G";
	case 104:
		return L"H";
	case 105:
		return L"I";
	case 106:
		return L"J";
	case 107:
		return L"K";
	case 108:
		return L"L";
	case 109:
		return L"M";
	case 110:
		return L"N";
	case 111:
		return L"O";
	case 112:
		return L"P";
	case 113:
		return L"Q";
	case 114:
		return L"R";
	case 115:
		return L"S";
	case 116:
		return L"T";
	case 117:
		return L"U";
	case 118:
		return L"V";
	case 119:
		return L"W";
	case 120:
		return L"X";
	case 121:
		return L"Y";
	case 122:
		return L"Z";
	case 123:
		return L"LeftCurlyBracket";
	case 124:
		return L"Pipe";
	case 125:
		return L"RightCurlyBracket";
	case 126:
		return L"Tilde";
	case 300:
		return L"Numlock";
	case 301:
		return L"CapsLock";
	case 302:
		return L"ScrollLock";
	case 303:
		return L"RightShift";
	case 304:
		return L"LeftShift";
	case 305:
		return L"RightControl";
	case 306:
		return L"LeftControl";
	case 307:
		return L"RightAlt";
	case 308:
		return L"LeftAlt";
	case 309:
		return L"Left Windows";
	case 310:
		return L"Right Windows";
	case 311:
		return L"LeftWindows";
	case 312:
		return L"RightWindows";
	case 313:
		return L"AltGr";
	case 315:
		return L"Help";
	case 316:
		return L"Print";
	case 317:
		return L"SysReq";
	case 318:
		return L"Break";
	case 319:
		return L"Menu";
	case 323:
		return L"Mouse0";
	case 324:
		return L"Mouse1";
	case 325:
		return L"Mouse2";
	case 326:
		return L"Mouse3";
	case 327:
		return L"Mouse4";
	case 328:
		return L"Mouse5";
	case 329:
		return L"Mouse6";
	case 900:
		return L"Left mouse";
	case 901:
		return L"Right mouse";
	case 902:
		return L"Middle button";
	}

	/* Convert text */
	char szValue[64];
	wchar_t wszValue[64];
	itoass(static_cast<float>(iKey), szValue, 10);
	AnsiToUnicode(szValue, wszValue);

	return wszValue;
}


inline unsigned long long iCurrentSelectedKeybind = 0;

inline void NewKeybind(const wchar_t* szTitle, RustStructs::KeyCode& iKey)
{
	static bool bWaited = false;

	bool bCringe = false;

	if (bWaited && iCurrentSelectedKeybind != constant::Hash(szTitle, false))
		bCringe = true;

	if (bWaited && !bCringe)
	{
		iKey = RustStructs::Nonez;
		/* Handle mouse */
		if (UnityEngine::Input::GetMouseButton(0))
		{
			iKey = RustStructs::Mouse0;
			bWaited = false;
		}
		else if (UnityEngine::Input::GetMouseButton(1))
		{
			iKey = RustStructs::Mouse1;
			bWaited = false;
		}
		else if (UnityEngine::Input::GetMouseButton(2))
		{
			iKey = RustStructs::Mouse3;
			bWaited = false;
		}
		else if (UnityEngine::Input::GetMouseButton(4))
		{
			iKey = RustStructs::Mouse4;
			bWaited = false;
		}

		if (menu_event == RustStructs::EventType::KeyUp)
		{
			iKey = (key_code);
			bWaited = false;
		}
	}

	bool bHovered = false;

	if (is_mouse_in_box(Vector2(next_item_pos.x, next_item_pos.y + 20), Vector2(next_item_pos.x + 80, next_item_pos.y + 20 + 28)))
	{
		iCurrentSelectedKeybind = constant::Hash(szTitle, false);
		bHovered = true;

		if (mouse_state && (!old_mouse_state || !bComboOpenedInThisFrame) && !interacting) {
			bWaited = true;
			hover_element = true;

		}
	}
	/* Dont draw if combo opened */
	if (bComboOpenedInThisFrame && vecLastCringePosition.x == next_item_pos.x && vecLastCringePosition.y > next_item_pos.y + 24 + 20 + 6 && iOpenedCringeY < next_item_pos.y)
	{
		//	next_item_pos.y += 38;
		return;
	}

	if (menu_event == RustStructs::EventType::Repaint)
	{
		Text(next_item_pos.x, next_item_pos.y, 100, 100, szTitle, Color(189.f, 189.f, 189.f, 255.f), false, false, 12.f);
		fill_box({ next_item_pos.x , next_item_pos.y + 20, 80 , 20 }, Color(38.f, 40.f, 42.f, 0.68 * 255.f));

		Color cNotReadColor = bHovered ? Color(189.f, 189.f, 189.f, 255.f) : Color(255.f, 255.f, 255.f, 0.6 * 255.f);
		Color cTextColor = bWaited ? Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f) : cNotReadColor;

		auto key = iKey == 0 ? std::wstring(SECURE(L"no key")) : std::wstring(SECURE(L"key: ")) + GetKey((RustStructs::KeyCode)iKey);


		Text(next_item_pos.x + static_cast<float>(80) / 2, next_item_pos.y + 30, 80, 28, key.c_str(), cTextColor, true, false, 12.f);
	}


	next_item_pos.y += 45;

}

inline void ComboBox(const wchar_t* szTitle, int& iSelected, Unity_String aNames[], int iElementsAmount)
{
	static bool bIsOpened = false;



	bool bCringe = false;

	if (bIsOpened && iCurrentOpenedCombo != constant::Hash(szTitle, false))
		bCringe = true;

	if (!bIsOpened && bComboOpenedInThisFrame)
		bComboOpenedInThisFrame = false;

	if (bIsOpened && !bCringe)
	{

		Vector2 vecNewBarPosition = { next_item_pos.x, next_item_pos.y + 24 + 20 + 6 };
		for (int iElement = 0; iElement < iElementsAmount; iElement++)
		{
			if (is_mouse_in_box(vecNewBarPosition, Vector2(vecNewBarPosition.x + 220, vecNewBarPosition.y + 20)) && mouse_state && !old_mouse_state && !interacting)
			{
				iSelected = iElement;
				bIsOpened = false;
				break;
			}

			vecNewBarPosition.y += 20;
		}

	}

	/* Check if mouse is in box */
	if (is_mouse_in_box({ next_item_pos.x - 1, next_item_pos.y + 24 }, { next_item_pos.x + 220, next_item_pos.y + 40 }) && !bCringe)
	{
		/* Check if mouse is down */
		if (mouse_state && !old_mouse_state && !interacting)
		{
			iCurrentOpenedCombo = constant::Hash(szTitle, false);

			iOpenedCringeY = next_item_pos.y + 24 + 20 + 6;

			/* Set opened to true */
			bIsOpened = !bIsOpened;

			if (bIsOpened)
				bComboOpenedInThisFrame = true;
		}
	}
	else if (mouse_state && !old_mouse_state && !interacting && !bCringe)
	{
		bIsOpened = false;
	}



	if (vecLastCringePosition.x == next_item_pos.x && vecLastCringePosition.y > next_item_pos.y + 24 + 20 + 6 && iOpenedCringeY < next_item_pos.y && bCringe)
	{
		next_item_pos.y += 24 + 20 + 6 + 20;
		return;
	}

	if (menu_event == RustStructs::EventType::Repaint)
	{
		Text(next_item_pos.x, next_item_pos.y, 300, 100, szTitle, { 189.f, 189.f, 189.f, 255.f }, false, false, 12.f);
		Color cBoxColor = bIsOpened && !bCringe ? Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f) : Color(224.f, 224.f, 224.f, 200.f);
		fill_box2(UnityEngine::rect_t{ next_item_pos.x, next_item_pos.y + 24, 244 , 20 }, Color(38.f, 40.f, 42.f, 255.f));
		DrawCheckCombo(next_item_pos.x + 244 - 15, next_item_pos.y + 33, cBoxColor);
		Color cTextColor = bIsOpened && !bCringe ? Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f) : Color(255.f, 255.f, 255.f, 255.f);
		Text(next_item_pos.x + 6, next_item_pos.y + 24, 300, 100, aNames[iSelected], cTextColor, false, false, 12.f);
		if (bIsOpened && !bCringe)
		{
			Vector2 vecNewBarPosition = { next_item_pos.x, next_item_pos.y + 24 + 20 + 6 };
			for (int iElement = 0; iElement < iElementsAmount; iElement++)
			{
				auto sElement = aNames[iElement];
				fill_box2(UnityEngine::rect_t{ vecNewBarPosition.x, vecNewBarPosition.y, 244 , 20 }, Color(38.f, 40.f, 42.f, 255.f));

				Color cTextColor = iElement == iSelected ? Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f) : Color(255.f, 255.f, 255.f, 255.f);
				Text(vecNewBarPosition.x + 6, vecNewBarPosition.y, 300, 100, sElement, cTextColor, false, false, 12.f);
				vecNewBarPosition.y += 20;
			}
			vecLastCringePosition = vecNewBarPosition;
			iLastCringeAmount = iElementsAmount;
		}
	}
	next_item_pos.y += 24 + 30;
}
inline void BeginChild(const wchar_t* title, const Vector2& pos, const Vector2& size)
{
	if (menu_event == RustStructs::EventType::Repaint)
	{
		fill_box({ window_pos.x + pos.x,  window_pos.y + pos.y , size.x, size.y }, Color(28, 28, 28, 255.f));
		outline_box_half({ window_pos.x + pos.x,  window_pos.y + pos.y }, { size.x, size.y }, Color(accent_color_[0], accent_color_[1], accent_color_[2], 66.f));

		Text(window_pos.x + pos.x + 15.f, window_pos.y + pos.y - 10.f, 300, 20, title, Color(accent_color_[0], accent_color_[1], accent_color_[2], accent_color_[3]), false, true, 12);
	}
	next_item_pos = { window_pos.x + pos.x + 10, window_pos.y + pos.y + 15 };
}
inline void BeginChildÒ(const wchar_t* title, const Vector2& pos, const Vector2& size, bool no_backround = false)
{
	if (!no_backround) {
		if (menu_event == RustStructs::EventType::Repaint)
		{
			fill_box({ window_pos.x + pos.x, window_pos.y + pos.y , size.x + 20,size.y + 20 }, Color(22.f, 22.f, 22.f, 255.f));

			fill_box({ window_pos.x + pos.x + 10,  window_pos.y + pos.y + 10 , size.x, size.y }, Color(28, 28, 28, 255.f));

			Text(window_pos.x + pos.x + 25.f, window_pos.y + pos.y, 300, 20, title, Color(255, 255, 255, 255.f), false, false, 12);
		}
	}
	next_color_pos = { window_pos.x + pos.x + 20, window_pos.y + pos.y + 20 };
}
static void Arrow(float x, float y, const Color& Color)
{
	//fill_box({ x, y }, { 1, 3 }, Color);
	//fill_box({ x - 1, y - 1 }, { 1, 3 }, Color);
	//fill_box({ x - 2, y - 2 }, { 1, 3 }, Color);
	//fill_box({ x - 3, y - 3 }, { 1, 3 }, Color);
	fill_box({ x, y , 1, 3 }, Color);
	fill_box({ x + 1, y + 1 , 1, 3 }, Color);
	fill_box({ x + 2, y + 2 , 1, 3 }, Color);
	fill_box({ x + 3, y + 1 , 1, 3 }, Color);
	fill_box({ x + 4, y , 1, 3 }, Color);
	fill_box({ x + 5, y - 1 , 1, 3 }, Color);
	fill_box({ x + 6, y - 2 , 1, 3 }, Color);
	fill_box({ x + 7, y - 3 , 1, 3 }, Color);

}
inline float alpha_test = 0.f;


inline void CreateColorPickerTexture() {



	//UnityEngine::Texture2D* colorPickerTexture = new UnityEngine::Texture2D(width, height);

	// Generate color gradient for the texture
	//for (int y = 0; y < height; ++y) {
	//	for (int x = 0; x < width; ++x) {
	//		// Compute the color value based on the position in the texture
	//		float r = static_cast<float>(x) / width;
	//		float g = static_cast<float>(y) / height;
	//		float b = 0.5f;

	//		// Set the color value to the corresponding pixel in the texture
	//		UnityEngine::Color32 color(static_cast<uint8_t>(r * 255), static_cast<uint8_t>(g * 255), static_cast<uint8_t>(b * 255), 255);
	//		colorPickerTexture->SetPixel(x, y, color);
	//	}
	//}
}
//
//inline bool mouse_in__box = false;
//
//inline void ColorPicker(const wchar_t* name, float accent_color[4], float width, float height)
//{
//
//
//	UnityEngine::Texture2D* texture = UnityEngine::Texture2D().New(width, height);
//
//	if (is_mouse_in_box({ next_item_pos.x, next_item_pos.y }, { width, height })) {
//		outline_box_({ mouse_pos.x,mouse_pos.y}, {25.f,25.f}, Color::Black());
//	}
//
//	for (int y = 0; y < height; y++)
//	{
//		for (int x = 0; x < width; x++)
//		{
//			// Calculate the hue value based on the x position
//			float hue = (float)x / (float)width;
//
//			// Convert the hue to RGB color
//			Color color = Color(hue, (float)y / height <= 0.f ? 1.f : (float)y / height, 1.f);
//			if (x == mouse_pos.x && y == mouse_pos.y && mouse_state && !old_mouse_state && !interacting && !bComboOpenedInThisFrame)
//			{
//				// Set the pixel color based on the mouse position
//				color.m_red = mouse_pos.x;
//				color.m_green = mouse_pos.y;
//			}
//			// Set the pixel color in the texture
//			texture->SetPixel(x, y, color);
//		}
//	}
//	// Apply the texture changes
//	texture->Apply();
//
//	UnityEngine::GUI::DrawTexture(UnityEngine::rect_t(next_item_pos.x, next_item_pos.y, width, height), texture);
//	next_item_pos.y += 20;
//}

inline void playerList(const std::string& title, const std::vector<std::wstring> items, int& selected_index)
{
	const auto wstring = std::wstring(title.begin(), title.end());

	for (size_t idx{ 0 }; idx < items.size(); idx++) {
		if (is_mouse_in_box({ next_item_pos.x, next_item_pos.y + (idx * 18) }, { next_item_pos.x + 100, next_item_pos.y + (idx * 18) + 15 }) && mouse_state && !old_mouse_state && !interacting)
			selected_index = idx;
	}

	if (menu_event == RustStructs::EventType::Repaint)
	{
		for (size_t idx{ 0 }; idx < items.size(); idx++) {
			const auto current_item = std::wstring(items.at(idx).begin(), items.at(idx).end());
			auto cleaned = current_item.substr(current_item.find_last_of(SECURE(L"\\")) + 1);

			

			//Text(next_item_pos.x + 7 + 1, next_item_pos.y + (idx * 18) + 1, 150, 100, cleaned.c_str(), Color::Black(), false, false, 12.f);
			Text(next_item_pos.x + 7, next_item_pos.y + (idx * 18), 150, 100, cleaned.c_str(), idx == selected_index ? Color::Red() : Color::White(), false, true, 12.f);
		
		}
	}


	outline_box({ next_item_pos.x, next_item_pos.y }, { 130, (items.size() * 18) + 10.f }, Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f));

	next_item_pos.y += (items.size() * 18) + 10.f;
}

inline void listbox(const std::string& title, const std::vector<std::string> items, int& selected_index)
{
	const auto wstring = std::wstring(title.begin(), title.end());

	for (size_t idx{ 0 }; idx < items.size(); idx++) {
		if (is_mouse_in_box({ next_item_pos.x, next_item_pos.y + (idx * 18) }, { next_item_pos.x + 100, next_item_pos.y + (idx * 18) + 15 }) && mouse_state && !old_mouse_state && !interacting)
			selected_index = idx;
	}

	if (menu_event == RustStructs::EventType::Repaint)
	{
		for (size_t idx{ 0 }; idx < items.size(); idx++) {
			const auto current_item = std::wstring(items.at(idx).begin(), items.at(idx).end());
			auto cleaned = current_item.substr(current_item.find_last_of(SECURE(L"\\")) + 1);

			//Text(next_item_pos.x + 7 + 1, next_item_pos.y + (idx * 18) + 1, 150, 100, cleaned.c_str(), Color::Black(), false, false, 12.f);
			Text(next_item_pos.x + 7, next_item_pos.y + (idx * 18), 150, 100, cleaned.c_str(), idx == selected_index ? Color::Red() : Color::White(), false, true, 12.f);
		}
	}


	outline_box({ next_item_pos.x, next_item_pos.y }, { 130, (items.size() * 18) + 10.f }, Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f));

	next_item_pos.y += (items.size() * 18) + 10.f;

}

inline void CheckBox(const wchar_t* szTitle, bool& bValue) {
	if (bComboOpenedInThisFrame && vecLastCringePosition.x == next_item_pos.x && vecLastCringePosition.y > next_item_pos.y + 24 + 20 && iOpenedCringeY < next_item_pos.y)
	{
		return;
	}

	bool isHovered = is_mouse_in_box({ next_item_pos.x - 6, next_item_pos.y }, { next_item_pos.x + 16 + 80.f, next_item_pos.y + 16 });

	if (is_mouse_in_box({ next_item_pos.x - 6, next_item_pos.y }, { next_item_pos.x + 16 + 80.f, next_item_pos.y + 16 }) && mouse_state && !old_mouse_state && !interacting && !bComboOpenedInThisFrame) {
		bValue = !bValue;
		hover_element = true;
	}

	if (menu_event == RustStructs::EventType::Repaint)
	{

		fill_box({ next_item_pos.x, next_item_pos.y , 13, 13 }, bValue ? Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f) : Color(34, 34, 34, 255.f));
		if (bValue)
			Arrow(next_item_pos.x + 3, next_item_pos.y + 5, Color(255, 255, 255, 255.f));

		Color txt = isHovered ? Color(77, 75, 75, 255.f) : Color(224.f, 224.f, 224.f, 255.f);

		Color cText = bValue ? Color(224.f, 224.f, 224.f, 255.f) : Color(255.f, 255.f, 255.f, 0.6 * 255.f);
		// Render text
		Text(next_item_pos.x + 17, next_item_pos.y - 4, 150, 100, szTitle, txt.GetUnityColor(), false, true, 12.f);
	}
	next_item_pos.y += 20;
}

inline void CheckBoxConfigSave(const wchar_t* szTitle, bool& bValue) {
	if (bComboOpenedInThisFrame && vecLastCringePosition.x == next_item_pos.x && vecLastCringePosition.y > next_item_pos.y + 24 + 20 && iOpenedCringeY < next_item_pos.y)
	{
		return;
	}
	if (is_mouse_in_box({ next_item_pos.x - 6, next_item_pos.y }, { next_item_pos.x + 16, next_item_pos.y + 16 }) && mouse_state && !old_mouse_state && !interacting && !bComboOpenedInThisFrame) {
		bValue = !bValue;
		hover_element = true;

		if (bValue)
		{
			if (Config::Save(SECURE(L"rust")))
			{
				bValue = false;
			}
		}
	}
	if (menu_event == RustStructs::EventType::Repaint)
	{
		fill_box({ next_item_pos.x, next_item_pos.y , 13, 13 }, bValue ? Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f) : Color(34, 34, 34, 255.f));
		if (bValue)
			Arrow(next_item_pos.x + 3, next_item_pos.y + 5, Color(255, 255, 255, 255.f));
		Color cText = bValue ? Color(224.f, 224.f, 224.f, 255.f) : Color(255.f, 255.f, 255.f, 0.6 * 255.f);
		// Render text
		Text(next_item_pos.x + 17, next_item_pos.y - 4, 150, 100, szTitle, cText, false, true, 12.f);
	}
	next_item_pos.y += 20;
}
inline void CheckBoxConfigLoad(const wchar_t* szTitle, bool& bValue) {
	if (bComboOpenedInThisFrame && vecLastCringePosition.x == next_item_pos.x && vecLastCringePosition.y > next_item_pos.y + 24 + 20 && iOpenedCringeY < next_item_pos.y)
	{
		return;
	}
	if (is_mouse_in_box({ next_item_pos.x - 6, next_item_pos.y }, { next_item_pos.x + 16, next_item_pos.y + 16 }) && mouse_state && !old_mouse_state && !interacting && !bComboOpenedInThisFrame) {
		bValue = !bValue;
		hover_element = true;

		if (bValue)
		{
			if (Config::Load(SECURE(L"rust")))
			{
				bValue = false;
			}
		}
	}
	if (menu_event == RustStructs::EventType::Repaint)
	{
		fill_box({ next_item_pos.x, next_item_pos.y , 13, 13 }, bValue ? Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f) : Color(34, 34, 34, 255.f));
		if (bValue)
			Arrow(next_item_pos.x + 3, next_item_pos.y + 5, Color(255, 255, 255, 255.f));
		Color cText = bValue ? Color(224.f, 224.f, 224.f, 255.f) : Color(255.f, 255.f, 255.f, 0.6 * 255.f);
		// Render text
		Text(next_item_pos.x + 17, next_item_pos.y - 4, 150, 100, szTitle, cText, false, true, 12.f);
	}
	next_item_pos.y += 20;
}
inline void SliderFloat(const wchar_t* szTitle, float& flValue, int iMin, int iMax, bool color = false)
{
	bool bHovered = false;
	if (bComboOpenedInThisFrame && vecLastCringePosition.x == next_item_pos.x && vecLastCringePosition.y > next_item_pos.y + 14 + 20 + 6 && iOpenedCringeY < next_item_pos.y)
	{
		//next_item_pos.y += 24 + 20 + 6 + 20;
		return;
	}

	if (is_mouse_in_box({ next_item_pos.x, next_item_pos.y + 24 }, { next_item_pos.x + 242 + 1, next_item_pos.y + 30 }) && !bComboOpenedInThisFrame)
	{
		bHovered = true;
		if (mouse_state && (!old_mouse_state || !bComboOpenedInThisFrame) && !interacting)
		{
			if (old_mouse_pos.x < next_item_pos.x)
				flValue = static_cast <float>(iMin);
			else if (old_mouse_pos.x > next_item_pos.x + 242 + 1)
				flValue = static_cast <float>(iMax);
			else
				flValue = static_cast <float> ((old_mouse_pos.x - (next_item_pos.x)) * (iMax - iMin) / 242 + 1);
			hover_element = true;
		}
	}

	if (menu_event == RustStructs::EventType::Repaint)
	{


		Color cLableColor = bHovered ? Color(116.f, 77.f, 169.f, 255.f) : Color(189.f, 189.f, 189.f, 255.f);

		/* Render lable name */
		Text(next_item_pos.x, next_item_pos.y, 300, 100, szTitle, cLableColor, false, false, 12.f);

		/* Convert text */
		char szValue[64];
		wchar_t wszValue[64];
		itoass(static_cast<float>(flValue), szValue, 10);
		AnsiToUnicode(szValue, wszValue);

		int iValueToAddForText = 0;

		/* Cringe hardcode to normalize text */
		if (flValue >= 10000)
			iValueToAddForText = -5;
		else if (flValue >= 1000)
			iValueToAddForText = 1;
		else if (flValue >= 100)
			iValueToAddForText = 8;
		else if (flValue >= 10)
			iValueToAddForText = 15;
		else if (flValue >= 0)
			iValueToAddForText = 20;

		/* Render value text*/
		Text(next_item_pos.x - 12 + 226 + iValueToAddForText, next_item_pos.y, 300, 100, wszValue, { 116.f, 77.f, 169.f, 255.f }, false, false, 12.f);

		/* Render slider bar */
		fill_box({ next_item_pos.x, next_item_pos.y + 24 , 242, 5 }, Color(33, 33, 33, 255.f));


		/* Calculate one amount */
		float flOneAmount = 242.f / (iMax - iMin);

		/* clamp for slider */
		if (flValue < iMin)
			flValue = iMin;

		if (flValue > iMax)
			flValue = iMax;

		/* Calculate current size of value */
		float flCurrentSize = flOneAmount * flValue;

		/* Render progress */
		fill_box({ next_item_pos.x, next_item_pos.y + 24 , flCurrentSize, 5 }, Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f));


	}

	next_item_pos.y += 24 + 15;
}
inline void Slider(const wchar_t* szTitle, float& flValue, float iMin, float iMax, bool color = false)
{
	bool bHovered = false;
	if (bComboOpenedInThisFrame && vecLastCringePosition.x == next_item_pos.x && vecLastCringePosition.y > next_item_pos.y + 14 + 20 + 6 && iOpenedCringeY < next_item_pos.y)
	{
		//next_item_pos.y += 24 + 20 + 6 + 20;
		return;
	}


	if (is_mouse_in_box({ next_item_pos.x, next_item_pos.y + 24 }, { next_item_pos.x + 242 + 0.1f, next_item_pos.y + 30 }) && !bComboOpenedInThisFrame)
	{
		bHovered = true;
		if (mouse_state && (!old_mouse_state || !bComboOpenedInThisFrame) && !interacting)
		{
			if (old_mouse_pos.x < next_item_pos.x)
				flValue = static_cast <float>(iMin);
			else if (old_mouse_pos.x > next_item_pos.x + 242 + 0.1f)
				flValue = static_cast <float>(iMax);
			else
				flValue = static_cast <float> ((old_mouse_pos.x - (next_item_pos.x)) * (iMax - iMin) / 242 + 0.1f);
			hover_element = true;
		}
	}

	if (menu_event == RustStructs::EventType::Repaint)
	{


		Color cLableColor = bHovered ? Color(116.f, 77.f, 169.f, 255.f) : Color(189.f, 189.f, 189.f, 255.f);

		/* Render lable name */
		Text(next_item_pos.x, next_item_pos.y, 300, 100, szTitle, cLableColor.GetUnityColor(), false, false, 12.f);

		/* Convert text */
		char szValue[64];
		wchar_t wszValue[64];
		itoass(static_cast<float>(flValue), szValue, 10);
		AnsiToUnicode(szValue, wszValue);

		int iValueToAddForText = 0;

		/* Cringe hardcode to normalize text */
		if (flValue >= 10000)
			iValueToAddForText = -5;
		else if (flValue >= 1000)
			iValueToAddForText = 1;
		else if (flValue >= 100)
			iValueToAddForText = 8;
		else if (flValue >= 10)
			iValueToAddForText = 15;
		else if (flValue >= 0)
			iValueToAddForText = 20;

		/* Render value text*/
		Text(next_item_pos.x - 12 + 226 + iValueToAddForText, next_item_pos.y, 300, 100, wszValue, { 116.f, 77.f, 169.f, 255.f }, false, false, 12.f);

		/* Render slider bar */
		fill_box({ next_item_pos.x, next_item_pos.y + 24 , 242, 5 }, Color(33, 33, 33, 255.f));


		/* Calculate one amount */
		float flOneAmount = 242.f / (iMax - iMin);

		/* clamp for slider */
		if (flValue < iMin)
			flValue = iMin;

		if (flValue > iMax)
			flValue = iMax;

		/* Calculate current size of value */
		float flCurrentSize = flOneAmount * flValue;

		/* Render progress */
		fill_box({ next_item_pos.x, next_item_pos.y + 24 , flCurrentSize, 5 }, Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f));


	}

	next_item_pos.y += 24 + 15;
}
inline void SliderColor(const wchar_t* szTitle, float& flValue, int iMin, int iMax, bool color = false)
{
	bool bHovered = false;


	if (is_mouse_in_box({ next_color_pos.x, next_color_pos.y + 24 }, { next_color_pos.x + 242 + 1, next_color_pos.y + 30 }) && !bComboOpenedInThisFrame)
	{
		bHovered = true;
		if (mouse_state && (!old_mouse_state || !bComboOpenedInThisFrame) && !interacting)
		{
			if (old_mouse_pos.x < next_color_pos.x)
				flValue = static_cast <float>(iMin);
			else if (old_mouse_pos.x > next_color_pos.x + 242 + 1)
				flValue = static_cast <float>(iMax);
			else
				flValue = static_cast <float> ((old_mouse_pos.x - (next_color_pos.x)) * (iMax - iMin) / 242 + 1);
			hover_element = true;
		}

	}
	else {
		bHovered = false;
		hover_element = false;
	}

	if (menu_event == RustStructs::EventType::Repaint)
	{


		Color cLableColor = bHovered ? Color(116.f, 77.f, 169.f, 255.f) : Color(189.f, 189.f, 189.f, 255.f);

		/* Render lable name */
		Text(next_color_pos.x, next_color_pos.y, 300, 100, szTitle, cLableColor, false, false, 12.f);

		/* Convert text */
		char szValue[64];
		wchar_t wszValue[64];
		itoass(static_cast<float>(flValue), szValue, 10);
		AnsiToUnicode(szValue, wszValue);

		int iValueToAddForText = 0;

		/* Cringe hardcode to normalize text */
		if (flValue >= 10000)
			iValueToAddForText = -5;
		else if (flValue >= 1000)
			iValueToAddForText = 1;
		else if (flValue >= 100)
			iValueToAddForText = 8;
		else if (flValue >= 10)
			iValueToAddForText = 15;
		else if (flValue >= 0)
			iValueToAddForText = 20;

		/* Render value text*/
		Text(next_color_pos.x - 12 + 226 + iValueToAddForText, next_color_pos.y, 300, 100, wszValue, { 116.f, 77.f, 169.f, 255.f }, false, false, 12.f);

		/* Render slider bar */
		fill_box({ next_color_pos.x, next_color_pos.y + 24, 242, 5 }, Color(33, 33, 33, 255.f));


		/* Calculate one amount */
		float flOneAmount = 242.f / (iMax - iMin);

		/* clamp for slider */
		if (flValue < iMin)
			flValue = iMin;

		if (flValue > iMax)
			flValue = iMax;

		/* Calculate current size of value */
		float flCurrentSize = flOneAmount * flValue;

		/* Render progress */
		fill_box({ next_color_pos.x, next_color_pos.y + 24 ,flCurrentSize, 5 }, Color(accent_color_[0], accent_color_[1], accent_color_[2], 255.f));


	}

	next_color_pos.y += 24 + 15;
}

inline void ColorBox(const wchar_t* szTitle, bool& test, float accent_color[4]) {
	static Vector2 last_pos = next_item_pos;
	static bool bValue = false;
	/* Dont draw if combo opened */
	if (bComboOpenedInThisFrame && vecLastCringePosition.x == next_item_pos.x && vecLastCringePosition.y > next_item_pos.y + 24 + 20 + 6 && iOpenedCringeY < next_item_pos.y)
	{
		next_item_pos.y += 20;
		return;
	}

	if (is_mouse_in_box({ next_item_pos.x - 6, next_item_pos.y }, { next_item_pos.x + 16, next_item_pos.y + 16 }) && mouse_state && !old_mouse_state && !interacting && !bComboOpenedInThisFrame) {
		test = !test;
		hover_element = true;
	}
	else {
		hover_element = false;
	}

	if (menu_event == RustStructs::EventType::Repaint)
	{
		Vector2 ColorPickerSize = Vector2(128.f, 128.f);

		Vector2 ColorWidgetSize = Vector2(20.f, 20.f);

		fill_box({ next_item_pos.x, next_item_pos.y , 13, 13 }, Color(accent_color[0], accent_color[1], accent_color[2], accent_color[3]));
		Color cText = test ? Color(0.9f, 0.9f, 0.9f, 0.9f) : Color(1.f, 1.f, 1.f, 0.6 * 1.f);
		// Render text
		Text(next_item_pos.x + 17, next_item_pos.y - 4, 150, 100, szTitle, cText, false, true, 12.f);
		if (test) {

			BeginChildÒ(SECURE(L"Color"), { 575,0 }, { 260,209 }); {
				SliderColor(SECURE(L"R"), accent_color[0], 0, 255);
				SliderColor(SECURE(L"G"), accent_color[1], 0, 255);
				SliderColor(SECURE(L"B"), accent_color[2], 0, 255);
				SliderColor(SECURE(L"A"), accent_color[3], 0, 255);
			}

		}
	}
	hover_element = false;
	next_item_pos.y += 20;
}



