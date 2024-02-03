#pragma once

#include "../../Globals/Globals.hpp"



namespace System
{
	IL2CPP_NAME_SPACE(SECURE("System"));

	struct Type {
		IL2CPP_CLASS("Type");

		static Type* GetType(const char* qualified_name);
		static Type* SkinnedMeshRenderer();
		static Type* Renderer();
		static Type* Shader();
		static Type* Material();
		static Type* Behaviour();
		static Type* Sprite();
		static Type* PostProcessDebug();

		static Type* Font();
		static Type* Projectile();
		static Type* BaseProjectile();
		static Type* ItemModProjectile();
		static Type* ExplosionsFPS();
		static Type* ScrollViewState();
		static Type* Object();

	};

	struct Object : Il2CppObject
	{
		IL2CPP_CLASS("Object");
	};

	struct IntPtr : Il2CppObject
	{
		IL2CPP_CLASS("IntPtr");

		//inline void* ToPointer()
		//{
		//	static uintptr_t procedure = 0;
		//	if ( !memory::IsAddressValid( procedure ) )
		//	{
		//		const auto method = rust::FindMethod( StaticClass(), HASH( "ToPointer" ), 0 );
		//		if ( memory::IsAddressValid( method ) )
		//		{
		//			procedure = memory::ToAddress( method->methodPointer );
		//		}
		//	}
		//
		//	if ( memory::IsAddressValid( procedure ) )
		//	{
		//		return memory::Call<void*>( procedure, this );
		//	}
		//
		//	return nullptr;
		//}

		void* m_value = nullptr;
	};

	struct String : Il2CppObject
	{
		String(uint32_t size)
		{
			m_stringLength = size;
			m_firstChar[size] = { };
		}

		const wchar_t* c_str() const;
		wchar_t* begin();
		wchar_t* end();
		const wchar_t* begin() const;
		auto string_safe() -> std::string;
		const wchar_t* end() const;

		IL2CPP_CLASS("String");
		std::int32_t m_stringLength = 0;	// 0x0010
		wchar_t m_firstChar[1] = { };		// 0x0014

		bool Contains(const char* str);
	};

	template <class T>
	class c_system_array {
	public:
		Il2CppObject obj;
		void* bounds;
		uint64_t size;
		T items[0];
	};

	template< typename Type >
	struct Array : Il2CppArray
	{
		Type m_Items[1] = { };


		uintptr_t GetData() {
			return reinterpret_cast<uintptr_t>(&m_pValues);
		}

		Type& operator[](unsigned int index)
		{
			return *reinterpret_cast<Type*>(GetData() + sizeof(Type) * index);
		}
		Type& At(unsigned int index)
		{
			return operator[](index);
		}

		Type* m_pValues = nullptr;
	};

	struct Byte : Il2CppObject
	{
		IL2CPP_CLASS("Byte");
		IL2CPP_FIELD(Byte*, m_value);

	};

	struct Array_3 : Il2CppObject
	{
		IL2CPP_CLASS("Array");

	};

	template< typename Type >
	struct BufferList : Il2CppObject
	{
		std::int32_t count = 0;						// 0x0010
		FIELD_PAD(0x0004);							// 0x0014
		Array< Type >* buffer = nullptr;	// 0x0018
	};

	template< typename KeyType, typename ValueType >
	struct ListDictionary : Il2CppObject
	{
		FIELD_PAD(0x0010);						// 0x0010
		BufferList< KeyType >* keys = nullptr;		// 0x0020
		BufferList< ValueType >* vals = nullptr;	// 0x0040
	};

	template< typename Type >
	struct List : Il2CppObject
	{
		Array< Type >* _items = nullptr;		// 0x0010
		std::int32_t _size = 0;					// 0x0018
		std::int32_t _version = 0;				// 0x001C
		Il2CppObject* _syncRoot = nullptr;		// 0x0020
	};


	template<typename T = unsigned char>
	System::Array<T>* il2cpp_array_new_specific(const Il2CppClass* klazz, unsigned long long length) {
		auto image = win32::GetImage(HASH("GameAssembly.dll"));
		auto ret = memory::Call< System::Array<T>*>((uintptr_t)win32::GetImageExport(image, HASH("il2cpp_array_new_specific")), klazz, length);
		return ret;
	}

	template<typename T = unsigned char>
	System::Array<T>* il2cpp_array_new(const Il2CppClass* klazz, unsigned long long length) {
		auto image = win32::GetImage(HASH("GameAssembly.dll"));
		auto ret = memory::Call< System::Array<T>*>((uintptr_t)win32::GetImageExport(image, HASH("il2cpp_array_new")), klazz, length);
		return ret;
	}

	struct Uri : Il2CppObject {
		IL2CPP_CLASS("Uri");

		IL2CPP_FIELD(System::String*, m_String);
		IL2CPP_PROPERTY(System::String*, AbsoluteUri);
		IL2CPP_PROPERTY(System::String*, Query);


		System::String* ToString()
		{
			if (!this) return {};

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("ToString"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::String*>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}
	};
}


namespace SystemText {
	IL2CPP_NAME_SPACE(SECURE("System.Text"));


	struct Encoding : Il2CppObject {
		IL2CPP_CLASS("Encoding");


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


namespace SystemIO
{
	IL2CPP_NAME_SPACE(SECURE("System.IO"));


	struct File : Il2CppObject {
		IL2CPP_CLASS("File");

		static void WriteAllText(const char* path, const char* contents)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("WriteAllText"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, CIl2Cpp::il2cpp_string_new(path), CIl2Cpp::il2cpp_string_new(contents));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		static System::String* ReadAllText(const char* path)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("ReadAllText"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::String*>(procedure, CIl2Cpp::il2cpp_string_new(path));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}

		static std::vector<uint8_t> ReadAllBytes(const char* path)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("ReadAllBytes"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<std::vector<uint8_t>>(procedure, CIl2Cpp::il2cpp_string_new(path));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}
	};

	struct Stream : Il2CppObject
	{
		IL2CPP_CLASS("Stream");

	};

	struct StreamReader : Il2CppObject {
		IL2CPP_CLASS("StreamReader");


		void _cctor(Stream* stream)
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH(".ctor"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, stream);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}

		}
	};
}

namespace System_Net
{
	IL2CPP_NAME_SPACE(SECURE("System.Net"));

	struct WebClient : Il2CppObject {

		IL2CPP_CLASS("WebClient");

		System::Uri* GetUri(const char* str)
		{
			if (!this) return {};

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetUri"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::Uri*>(procedure, this, CIl2Cpp::il2cpp_string_new(str));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}


		System::c_system_array< System::Byte* >* DownloadDataInternal(System::Uri* str, uintptr_t* request)
		{
			if (!this) return {};

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("DownloadDataInternal"),2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::c_system_array< System::Byte* >*>(procedure, this, (str), request);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}

		void DownloadStringAsync(System::Uri* str)
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("DownloadStringAsync"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, (str));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		System::String* DownloadString(const char* str)
		{
			if (!this) return {};

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("DownloadString"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::String*>(procedure, this, CIl2Cpp::il2cpp_string_new(str));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
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



namespace NewtonsoftJson
{
	IL2CPP_NAME_SPACE(SECURE("Newtonsoft.Json"));


	struct JsonConvert : Il2CppObject {
		IL2CPP_CLASS("JsonConvert");

		template<typename T = UnityEngine::Object>
		static T* DeserializeObject(const char* value, System::Type* typle)
		{

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("DeserializeObject"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<T*>(procedure, CIl2Cpp::il2cpp_string_new(value), typle);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}

		static System::String* ToString(const char* value)
		{

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{

				const auto method = CIl2Cpp::FindMethodFullArgs(HASH("Newtonsoft.Json::Newtonsoft.Json::JsonConvert::ToString(String): String"));
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}

			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::String*>(procedure, CIl2Cpp::il2cpp_string_new(value));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}

		}
	};
}