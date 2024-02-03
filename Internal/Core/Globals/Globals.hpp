#pragma once



#include <Library/constant/character.hpp>
#include <Library/constant/hash.hpp>
#include <Library/constant/secure_string.hpp>
#include <Library/constant/string.hpp>
#include <Library/constant/randomizer.hpp>

#include <Library/core/map_data.hpp>
#include <Library/core/mutex.hpp>
#include <Library/core/no_copy.hpp>
#include <Library/core/no_move.hpp>
#include <Library/core/singleton.hpp>
#include <Library/core/timer.hpp>

#include <Library/graphics/color.hpp>
#include <Library/hde/hde64.hpp>

#include <Library/include/auto.hpp>
#include <Library/include/base.hpp>
#include <Library/include/win32.hpp>

#include <Library/memory/operation.hpp>
#include <Library/memory/scan.hpp>
#include <Library/memory/secure_call.hpp>

#include <Library/vcruntime/dll_dllmain.hpp>
#include <Library/vcruntime/security_cookie.hpp>
#include <Library/vcruntime/thread_local_storage.hpp>
#include <Library/vcruntime/thread_safe_statics.hpp>

#include <Library/win32/sdk/access_mask.hpp>
#include <Library/win32/sdk/client_id.hpp>
#include <Library/win32/sdk/constant_page.hpp>
#include <Library/win32/sdk/constant_region.hpp>
#include <Library/win32/sdk/constant_section.hpp>
#include <Library/win32/sdk/event_basic_information.hpp>
#include <Library/win32/sdk/event_information_class.hpp>
#include <Library/win32/sdk/event_type.hpp>
#include <Library/win32/sdk/exception_registration_record.hpp>
#include <Library/win32/sdk/file_basic_information.hpp>
#include <Library/win32/sdk/io_apc_routine.hpp>
#include <Library/win32/sdk/io_status_block.hpp>
#include <Library/win32/sdk/large_integer.hpp>
#include <Library/win32/sdk/ldr_data_table_entry.hpp>
#include <Library/win32/sdk/list_entry.hpp>
#include <Library/win32/sdk/memory_basic_information.hpp>
#include <Library/win32/sdk/memory_enclave_image_information.hpp>
#include <Library/win32/sdk/memory_frame_information.hpp>
#include <Library/win32/sdk/memory_image_information.hpp>
#include <Library/win32/sdk/memory_information_class.hpp>
#include <Library/win32/sdk/memory_region_information.hpp>
#include <Library/win32/sdk/memory_shared_commit_information.hpp>
#include <Library/win32/sdk/memory_working_set_block.hpp>
#include <Library/win32/sdk/memory_working_set_ex_block.hpp>
#include <Library/win32/sdk/memory_working_set_ex_information.hpp>
#include <Library/win32/sdk/memory_working_set_ex_location.hpp>
#include <Library/win32/sdk/memory_working_set_information.hpp>
#include <Library/win32/sdk/mutant_basic_information.hpp>
#include <Library/win32/sdk/mutant_information_class.hpp>
#include <Library/win32/sdk/nt_tib.hpp>
#include <Library/win32/sdk/object_attributes.hpp>
#include <Library/win32/sdk/object_basic_information.hpp>
#include <Library/win32/sdk/object_information_class.hpp>
#include <Library/win32/sdk/object_name_information.hpp>
#include <Library/win32/sdk/object_wait_type.hpp>
#include <Library/win32/sdk/peb.hpp>
#include <Library/win32/sdk/peb_ldr_data.hpp>
#include <Library/win32/sdk/rtl_balanced_node.hpp>
#include <Library/win32/sdk/rtl_condition_variable.hpp>
#include <Library/win32/sdk/rtl_critical_section.hpp>
#include <Library/win32/sdk/rtl_critical_section_debug.hpp>
#include <Library/win32/sdk/rtl_heap_parameters.hpp>
#include <Library/win32/sdk/section_basic_information.hpp>
#include <Library/win32/sdk/section_information_class.hpp>
#include <Library/win32/sdk/section_inherit.hpp>
#include <Library/win32/sdk/string.hpp>
#include <Library/win32/sdk/teb.hpp>
#include <Library/win32/sdk/ularge_integer.hpp>
#include <Library/win32/sdk/unicode_string.hpp>

#include <Library/win32/basic.hpp>
#include <Library/win32/image.hpp>
#include <Library/win32/input_manager.hpp>
#include <Library/win32/math.hpp>
#include <Library/win32/process.hpp>
#include <Library/win32/string.hpp>
#include <Library/win32/time.hpp>
#include <Library/win32/trace.hpp>

using namespace horizon;


using graphics::Color;

#include "../Utils/Utils.hpp"

#include "../Sdk/Il2Cpp/il2cpp-api.hpp"
#include "../Sdk/Il2Cpp/il2cpp-api-types.hpp"
#include "../Sdk/Il2Cpp/il2cpp-blob.hpp"
#include "../Sdk/Il2Cpp/il2cpp-class-internals.hpp"
#include "../Sdk/Il2Cpp/il2cpp-config.hpp"
#include "../Sdk/Il2Cpp/il2cpp-config-api.hpp"
#include "../Sdk/Il2Cpp/il2cpp-metadata.hpp"
#include "../Sdk/Il2Cpp/il2cpp-object-internals.hpp"
#include "../Sdk/Il2Cpp/il2cpp-runtime-metadata.hpp"
#include <map>

#define maxx( a, b ) ( ( ( a ) > ( b ) ) ? ( a ) : ( b ) )
#define minm( a, b ) ( ( ( a ) < ( b ) ) ? ( a ) : ( b ) )
inline unsigned short lfsr = 0xACE1u;
inline unsigned int bit = 0;

inline unsigned my_rand()
{
	if (!lfsr) {
		lfsr = 0xACE1u; bit = 0;
	}
	bit = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5)) & 1;
	return lfsr = (lfsr >> 1) | (bit << 15);
}

class Unity_String
{
public:
	char zpad[0x10]{ };
	int size{ };
	wchar_t buffer[128 + 1];
public:
	Unity_String(const wchar_t* st)
	{
		size = minm(win32::wcslen(st), 128);
		for (int idx = 0; idx < size; idx++)
		{
			buffer[idx] = st[idx];
		}
		buffer[size] = 0;
	}

	wchar_t* GetWchar()
	{
		return buffer;
	}
};
inline float time_last_upgrade = 0.f;
inline uintptr_t m_game_assembly;

struct _iobuf222 {
	char* _ptr;
	int   _cnt;
	char* _base;
	int   _flag;
	int   _file;
	int   _charbuf;
	int   _bufsiz;
	char* _tmpfname;
};
typedef struct _iobuf222 FILEz;

typedef int(WINAPI* Proto_printf)(const char* format, ...);
typedef FILEz* (WINAPI* Proto_freopen)(const char* path, const char* mode, FILEz* stream);
typedef FILEz* (WINAPI* Proto_iob_func)();
typedef BOOL(WINAPI* f_alloc_console)();

#define stdout (&iob_funcz()[1]);
inline Proto_printf printfz = NULL;
inline Proto_freopen freopenz = NULL;
inline Proto_iob_func iob_funcz = NULL;
inline f_alloc_console alloc_console = NULL;


#define RVA(addr, size) ((uintptr_t)((UINT_PTR)(addr) + *(PINT)((UINT_PTR)(addr) + ((size) - sizeof(INT))) + (size)))

typedef int(WINAPI* sprintf_szz)(char* const, std::size_t, const char* const, _locale_t, va_list);

inline sprintf_szz crt_sprintf = NULL;

inline int vsnprintf_s222(char* const destination, std::size_t size, const char* const format, va_list arg_pack) noexcept
{
	return crt_sprintf(destination, size, format, nullptr, arg_pack);
}

inline int vsprintf_szzz(char* const destination, std::size_t size, const char* const format, va_list arg_list) noexcept
{
	return vsnprintf_s222(destination, size, format, arg_list);
}

inline int crt_sprintf_s(char* const destination, std::size_t size, const char* const format, ...) noexcept
{
	va_list arg_list = { };
	va_start(arg_list, format);
	const int result = vsprintf_szzz(destination, size, format, arg_list);
	va_end(arg_list);
	return result;
}

inline uintptr_t GetListAddress = 54982184; //Method$Facepunch.Pool.GetList\u003CTraceInfo\u003E() address
inline uintptr_t GetListMethodAddress = 19259488;

#define DEBUG_MODE 0

namespace CIl2Cpp {

	DO_API(Il2CppDomain*, il2cpp_domain_get, ());
	DO_API(void*, il2cpp_thread_attach, (void*));
	DO_API(void*, il2cpp_thread_detach, (void*));
	DO_API(const Il2CppAssembly**, il2cpp_domain_get_assemblies, (const Il2CppDomain* domain, std::size_t* size));
	DO_API(uint64_t, il2cpp_image_get_class_count, (const Il2CppImage* domain));
	DO_API(Il2CppClass*, il2cpp_image_get_class, (const Il2CppImage* domain, uint64_t));

	DO_API(void, il2cpp_set_memory_callbacks, (Il2CppMemoryCallbacks* callbacks));
	DO_API(const Il2CppImage*, il2cpp_get_corlib, ());
	DO_API(void, il2cpp_add_internal_call, (const char* name, Il2CppMethodPointer method));
	DO_API(Il2CppMethodPointer, il2cpp_resolve_icall, (const char* name));

	DO_API(void*, il2cpp_alloc, (std::size_t size));
	DO_API(void, il2cpp_free, (void* ptr));

	DO_API(const Il2CppImage*, il2cpp_assembly_get_image, (const Il2CppAssembly* assembly));

	DO_API(bool, il2cpp_class_has_parent, (Il2CppClass* klass, Il2CppClass* klassc));
	DO_API(Il2CppClass*, il2cpp_class_from_name, (const Il2CppImage* image, const char* namespaze, const char* name));
	DO_API(const EventInfo*, il2cpp_class_get_events, (const Il2CppClass* klass, void** iter));
	DO_API(FieldInfo*, il2cpp_class_get_fields, (const Il2CppClass* klass, void** iter));
	DO_API(FieldInfo*, il2cpp_class_get_field_from_name, (const Il2CppClass* klass, const char* name));
	DO_API(const PropertyInfo*, il2cpp_class_get_properties, (const Il2CppClass* klass, void** iter));
	DO_API(const PropertyInfo*, il2cpp_class_get_property_from_name, (const Il2CppClass* klass, const char* name));
	DO_API(const MethodInfo*, il2cpp_class_get_methods, (Il2CppClass* klass, void** iter));
	DO_API(const MethodInfo*, il2cpp_class_get_method_from_name, (Il2CppClass* klass, const char* name, int argsCount));
	DO_API(const Il2CppType*, il2cpp_type_get_object, (const Il2CppType* klass));

	DO_API(const Il2CppType*, il2cpp_class_get_type, (Il2CppClass* klass));

	DO_API(const char*, il2cpp_class_get_name, (Il2CppClass* klass));
	DO_API(const char*, il2cpp_class_get_namespace, (Il2CppClass* klass));
	DO_API(Il2CppClass*, il2cpp_class_get_parent, (Il2CppClass* klass));
	DO_API(int, il2cpp_method_get_param_count, (uintptr_t));
	DO_API(Il2CppType*, il2cpp_method_get_param, (const MethodInfo*, uint32_t));
	DO_API(const char*, il2cpp_type_get_name, (const Il2CppType*));
	DO_API(Il2CppType*, il2cpp_method_get_return_type, (const MethodInfo*));

	DO_API(const char*, il2cpp_field_get_name, (const FieldInfo* field));
	DO_API(std::size_t, il2cpp_field_get_offset, (const FieldInfo* field));

	DO_API(const MethodInfo*, il2cpp_property_get_get_method, (const PropertyInfo* prop));
	DO_API(const MethodInfo*, il2cpp_property_get_set_method, (const PropertyInfo* prop));
	DO_API(const char*, il2cpp_property_get_name, (const PropertyInfo* prop));

	DO_API(Il2CppString*, il2cpp_string_new, (const char* str));
	DO_API(uintptr_t, il2cpp_object_new, (void*));

	// runtime
	DO_API(Il2CppObject*, il2cpp_runtime_invoke, (const MethodInfo* method, void* obj, void** params, Il2CppObject** exc));
	DO_API(void*, il2cpp_object_unbox, (Il2CppObject* obj));

	DO_API(const Il2CppAssembly*, il2cpp_image_get_assembly, (const Il2CppImage* image));
	DO_API(const char*, il2cpp_image_get_name, (const Il2CppImage* image));
	DO_API(const char*, il2cpp_image_get_filename, (const Il2CppImage* image));
	DO_API(const MethodInfo*, il2cpp_image_get_entry_point, (const Il2CppImage* image));

	DO_API(void, il2cpp_monitor_enter, (Il2CppObject* obj));
	DO_API(void, il2cpp_monitor_exit, (Il2CppObject* obj));


	inline const Il2CppClass* FindClass(const Il2CppImage* image, const char* name_space, const char* name)
	{
		return il2cpp_class_from_name(image, name_space, name);
	}

	inline const Il2CppClass* FindClass(const char* name_space, const char* name)
	{
		const auto domain = il2cpp_domain_get();

		if (memory::IsAddressValid(domain))
		{
			std::size_t count = 0;
			const auto assemblies = il2cpp_domain_get_assemblies(domain, &count);

			if (memory::IsAddressValid(assemblies))
			{
				for (std::size_t index = 0; index < count; index++)
				{
					const auto assembly = assemblies[index];

					if (memory::IsAddressValid(assembly))
					{
						const auto image = il2cpp_assembly_get_image(assembly);

						if (memory::IsAddressValid(image))
						{
							const auto instance = FindClass(image, name_space, name);

							if (memory::IsAddressValid(instance))
							{
								return instance;
							}
						}
					}
				}
			}
			else
			{
				TRACE("%s: il2cpp_domain_get_assemblies( ... ) error!", ATOM_FUNCTION);
			}
		}
		else
		{
			TRACE("%s: il2cpp_domain_get() error!", ATOM_FUNCTION);
		}

		return nullptr;
	}

	inline const FieldInfo* FindField(const Il2CppClass* object, std::uint64_t name_hash)
	{
		auto super = object;

		while (memory::IsAddressValid(super))
		{
			auto iterator = static_cast<void*>(nullptr);
			auto field = il2cpp_class_get_fields(super, &iterator);

			while (memory::IsAddressValid(field))
			{
				const auto name = il2cpp_field_get_name(field);

				if (memory::IsAddressValid(name))
				{
					if (constant::Hash(name, true) == name_hash)
					{
						return field;
					}
				}

				field = il2cpp_class_get_fields(super, &iterator);
			}
			super = super->parent;
		}

		return nullptr;
	}

	inline const PropertyInfo* FindProperty(const Il2CppClass* object, std::uint64_t name_hash)
	{
		auto super = object;

		while (memory::IsAddressValid(super))
		{
			auto iterator = static_cast<void*>(nullptr);
			auto prop = il2cpp_class_get_properties(super, &iterator);

			while (memory::IsAddressValid(prop))
			{
				const auto name = il2cpp_property_get_name(prop);

				if (memory::IsAddressValid(name))
				{
					if (constant::Hash(name, true) == name_hash)
					{
						return prop;
					}
				}

				prop = il2cpp_class_get_properties(super, &iterator);
			}

			super = super->parent;
		}

		return nullptr;
	}


	inline const Il2CppType* FindType(const Il2CppClass* object) {
		auto get_type = CIl2Cpp::il2cpp_class_get_type((Il2CppClass*)object);
		return CIl2Cpp::il2cpp_type_get_object(get_type);
	}


	inline const MethodInfo* FindMethod(const Il2CppClass* object, std::uint64_t name_hash, std::uint8_t params_count /* = 0 */)
	{
		auto super = object;

		while (memory::IsAddressValid(super))
		{
			auto iterator = static_cast<void*>(nullptr);
			auto method = il2cpp_class_get_methods(const_cast<Il2CppClass*>(super), &iterator);

			while (memory::IsAddressValid(method))
			{
				if (constant::Hash(method->name, true) == name_hash)
				{
					if (method->parameters_count == params_count)
					{
						return method;
					}
				}

				method = il2cpp_class_get_methods(const_cast<Il2CppClass*>(super), &iterator);
			}

			super = super->parent;
		}

		return nullptr;
	}

	inline uintptr_t hook(void* our_func, const char* kl, uint64_t name, int arg, const char* name_space)
	{
		auto il2cpp_method = (uintptr_t)FindMethod(FindClass(name_space, kl), name, arg);

		*reinterpret_cast<void**>(il2cpp_method) = our_func;

		void* written = *reinterpret_cast<void**>(il2cpp_method);
		if (written == our_func)
		{
			//TRACE("Hooked [%s] Successfully", kl);
		}

		return il2cpp_method;
	}



	inline uint64_t FindKlassFullArgs(uint32_t path) {
		const auto domain = il2cpp_domain_get();

		if (memory::IsAddressValid(domain))
		{
			std::size_t count = 0;
			const auto assemblies = il2cpp_domain_get_assemblies(domain, &count);

			if (memory::IsAddressValid(assemblies))
			{
				for (std::size_t index = 0; index < count; index++)
				{
					const auto assembly = assemblies[index];

					if (memory::IsAddressValid(assembly))
					{
						const auto image = il2cpp_assembly_get_image(assembly);

						if (memory::IsAddressValid(image))
						{
							const auto image_class_count = il2cpp_image_get_class_count(image);

							for (int c = 0; c < image_class_count; c++) {
								if (image_class_count < 0)
									continue;

								std::string temp(il2cpp_image_get_name(image));
								temp.erase(temp.find(SECURE(".dll")), 4);


								auto klass = il2cpp_image_get_class(image, c);
								if (!memory::IsAddressValid(klass))
									continue;



								const char* name = il2cpp_class_get_name(klass);
								if (!memory::IsAddressValid(name))
									continue;

								const char* ns = il2cpp_class_get_namespace(klass);
								if (!memory::IsAddressValid(ns))
									continue;


								

								if (std::string(ns).empty())
									temp = temp + SECURE("::") + name;
								else
									temp = temp + SECURE("::") + ns + SECURE("::") + name;

								printfz("\n temp - %s", temp.c_str());

								if (constant::Hash(temp.c_str(), true) == path) {
									return std::uint64_t(klass);
								}
							}
						}
					}
				}
			}
			else
			{
				printfz("\n %s: il2cpp_domain_get_assemblies( ... ) error!", ATOM_FUNCTION);
			}
		}
		else
		{
			printfz("\n %s: il2cpp_domain_get() error!", ATOM_FUNCTION);
		}

		return 0;
	}


	inline const MethodInfo* FindMethodFullArgs(std::uint64_t name_hash)
	{
		auto domain = il2cpp_domain_get();
		std::size_t count = 0;
		auto assemblies = il2cpp_domain_get_assemblies(domain, &count);

		for (int i = 0; i < count; i++) {
			auto image = *reinterpret_cast<Il2CppImage**>(*reinterpret_cast<uint64_t*>(std::uint64_t(assemblies) + (0x8 * i)));

			if (!memory::IsAddressValid(image))
				continue;

			for (int c = 0; c < il2cpp_image_get_class_count(image); c++) {
				std::string temp(image->name);
				temp.erase(temp.find(SECURE(".dll")), 4);

				auto klass = il2cpp_image_get_class(image, c);
				if (!memory::IsAddressValid(klass))
					continue;

				const char* name = klass->name;

				if (!memory::IsAddressValid(name))
					continue;

				const char* ns = klass->namespaze;

				if (std::string(ns).empty())
					temp = temp + SECURE("::") + name;
				else
					temp = temp + SECURE("::") + ns + SECURE("::") + name;

				const MethodInfo* Method = nullptr;
				auto iterator = static_cast<void*>(nullptr);
				while (Method = il2cpp_class_get_methods(const_cast<Il2CppClass*>(klass), &iterator))
				{
					std::string temp2(temp + SECURE("::") + Method->name);

					auto param_count = il2cpp_method_get_param_count(memory::ToAddress(Method));
					if (param_count > 0)
					{
						temp2 = temp2 + SECURE("(");
						for (int p = 0; p < param_count; p++) {
							const auto param = il2cpp_method_get_param(Method, p);
							const auto param_name = il2cpp_type_get_name(param);
							std::string t(param_name);
							t = t.substr(t.find(SECURE(".")) + 1);
							temp2 = temp2 + t + SECURE(",");
						}

						const auto return_type = Method->return_type;
						const auto return_type_name = il2cpp_type_get_name(return_type);

						std::string t(return_type_name);
						temp2 = temp2.substr(0, temp2.size() - 1);
						temp2 = temp2 + SECURE("): ") + t.substr(t.find(SECURE(".")) + 1);
					}
					else {
						const auto return_type = Method->return_type;
						const auto return_type_name = il2cpp_type_get_name(return_type);
						std::string t(return_type_name);
						temp2 = temp2 + SECURE("(): ") + t.substr(t.find(SECURE(".")) + 1);
					}
					if (constant::Hash(temp2.c_str(), true) == name_hash) {
						return Method;
					}
				}

			}
		}

		return nullptr;
	}

	inline uintptr_t hook_virtual_function(const char* classname, const char* function_to_hook, void* our_func, int param_count, const char* name_space)
	{
		auto method = (uintptr_t)FindMethod(FindClass(name_space, classname), constant::Hash(function_to_hook, true), param_count);
		if (!method)
		{
			//TRACE(" Error getting method %s::%s::%s", name_space, classname, function_to_hook);
			return NULL;
		}
		uintptr_t search = *reinterpret_cast<uintptr_t*>(method);
		uintptr_t table = (uintptr_t)FindClass(name_space, classname);

		if (search == (uintptr_t)our_func)
		{
			//TRACE("Already Hooked %s::%s::%s", name_space, classname, function_to_hook);
			return (uintptr_t)our_func;
		}

		for (uintptr_t i = table; i <= table + 0x10000; i += 0x1) {
			uintptr_t addr = *reinterpret_cast<uintptr_t*>(i);
			if (addr == search)
			{
				//TRACE("Hooking %s::%s::%s", name_space, classname, function_to_hook);
				*reinterpret_cast<uintptr_t*>(i) = (uintptr_t)our_func;
				return addr;
			}
		}
		//TRACE("Failed to Hook %s::%s::%s", name_space, classname, function_to_hook);
	}


	inline uintptr_t VFuntionHook(const char* classname, const char* function_to_hook, void* our_func, const char* name_space = SECURE(""), int argcounttt = -1) {
		uintptr_t search = *reinterpret_cast<uintptr_t*>((uintptr_t)FindMethod(FindClass(name_space, classname), constant::Hash(function_to_hook, true), argcounttt));
		uintptr_t table = (uintptr_t)FindClass(name_space, classname);


		if (search == (uintptr_t)our_func)
			return (uintptr_t)our_func;

		for (uintptr_t i = table; i <= table + 0x1500; i += 0x1) {
			uintptr_t addr = *reinterpret_cast<uintptr_t*>(i);
			if (addr == search) {
				*reinterpret_cast<uintptr_t*>(i) = (uintptr_t)our_func;
				return addr;
			}
		}
	}
}

#pragma region il2cpp_reg
#define IL2CPP_NAME_SPACE( name )																															\
static const char* get_namespace(){\
	const char* gg = name; \
	return gg;\
}\

#define IL2CPP_CLASS( name )																																	\
	static const Il2CppClass* StaticClass()																											\
	{																																														\
		static const Il2CppClass* instance = nullptr;																							\
		if( !memory::IsAddressValid( instance ) )																									\
		{																																													\
			instance = CIl2Cpp::FindClass( get_namespace(), SECURE(name) );																			\
		}																																													\
		return instance;																																					\
	}


#define IL2CPP_FIELD(return_type, name) return_type& name() { \
	__try \
	{ \
		static std::size_t offset = 0; \
		if( !offset )																																							\
		{																																													\
			const auto field = CIl2Cpp::FindField( klass, HASH( #name ) );															\
			if( memory::IsAddressValid( field ) )																										\
			{																																												\
				offset = CIl2Cpp::il2cpp_field_get_offset( field );																			\
			}																																												\
		}																																													\
		return *reinterpret_cast< return_type* >( memory::ToAddress( this ) + offset );						\
	} \
	__except (true) \
	{ \
		TRACE("Exception Occurred. %s", FN); \
	} \
		auto n = nullptr; \
		return reinterpret_cast<return_type&>(n); \
	}

#define IL2CPP_STATIC_FIELD( return_type, name )																							\
	static return_type& name()																																	\
	{																																														\
		static auto offset = std::numeric_limits< std::size_t >::max();														\
		if( offset == std::numeric_limits< std::size_t >::max() )																	\
		{																																													\
			const auto field = CIl2Cpp::FindField( StaticClass(), HASH( #name ) );											\
			if( memory::IsAddressValid( field ) )																										\
			{																																												\
				offset = CIl2Cpp::il2cpp_field_get_offset( field );																			\
			}																																												\
		}																																													\
		const auto static_fields = StaticClass()->static_fields;																	\
		return *reinterpret_cast< return_type* >( memory::ToAddress( static_fields ) + offset );	\
	}


#define IL2CPP_PROPERTY( return_type, name )																																		\
	return_type JOIN( get_, name )()																																							\
	{																																																							\
		static std::uintptr_t procedure = 0;																																				\
		if( !memory::IsAddressValid( procedure ) )																																	\
		{																																																						\
			const auto prop = CIl2Cpp::FindProperty( klass, HASH( #name ) );																							\
			if( memory::IsAddressValid( prop ) )																																			\
			{																																																					\
				const auto method = CIl2Cpp::il2cpp_property_get_get_method( prop );																				\
				if( memory::IsAddressValid( method ) )																																	\
				{																																																				\
					procedure = memory::ToAddress( method->methodPointer );																								\
				}																																																				\
			}																																																					\
		}																																																						\
		if constexpr( sizeof( return_type ) > sizeof( std::uintptr_t ) )																						\
		{																																																						\
			return_type data = { };																																										\
			const auto method = reinterpret_cast< return_type*( __fastcall* )( return_type*, void* ) >( procedure );	\
			method( &data, this );																																										\
			return std::move( data );																																									\
		}																																																						\
		else																																																				\
		{																																																						\
			const auto method = reinterpret_cast< return_type( __fastcall* )( void* ) >( procedure );									\
			return method(this );																																										\
		}																																																						\
	}

#define IL2CPP_STATIC_PROPERTY( return_type, name )																															\
	static return_type JOIN( get_, name )()																																				\
	{																																																							\
		static std::uintptr_t procedure = 0;																																				\
		if( !memory::IsAddressValid( procedure ) )																																	\
		{																																																						\
			const auto prop = CIl2Cpp::FindProperty( StaticClass(), HASH( #name ) );																			\
			if( memory::IsAddressValid( prop ) )																																			\
			{																																																					\
				const auto method = CIl2Cpp::il2cpp_property_get_get_method( prop );																				\
				if( memory::IsAddressValid( method ) )																																	\
				{																																																				\
					procedure = memory::ToAddress( method->methodPointer );																								\
				}																																																				\
			}																																																					\
		}																																																						\
		if constexpr( sizeof( return_type ) > sizeof( std::uintptr_t ) )																						\
		{																																																						\
			return_type data = { };																																										\
			const auto method = reinterpret_cast< return_type*( __fastcall* )( return_type* ) >( procedure );					\
			method(&data );																																													\
			return std::move( data );																																									\
		}																																																						\
		else																																																				\
		{																																																						\
			const auto method = reinterpret_cast< return_type( __fastcall* )() >( procedure );												\
			return method();																																													\
		}																																																						\
	}

#define IL2CPP_METHOD_FIND(name, size)												\
		static const MethodInfo* method_info = nullptr;								\
		if(!memory::IsAddressValid(method_info))									\
		{																			\
			method_info = CIl2Cpp::FindMethod( StaticClass(), HASH(##name), size );    \
			if ( !memory::IsAddressValid( method_info ) )							\
			{																		\
				TRACE( "%s: Invalid method_info for calling runtime method!", FN ); \
				return { };														    \
			}																		\
		}	
#pragma endregion


IL2CPP_NAME_SPACE(SECURE(""));
