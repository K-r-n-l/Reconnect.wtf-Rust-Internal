#include "System.h"


namespace System
{
	Type* Type::GetType(const char* qualified_name) {
		const auto get_type = reinterpret_cast<Type * (*)(Il2CppString*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(CIl2Cpp::FindClass(SECURE("System"), SECURE("Type")), HASH("GetType"), 1)));
		if (!get_type)
			return nullptr;

		return memory::Call<Type*>((uintptr_t)get_type, CIl2Cpp::il2cpp_string_new(qualified_name));
	}

	Type* Type::SkinnedMeshRenderer() {
		Type* type = GetType(SECURE("UnityEngine.SkinnedMeshRenderer, UnityEngine.CoreModule"));
		if (!type)
			return nullptr;

		return type;
	}
	Type* Type::PostProcessDebug() {
		Type* type = GetType(SECURE("UnityEngine.Rendering.PostProcessing.PostProcessDebug, Assembly-CSharp"));
		if (!type)
			return nullptr;

		return type;
	}
	Type* Type::Renderer() {
		Type* type = GetType(SECURE("UnityEngine.Renderer, UnityEngine.CoreModule"));
		if (!type)
			return nullptr;

		return type;
	}

	Type* Type::Shader() {
		Type* type = GetType(SECURE("UnityEngine.Shader, UnityEngine.CoreModule"));
		return type;
	}


	Type* Type::Object() {
		Type* type = GetType(SECURE("UnityEngine.Object, UnityEngine.CoreModule"));
		return type;
	}


	Type* Type::Material() {
		Type* type = GetType(SECURE("UnityEngine.Material, UnityEngine.CoreModule"));
		return type;
	}

	Type* Type::Behaviour() {
		Type* type = GetType(SECURE("UnityEngine.Behaviour, UnityEngine.CoreModule"));
		return type;
	}

	Type* Type::Sprite() {
		Type* type = GetType(SECURE("UnityEngine.Sprite, UnityEngine.CoreModule"));
		return type;
	}

	Type* Type::Font() {
		Type* type = GetType(SECURE("UnityEngine.Font, UnityEngine.CoreModule"));
		return type;
	}

	Type* Type::Projectile() {
		Type* type = GetType(SECURE("Projectile, Assembly-CSharp"));
		return type;
	}

	Type* Type::BaseProjectile() {
		Type* type = GetType(SECURE("BaseProjectile, Assembly-CSharp"));
		return type;
	}

	Type* Type::ItemModProjectile() {
		Type* type = GetType(SECURE("ItemModProjectile, Assembly-CSharp"));
		return type;
	}

	Type* Type::ExplosionsFPS() {
		Type* type = GetType(SECURE("ExplosionsFPS, Assembly-CSharp"));
		return type;
	}

	Type* Type::ScrollViewState() {
		Type* type = GetType(SECURE("UnityEngine.ScrollViewState, UnityEngine.IMGUIModule"));
		return type;
	}

	const wchar_t* String::c_str() const
	{
		return m_firstChar;
	}

	wchar_t* String::begin()
	{
		return m_firstChar;
	}

	wchar_t* String::end()
	{
		return (begin() + m_stringLength);
	}

	const wchar_t* String::begin() const
	{
		return m_firstChar;
	}

	auto String::string_safe() -> std::string
	{
		std::wstring ws = m_firstChar;
		return std::string(ws.begin(), ws.end());
	}

	const wchar_t* String::end() const
	{
		return (begin() + m_stringLength);
	}

	bool String::Contains(const char* str)
	{
		if (!this) return false;

		static uintptr_t procedure = 0;
		if (!memory::IsAddressValid(procedure))
		{
			const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Contains"), 1);
			if (memory::IsAddressValid(method))
			{
				procedure = memory::ToAddress(method->methodPointer);
			}
		}

		if (memory::IsAddressValid(procedure))
		{
			return memory::Call<bool>(procedure, this, CIl2Cpp::il2cpp_string_new(str));
		}
		else
		{
			TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
			return false;
		}
	}


}