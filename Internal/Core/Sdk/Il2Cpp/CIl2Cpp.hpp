#pragma once

#include "../../Globals/Globals.hpp"

namespace CIl2Cpp
{

DO_API( Il2CppDomain*, il2cpp_domain_get, () );
DO_API( const Il2CppAssembly**, il2cpp_domain_get_assemblies, ( const Il2CppDomain* domain, std::size_t* size ) );

DO_API( void, il2cpp_set_memory_callbacks, ( Il2CppMemoryCallbacks* callbacks ) );
DO_API( const Il2CppImage*, il2cpp_get_corlib, () );
DO_API( void, il2cpp_add_internal_call, ( const char* name, Il2CppMethodPointer method ) );
DO_API( Il2CppMethodPointer, il2cpp_resolve_icall, ( const char* name ) );
DO_API(uint64_t, il2cpp_image_get_class_count, (const Il2CppImage* domain));
DO_API(Il2CppClass*, il2cpp_image_get_class, (const Il2CppImage* domain, uint64_t));
DO_API(Il2CppType*, il2cpp_method_get_param, (const MethodInfo*, uint32_t));
DO_API(const char*, il2cpp_type_get_name, (const Il2CppType*));
DO_API(Il2CppType*, il2cpp_method_get_return_type, (const MethodInfo*));

DO_API( void*, il2cpp_alloc, ( std::size_t size ) );
DO_API( void, il2cpp_free, ( void* ptr ) );

DO_API( const Il2CppImage*, il2cpp_assembly_get_image, ( const Il2CppAssembly* assembly ) );

DO_API( bool, il2cpp_class_has_parent, ( Il2CppClass* klass, Il2CppClass* klassc ) );
DO_API( Il2CppClass*, il2cpp_class_from_name, ( const Il2CppImage* image, const char* namespaze, const char* name ) );
DO_API( const EventInfo*, il2cpp_class_get_events, ( const Il2CppClass* klass, void** iter ) );
DO_API( FieldInfo*, il2cpp_class_get_fields, ( const Il2CppClass* klass, void** iter ) );
DO_API( FieldInfo*, il2cpp_class_get_field_from_name, ( const Il2CppClass* klass, const char* name ) );
DO_API( const PropertyInfo*, il2cpp_class_get_properties, ( const Il2CppClass* klass, void** iter ) );
DO_API( const PropertyInfo*, il2cpp_class_get_property_from_name, ( const Il2CppClass* klass, const char* name ) );
DO_API( const MethodInfo*, il2cpp_class_get_methods, ( Il2CppClass* klass, void** iter ) );
DO_API( const MethodInfo*, il2cpp_class_get_method_from_name, ( Il2CppClass* klass, const char* name, int argsCount ) );
DO_API( const char*, il2cpp_class_get_name, ( Il2CppClass* klass ) );
DO_API( const char*, il2cpp_class_get_namespace, ( Il2CppClass* klass ) );
DO_API( Il2CppClass*, il2cpp_class_get_parent, ( Il2CppClass* klass ) );
DO_API(int, il2cpp_method_get_param_count, (uintptr_t));

DO_API( const char*, il2cpp_field_get_name, ( const FieldInfo* field ) );
DO_API( std::size_t, il2cpp_field_get_offset, ( const FieldInfo* field ) );

DO_API( const MethodInfo*, il2cpp_property_get_get_method, ( const PropertyInfo* prop ) );
DO_API( const MethodInfo*, il2cpp_property_get_set_method, ( const PropertyInfo* prop ) );
DO_API( const char*, il2cpp_property_get_name, ( const PropertyInfo* prop ) );

DO_API( Il2CppString*, il2cpp_string_new, ( const char* str ) );

DO_API( Il2CppObject*, il2cpp_runtime_invoke, ( const MethodInfo* method, void* obj, void** params, Il2CppObject** exc ) );
DO_API( void*, il2cpp_object_unbox, ( Il2CppObject* obj ) );

DO_API( const Il2CppAssembly*, il2cpp_image_get_assembly, ( const Il2CppImage* image ) );
DO_API( const char*, il2cpp_image_get_name, ( const Il2CppImage* image ) );
DO_API( const char*, il2cpp_image_get_filename, ( const Il2CppImage* image ) );
DO_API( const MethodInfo*, il2cpp_image_get_entry_point, ( const Il2CppImage* image ) );

DO_API( void, il2cpp_monitor_enter, ( Il2CppObject* obj ) );
DO_API( void, il2cpp_monitor_exit, ( Il2CppObject* obj ) );
DO_API(uintptr_t, il2cpp_object_new, (void*));

bool CreateIL2CPP();
void DestroyIL2CPP();

} // namespace rust