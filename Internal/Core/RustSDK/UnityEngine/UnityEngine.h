#pragma once
#include "../../Globals/Globals.hpp"
#include "../System/System.h"
#include "../RustStructs.h"

#define powFFFFFFFFFFFFFFFFFFFFFF(n) (n)*(n)

namespace UnityEngine {
	IL2CPP_NAME_SPACE(SECURE("UnityEngine"));

	constexpr auto PiRadians = static_cast<float>(3.1415926535897932);
	constexpr auto PiDegrees = static_cast<float>(180.0);

	struct Vector2;
	struct Vector3;
	struct Bounds;
	struct Matrix4x4;

	struct Object;
	struct Component;
	struct Behaviour;
	struct MonoBehaviour;
	struct Transform;
	struct GameObject;
	struct Camera;
	struct Material;
	struct Input;

	class Vector2
	{
	public:
		Vector2();
		Vector2(float vector_x, float vector_y);
		Vector2(const Vector2& vector);

	public:
		float Dot(const Vector2& vector) const;

		float LengthSquared() const;
		float Length() const;

		float DistanceSquared(const Vector2& vector) const;
		float Distance(const Vector2& vector) const;

		float Normalize();

	public:
		Vector2 Normalized();
		Vector2 MemoryAimNormalize();

	public:
		float operator[](std::size_t index) const;
		float& operator[](std::size_t index);

	public:
		Vector2& operator=(const Vector2& vector);

	public:
		Vector2& operator+=(float value);
		Vector2& operator-=(float value);
		Vector2& operator*=(float value);
		Vector2& operator/=(float value);

		Vector2& operator+=(const Vector2& vector);
		Vector2& operator-=(const Vector2& vector);
		Vector2& operator*=(const Vector2& vector);
		Vector2& operator/=(const Vector2& vector);

	public:
		Vector2 operator+(float value) const;
		Vector2 operator-(float value) const;
		Vector2 operator*(float value) const;
		Vector2 operator/(float value) const;

		Vector2 operator+(const Vector2& vector) const;
		Vector2 operator-(const Vector2& vector) const;
		Vector2 operator*(const Vector2& vector) const;
		Vector2 operator/(const Vector2& vector) const;
		bool operator==(const Vector2& input) const
		{
			return x == input.x && y == input.y;
		}
		bool IsZero() const
		{
			return (x == 0.f &&
				y == 0.f);
		}
	public:
		float x = 0.f;	// 0x0000
		float y = 0.f;	// 0x0004
	};

	struct Vector3
	{
		Vector3();
		Vector3(float vector_x, float vector_y, float vector_z);
		Vector3(const Vector3& vector);

		float Dot(const Vector3& vector) const;
		Vector3 operator/(float fInput) const;
		float Length() const;
		float Length2D() const;
		Vector3 Cross(Vector3 rhs)
		{
			return Vector3(y * rhs.z - z * rhs.y, z * rhs.x - x * rhs.z, x * rhs.y - y * rhs.x);
		}

		static float Clamp01(float value)
		{
			if (value < 0.f)
				return 0.f;
			else if (value > 1.f)
				return 1.f;
			else
				return value;
		}

		static float InverseLerp(float a, float b, float value)
		{
			float result;
			if (a != b)
			{
				result = Clamp01((value - a) / (b - a));
			}
			else
			{
				result = 0.f;
			}
			return result;
		}

		static Vector3 Lerp(Vector3 a, Vector3 b, float t)
		{
			t = Clamp01(t);
			return Vector3(
				a.x + (b.x - a.x) * t,
				a.y + (b.y - a.y) * t,
				a.z + (b.z - a.z) * t
			);
		}
		static Vector3 move_towards(Vector3 current, Vector3 target, float maxDistanceDelta)
		{
			float toVector_x = target.x - current.x;
			float toVector_y = target.y - current.y;
			float toVector_z = target.z - current.z;

			float sqdist = toVector_x * toVector_x + toVector_y * toVector_y + toVector_z * toVector_z;

			if (sqdist == 0 || (maxDistanceDelta >= 0 && sqdist <= maxDistanceDelta * maxDistanceDelta))
				return target;
			auto dist = (float)win32::sqrt(sqdist);

			return Vector3(current.x + toVector_x / dist * maxDistanceDelta,
				current.y + toVector_y / dist * maxDistanceDelta,
				current.z + toVector_z / dist * maxDistanceDelta);
		}
		template<typename T>
		T get_remainder(T val, T min, T max)
		{
			while (val > max)
				val -= max * 2;
			while (val < min)
				val += max * 2;
			return val;
		}

		static Vector3 XZ3D(Vector3 v)
		{
			return Vector3(v.x, 0.f, v.z);
		}
		void angle_vectors(Vector3* forward, Vector3* right, Vector3* up)
		{
			float sp, sy, cp, cy, sr, cr;

			constexpr auto PI = 3.14159265358979323846;
			sy = win32::sin(y / 180.f * (float)PI);
			cy = win32::cos(y / 180.f * (float)PI);

			sp = win32::sin(x / 180.f * (float)PI);
			cp = win32::cos(x / 180.f * (float)PI);

			sr = 0.f;
			cr = 1.f;

			if (forward) {
				*forward = { cp * cy, -sp, cp * sy };
			}

			if (right) {
				right->x = cp * sy;
				right->y = (cr * sp * sy + -sr * cy);
				right->z = (sr * sp * sy + cr * cy);
			}

			if (up) {
				up->x = (cr * sp * cy + -sr * -sy);
				up->y = cr * cp;
				up->z = (cr * sp * sy + -sr * cy);
			}
		}

		Vector3 clamp()
		{
			constexpr auto yaw_limit = static_cast<float>(180);
			constexpr auto pitch_limit = static_cast<float>(90);

			y = get_remainder(y, -yaw_limit, yaw_limit);
			x = get_remainder(x, -pitch_limit, pitch_limit);

			/*if ( x > 180 )
				x -= 360;
			else if ( x < -180 )
				x += 360;

			if ( y > 180 )
				y -= 360;
			else if ( y < -180 )
				y += 360;

			if ( x < -89 )
				x = -89;

			if ( x > 89 )
				x = 89;

			while ( y < -180 )
				y += 360;

			while ( y > 180 )
				y -= 360;

			z = 0;*/

			if (x != x)
				x = 0;
			if (y != y)
				y = 0;
			if (z != z)
				z = 0;

			return *this;
		}



		float Distance(const Vector3& vector) const;
		float Distance2D(const Vector3& vector) const;
		float Magnitude();
		float Normalize();
		Vector3 Normalized() const;
		float Magnitude2D();
		Vector3 UnityNormalize();
		bool IsZero() const;
		float UnityMagnitude();
		Vector3 ToVector() const;
		Vector3 ToEulerAngles() const;

		float get_3d_dist(const Vector3& Dst);
		float operator[](std::size_t index) const;
		float& operator[](std::size_t index);

		Vector3 operator-() const;

		Vector3& operator-=(const Vector3& vInput);

		Vector3& operator+=(const Vector3& vInput);
		Vector3& operator*=(const Vector3& vInput);
		Vector3& operator*=(float vInput);
		Vector3 operator*(float fInput) const;
		Vector3 operator+(const Vector3& vector) const;
		Vector3 operator-(const Vector3& vector) const;
		Vector3 operator*(const Vector3& vector) const;
		Vector3 operator/(const Vector3& vector) const;
		bool operator!=(const Vector3& vInput) const;
		bool operator==(const Vector3& input) const
		{
			return x == input.x && y == input.y && z == input.z;
		}

		Vector3& operator/=(const Vector3& vInput);

		Vector3& operator/=(float fInput);

		Vector3 Scale(const float times) const;

		static Vector3 Zero;

		float x = 0.f;
		float y = 0.f;
		float z = 0.f;

		template<typename genType>
		static bool epsilonEqual(genType const& x, genType const& y, genType const& epsilon)
		{
			genType diff = x - y;
			return win32::abs(diff) <= epsilon;
		}

		static float Clamp(float value, float min, float max)
		{
			bool flag = value < min;
			if (flag)
			{
				value = min;
			}
			else
			{
				bool flag2 = value > max;
				if (flag2)
				{
					value = max;
				}
			}
			return value;
		}

		static float Angle(Vector3 from, Vector3 to)
		{
			float num = (float)win32::sqrt((double)(from.Magnitude() * to.Magnitude()));
			bool flag = num < 1E-15f;
			float result;
			if (flag)
			{
				result = 0.f;
			}
			else
			{
				float num2 = Clamp(from.Dot(to) / num, -1.f, 1.f);
				result = (float)win32::acos((double)num2) * 57.29578f;
			}
			return result;
		}

		static Vector3 Forward;
		static Vector3 Back;
	};

	struct RaycastHit // TypeDefIndex: 3913
	{
		// Fields
		Vector3 m_Point; // 0x0
		Vector3 m_Normal; // 0xC
		uintptr_t m_FaceID; // 0x18
		float m_Distance; // 0x1C
		Vector2 m_UV; // 0x20
		int m_Collider; // 0x28
	};

	inline float ToRadians(float degrees)
	{
		return (degrees * (PiRadians / PiDegrees));
	}

	class Vector4
	{
	public:
		Vector4()
		{
			x = y = z = w = 0.f;
		}

		Vector4(float fx, float fy, float fz, float fw)
		{
			x = fx;
			y = fy;
			z = fz;
			w = fw;
		}

		float x, y, z, w;

		Vector4 operator+(const Vector4& input) const
		{
			return Vector4{ x + input.x, y + input.y, z + input.z, w + input.w };
		}

		Vector4 operator-(const Vector4& input) const
		{
			return Vector4{ x - input.x, y - input.y, z - input.z, w - input.w };
		}

		Vector4 operator/(float input) const
		{
			return Vector4{ x / input, y / input, z / input, w / input };
		}

		Vector4 operator*(float input) const
		{
			return Vector4{ x * input, y * input, z * input, w * input };
		}

		Vector4& operator-=(const Vector4& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;

			return *this;
		}

		Vector4& operator/=(float input)
		{
			x /= input;
			y /= input;
			z /= input;
			w /= input;
			return *this;
		}

		Vector4& operator*=(float input)
		{
			x *= input;
			y *= input;
			z *= input;
			w *= input;
			return *this;
		}

		bool operator==(const Vector4& input) const
		{
			return x == input.x && y == input.y && z == input.z && w == input.w;
		}
		static Vector4 QuaternionLookRotation(Vector3 forward, Vector3 up)
		{

			Vector3 vector = forward.UnityNormalize();
			Vector3 vector2 = (up).Cross(vector).UnityNormalize();
			Vector3 vector3 = (vector).Cross(vector2);
			auto m00 = vector2.x;
			auto m01 = vector2.y;
			auto m02 = vector2.z;
			auto m10 = vector3.x;
			auto m11 = vector3.y;
			auto m12 = vector3.z;
			auto m20 = vector.x;
			auto m21 = vector.y;
			auto m22 = vector.z;


			float num8 = (m00 + m11) + m22;
			auto quaternion = Vector4();
			if (num8 > 0.f)
			{
				auto num = (float)win32::sqrt(num8 + 1.f);
				quaternion.w = num * 0.5f;
				num = 0.5f / num;
				quaternion.x = (m12 - m21) * num;
				quaternion.y = (m20 - m02) * num;
				quaternion.z = (m01 - m10) * num;
				return quaternion;
			}
			if ((m00 >= m11) && (m00 >= m22))
			{
				auto num7 = (float)win32::sqrt(((1.f + m00) - m11) - m22);
				auto num4 = 0.5f / num7;
				quaternion.x = 0.5f * num7;
				quaternion.y = (m01 + m10) * num4;
				quaternion.z = (m02 + m20) * num4;
				quaternion.w = (m12 - m21) * num4;
				return quaternion;
			}
			if (m11 > m22)
			{
				auto num6 = (float)win32::sqrt(((1.f + m11) - m00) - m22);
				auto num3 = 0.5f / num6;
				quaternion.x = (m10 + m01) * num3;
				quaternion.y = 0.5f * num6;
				quaternion.z = (m21 + m12) * num3;
				quaternion.w = (m20 - m02) * num3;
				return quaternion;
			}
			auto num5 = (float)win32::sqrt(((1.f + m22) - m00) - m11);
			auto num2 = 0.5f / num5;
			quaternion.x = (m20 + m02) * num2;
			quaternion.y = (m21 + m12) * num2;
			quaternion.z = 0.5f * num5;
			quaternion.w = (m01 - m10) * num2;
			return quaternion;
		}


		Vector3 Multiply(CONST Vector3& Point)
		{
			float num = x * 2.00f;
			float num2 = y * 2.00f;
			float num3 = z * 2.00f;
			float num4 = x * num;
			float num5 = y * num2;
			float num6 = z * num3;
			float num7 = x * num2;
			float num8 = x * num3;
			float num9 = y * num3;
			float num10 = w * num;
			float num11 = w * num2;
			float num12 = w * num3;

			Vector3 result;
			result.x = (1.00f - (num5 + num6)) * Point.x +
				(num7 - num12) * Point.y + (num8 + num11) * Point.z;
			result.y = (num7 + num12) * Point.x +
				(1.00f - (num4 + num6)) * Point.y +
				(num9 - num10) * Point.z;
			result.z = (num8 - num11) * Point.x + (num9 + num10) * Point.y +
				(1.00f - (num4 + num5)) * Point.z;
			return result;
		}
		void make_absolute()
		{
			x = win32::abs(x);
			y = win32::abs(y);
			z = win32::abs(z);
			w = win32::abs(w);
		}

		float length_sqr() const
		{
			return (x * x) + (y * y) + (z * z) + (w * w);
		}

		float length() const
		{
			return win32::sqrt(length_sqr());
		}

		float length_2d() const
		{
			return win32::sqrt((x * x) + (y * y));
		}

		Vector4 normalized() const
		{
			return { x / length(), y / length(), z / length(), w / length() };
		}

		float dot_product(Vector4 input) const
		{
			return (x * input.x) + (y * input.y) + (z * input.z) + (w * input.w);
		}

		float distance(Vector4 input) const
		{
			return (*this - input).length();
		}

		float distance_2d(Vector4 input) const
		{
			return (*this - input).length_2d();
		}
		void clamp()
		{
			static_cast<void>(std::clamp(x, -89.f, 89.f));
			static_cast<void>(std::clamp(y, -180.f, 180.f));

			z = 0.f;
			w = 0.f;
		}

		bool empty() const
		{
			return x == 0.f && y == 0.f && z == 0.f && w == 0.f;
		}
	};

	struct Bounds
	{
		Vector3 m_Center = { };		// 0x0000
		Vector3 m_Extents = { };	// 0x000C
	};

	struct Object : Il2CppObject
	{
		bool IsA(const Il2CppClass* object);

		IL2CPP_CLASS("Object");

		IL2CPP_FIELD(uintptr_t, m_CachedPtr);
		IL2CPP_PROPERTY(System::String*, name);

		void set_name(const char* str)
		{
			if (!this)return;
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_name"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, CIl2Cpp::il2cpp_string_new(str));
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		static void DontDestroyOnLoad(Object* targ);

		static bool IsNativeObjectAlive(Object* objvc)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("IsNativeObjectAlive"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, objvc);
			}

			TRACE("%s: procedure not a valid!", FN);
			return false;
		}

	/*	bool IsNativeObjectAlive()
		{
			auto load_from_memory_internal = reinterpret_cast<bool * (*)(Object*)>(CIl2Cpp::il2cpp_resolve_icall(SECURE("UnityEngine.Object::IsNativeObjectAlive")));
			return load_from_memory_internal(this);
		}*/
	};

	struct AssetBundle : Il2CppObject {
		IL2CPP_CLASS("AssetBundle");

		template<typename T = uintptr_t>
		T* LoadAsset(const char* name, Il2CppType* type) {
			auto _this = memory::ToAddress(this);
			auto load_from_memory_internal = reinterpret_cast<T * (*)(AssetBundle*, Il2CppString*, Il2CppType*)>(CIl2Cpp::il2cpp_resolve_icall(SECURE("UnityEngine.AssetBundle::LoadAsset_Internal")));
			return load_from_memory_internal(this, CIl2Cpp::il2cpp_string_new(name), type);
		}
		static AssetBundle* LoadFromMemory_Internal(System::c_system_array<System::Byte*>* assetBundle, uint32_t CRC, uint64_t offset);
		void Unload(bool all);


		static AssetBundle* LoadFromFile_Internal(const char* a1, int a2, int a3)
		{
			auto load_from_memory_internal = reinterpret_cast<AssetBundle * (*)(Il2CppString*, uint32_t CRC, uint64_t offset)>(CIl2Cpp::il2cpp_resolve_icall(SECURE("UnityEngine.AssetBundle::LoadFromFile_Internal")));
			return load_from_memory_internal(CIl2Cpp::il2cpp_string_new(a1), a2, a3);
		}
	};

	struct JsonUtility : Il2CppObject {
		IL2CPP_CLASS("JsonUtility");


		template<typename T = GameObject>
		static T* FromJson(const char* name, System::Type* type) {
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("FromJson"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<T*>(procedure, CIl2Cpp::il2cpp_string_new(name), type);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}
	};

	struct Shader : Il2CppObject {
		IL2CPP_CLASS("Shader");

		static Shader* Find(char* name) {
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Find"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<Shader*>(procedure, CIl2Cpp::il2cpp_string_new(name));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return 0;
			}
		}
		static int PropertyToID(char* name) {
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("PropertyToID"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<int>(procedure, CIl2Cpp::il2cpp_string_new(name));
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return 0;
			}
		}
	};


	struct Material : Object
	{
		IL2CPP_CLASS("Material");

		void SetColor(int proper, Color value) {
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SetColor"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				memory::Call<void>(procedure, this, proper, value);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}
		bool SetPass(int value)
		{
			const auto set_int = reinterpret_cast<bool (*)(Material*, int)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("SetPass"), 1)));
			return set_int(this, value);
		}

		inline static void CreateWithShader(Material* a1, Shader* a2) {
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("CreateWithShader"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);

				}
			}

			if (memory::IsAddressValid(procedure))
			{
				memory::Call<void>(procedure, a1, a2);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}
		void SetColor(char* proper, Color value) {
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SetColor"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				memory::Call<void>(procedure, this, CIl2Cpp::il2cpp_string_new(proper), value);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}
		void SetInt(char* name, int value) {
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SetInt"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				memory::Call<void>(procedure, this, CIl2Cpp::il2cpp_string_new(name), value);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}
		void SetFloat(char* name, float value) {
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SetFloat"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				memory::Call<void>(procedure, this, CIl2Cpp::il2cpp_string_new(name), value);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}
		Shader* shader() {
			if (!this) return nullptr;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("get_shader"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<Shader*>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return nullptr;
			}
		}
		void set_shader(Shader* val) {
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_shader"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				memory::Call<void>(procedure, this, val);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}
	};

	struct RenderSettings : Object
	{
		IL2CPP_CLASS("RenderSettings");

		IL2CPP_STATIC_FIELD(Material*, skybox);


		static Material* get_skybox()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("get_skybox"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<Material*>(procedure);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}
		}

		static void SetSkybox(Material* Value)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_skybox"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, Value);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		static void SetAmbientMode(RustStructs::AmbientMode Value)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_ambientMode"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, Value);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		static void SetAmbientIntensity(float Value)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_ambientIntensity"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, Value);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		static void SetAmbientLight(Color Value)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_ambientLight"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, Value);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}
	};

	struct GameObject : Object
	{
		IL2CPP_CLASS("GameObject");
		IL2CPP_PROPERTY(Transform*, transform);
		IL2CPP_PROPERTY(std::int32_t, layer);
		IL2CPP_PROPERTY(System::String*, tag);
		IL2CPP_PROPERTY(GameObject*, gameObject);


		UnityEngine::Component* AddComponent(System::Type* type);

		template<typename T = GameObject>
		T* GetComponent(System::Type* type) {
			if (!this || !type) return nullptr;

			const auto get_component = reinterpret_cast<T * (*)(GameObject*, System::Type*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("GetComponent"), 1)));
			return memory::Call<T*>((uintptr_t)get_component, this, type);
		}
	};

	struct Texture : Object
	{
		IL2CPP_CLASS("Texture");

		int get_width()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("get_width"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<int>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return 0;
			}
		}

		int get_height()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("get_height"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<int>(procedure, this);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return 0;
			}
		}


	};
	struct Random : Il2CppObject {
		IL2CPP_CLASS("Random");


		inline static float Range(float a1, float a2) {
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Range"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<float>(procedure, a1, a2);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return false;
			}
		}
	};
	struct Texture2D : Texture
	{
		IL2CPP_CLASS("Texture2D");

		IL2CPP_STATIC_PROPERTY(Texture2D*, whiteTexture);
		IL2CPP_STATIC_PROPERTY(Texture2D*, blackTexture);

		void _cctor(int width, int height)
		{
			if (!this) return;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH(".ctor"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, width, height);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}

		}


		void SetPixel(int width, int height, Color clr)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SetPixel"), 3);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, width, height,clr);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return;
			}
		}

		void Apply()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Apply"), 0);
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


		//static Texture2D* _cctor(Texture2D* tex,int width, int height, RustStructs::TextureFormat textureFormat, bool mipChain)
		//{
		//	static uintptr_t procedure = 0;
		//	if (!memory::IsAddressValid(procedure))
		//	{
		//		const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH(".ctor"), 4);
		//		if (memory::IsAddressValid(method))
		//		{
		//			procedure = memory::ToAddress(method->methodPointer);
		//		}

		//		printfz("\n procedure2 - %p", procedure);
		//	}

		//	if (memory::IsAddressValid(procedure))
		//	{
		//		return memory::Call<Texture2D*>(procedure, tex, width, height, textureFormat, mipChain);
		//	}
		//	else
		//	{
		//		TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
		//		return;
		//	}
		//}

		static Texture2D* New2()
		{


			uintptr_t Texture2DClass = 0;
			if (!Texture2DClass)
			{
				Texture2DClass = (uintptr_t)CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture2D"));
			}
			UnityEngine::Texture2D* texture2D = reinterpret_cast<UnityEngine::Texture2D*>(CIl2Cpp::il2cpp_object_new(memory::ToPointer(Texture2DClass)));
			return texture2D;
		}
		static Texture2D* New(int width, int height)
		{


			uintptr_t Texture2DClass = 0; 
			if (!Texture2DClass)
			{ 
				Texture2DClass = (uintptr_t)CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Texture2D")); 
			}
			UnityEngine::Texture2D* texture2D = reinterpret_cast<UnityEngine::Texture2D*>(CIl2Cpp::il2cpp_object_new(memory::ToPointer(Texture2DClass)));

			texture2D->_cctor(width, height);
			return texture2D;
		}

		
		
	};

	struct rect_t
	{
		float x, y, w, h;
		rect_t(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h) { }
		rect_t() : x(0), y(0), w(0), h(0) { }

		auto contains(Vector2 point) -> bool
		{
			return point.x >= x && point.x < (x + w) && point.y >= y && point.y < (y + h);
		}
	};
	struct Renderer;
	struct Component : Object
	{
		Vector3 get_positionz();
		IL2CPP_CLASS("Component");

		IL2CPP_PROPERTY(GameObject*, gameObject);


		Transform* get_transform();
		const char* class_name();
			

		uint64_t class_name_hash();

		template<typename T = Component>
		T* GetComponent(System::Type* type) {
			if (!this) return nullptr;
			const auto get_component = reinterpret_cast<T * (*)(Component*, System::Type*)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(this->StaticClass(), HASH("GetComponent"), 1)));
			return memory::Call<T*>((uintptr_t)get_component, this, type);
		}

		System::Array<Renderer*>* GetComponentsInChildren(System::Type* type);
	};


	struct Renderer : Il2CppObject
	{
		IL2CPP_CLASS("Renderer");

		Material* material();

		void set_material(Material* value);
	};

	struct Ray {
		Vector3 origin;
		Vector3 dir;
		Ray(Vector3 o, Vector3 d) {
			origin = o;
			dir = d;
		}
	};

	struct Time
	{
		IL2CPP_CLASS("Time");


		IL2CPP_STATIC_PROPERTY(float, timeScale);
		IL2CPP_STATIC_PROPERTY(float, deltaTime);
		IL2CPP_STATIC_PROPERTY(float, fixedTime);
		IL2CPP_STATIC_PROPERTY(float, time);
		IL2CPP_STATIC_PROPERTY(float, realtimeSinceStartup);
		IL2CPP_STATIC_PROPERTY(float, fixedDeltaTime);
		IL2CPP_STATIC_PROPERTY(float, smoothDeltaTime);
		//SPOOF_STATIC_PROPERTY(Vector3, gravity);

	};


	struct Physics
	{
		IL2CPP_CLASS("Physics");

		IL2CPP_STATIC_PROPERTY(Vector3, gravity);


		static void IgnoreLayerCollision(int layer1, int layer2, bool ignore);
		static bool CheckCapsule(Vector3 start, Vector3 end, float radius, int layerMask, RustStructs::QueryTriggerInteraction triggerInteraction);
		inline static bool SphereCast(Ray a1, float a2, float a3, int a4) {
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SphereCast"), 4);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, a1, a2, a3, a4);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return false;
			}
		}

		inline static bool Raycast(Ray a1, UnityEngine::RaycastHit hit, float a3, int a4, RustStructs::QueryTriggerInteraction a5) {
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Raycast"), 5);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, a1, hit, a3, a4, a5);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return false;
			}
		}

		inline static bool Raycast(Ray a1, float a2, int a4) {
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Raycast"), 3);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, a1, a2, a4);
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return false;
			}
		}
	};

	struct Matrix4x4 : Il2CppObject {
		IL2CPP_CLASS("Matrix4x4");


		Vector3 MultiplyPoint3x4(Vector3 value)
		{
			if (!this)
				return Vector3();

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethodFullArgs(HASH("UnityEngine.CoreModule::UnityEngine::Matrix4x4::MultiplyPoint3x4(Vector3): Vector3"));
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<Vector3>(procedure, this, value);
			}

			TRACE("%s: procedure not a valid!", FN);
			return Vector3();
		}
	};

	struct Transform : Component
	{
		IL2CPP_CLASS("Transform");

		Vector3 get_position();

		//IL2CPP_PROPERTY(Vector3, position);
		IL2CPP_PROPERTY(Vector3, localPosition);
		IL2CPP_PROPERTY(Vector3, eulerAngles);
		IL2CPP_PROPERTY(Vector3, localEulerAngles);
		IL2CPP_PROPERTY(Vector3, right);
		IL2CPP_PROPERTY(Vector3, up);
		IL2CPP_PROPERTY(Vector3, forward);
		IL2CPP_PROPERTY(Vector3, localScale);

		IL2CPP_PROPERTY(Matrix4x4*, localToWorldMatrix);

		
		auto InverseTransformPoint(Vector3 pt) -> Vector3;
		auto InverseTransformDirection(Vector3 pt) -> Vector3;

		Transform* get_parent();
		auto set_position(Vector3 pos) -> void;
		auto set_rotation(Vector4 pos) -> void;
		auto set_localscale(Vector3 amount) -> void;
	};

	struct Behaviour : Component
	{
		IL2CPP_CLASS("Behaviour");
	};

	struct MonoBehaviour : Behaviour
	{
		IL2CPP_CLASS("MonoBehaviour");
	};

	struct Input : Il2CppObject
	{
		IL2CPP_CLASS("Input");

		static bool GetMouseButton(int a1);
		static Vector3 GetMousePosition();
		static bool GetKey(RustStructs::KeyCode code);
		static bool GetKeyDown(RustStructs::KeyCode code);
	};

	struct Event {
		IL2CPP_CLASS("Event");

		static Event* Current();
		static RustStructs::KeyCode get_keyCode(Event* current);
		Vector2 Delta();
		Vector2 mousePosition();
		RustStructs::EventType Type();

		void Use()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Use"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}
	};

	struct GUIContent;

	struct GUIStyleState : Il2CppObject {
		IL2CPP_CLASS("GUIStyleState");

	};

	struct GUIStyle : Il2CppObject {
		IL2CPP_CLASS("GUIStyle");

		IL2CPP_FIELD(GUIStyleState*, m_Normal);

		void SetFontSize(int value);
		static void SetAlignment(GUIStyle* style, int text);

		void Internal_CalcSize_Injected(GUIContent* content, Vector2 ret)
		{
			auto ga = win32::GetImage(L"GameAssembly.dll");

			auto ga2 = *reinterpret_cast<uintptr_t*>(ga + 0x25AB820);

			return memory::Call<void>(ga2, this, content, ret);


			//TRACE("%s: procedure not a valid!", FN);
			//return;
		}
	};

	struct GUISkin : Object {
		IL2CPP_CLASS("GUISkin");
		IL2CPP_FIELD(GUIStyle*, m_label);
		IL2CPP_FIELD(uintptr_t, m_Font);
		IL2CPP_FIELD(GUIStyle*, m_box);

		static GUIStyle* GetLabel();
	};

	class Rect {
	public:
		float x; // 0x10
		float y; // 0x14
		float wid; // 0x18
		float hei; // 0x1C
		Rect(float x, float y/*top left*/, float x_rightsize, float y_downsize) {
			this->x = x;
			this->y = y;
			wid = x_rightsize;
			hei = y_downsize;
		}
		Rect() {
			this->x = 0;
			this->y = 0;
			wid = 0;
			hei = 0;
		}
		bool Contains(Vector2 point)
		{
			return point.x >= x && point.x < (x + wid) && point.y >= y && point.y < (y + hei);
		}
	};

	struct VMatrix
	{
		VMatrix()
			: m{ { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 },
				 { 0, 0, 0, 0 } }
		{}

		VMatrix(const VMatrix&) = default;

		VMatrix transpose() {
			VMatrix m;

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					m.m[i][j] = this->m[j][i];

			return m;
		}

		void matrix_identity() {
			memset(this, 0, sizeof(VMatrix));
			m[0][0] = 1.0f;
			m[1][1] = 1.0f;
			m[2][2] = 1.0f;
			m[3][3] = 1.0f;
		}
		Vector3 GetRow(std::size_t index) const
		{
			return { m[index][0], m[index][1], m[index][2] };
		}

		Vector3 GetColumn(std::size_t index) const
		{
			return { m[0][index], m[1][index], m[2][index] };
		}

		Vector3 GetAxis(std::size_t index) const
		{
			return { m[index][0], m[index][1], m[index][2] };
		}
		bool is_empty() {
			if (!m[3][0] && !m[3][1] && !m[3][2] && !m[2][1] && !m[2][0] && !m[2][2])
				return true;

			return false;
		}

		Vector3 operator*(const Vector3& vec) {
			VMatrix m;

			m[3][0] = vec.x;
			m[3][1] = vec.y;
			m[3][2] = vec.z;

			m[0][0] = 1;
			m[1][1] = 1;
			m[2][2] = 1;


			m[0][3] = 0.0f;
			m[1][3] = 0.0f;
			m[2][3] = 0.0f;
			m[3][3] = 1.0f;

			auto result = m * (*this);

			return Vector3{ result[3][0], result[3][1], result[3][2] };
		}

		VMatrix operator*(const VMatrix& _m2) {
			auto _m = *this;

			VMatrix out;
			out[0][0] = _m[0][0] * _m2[0][0] + _m[0][1] * _m2[1][0] + _m[0][2] * _m2[2][0] + _m[0][3] * _m2[3][0];
			out[0][1] = _m[0][0] * _m2[0][1] + _m[0][1] * _m2[1][1] + _m[0][2] * _m2[2][1] + _m[0][3] * _m2[3][1];
			out[0][2] = _m[0][0] * _m2[0][2] + _m[0][1] * _m2[1][2] + _m[0][2] * _m2[2][2] + _m[0][3] * _m2[3][2];
			out[0][3] = _m[0][0] * _m2[0][3] + _m[0][1] * _m2[1][3] + _m[0][2] * _m2[2][3] + _m[0][3] * _m2[3][3];
			out[1][0] = _m[1][0] * _m2[0][0] + _m[1][1] * _m2[1][0] + _m[1][2] * _m2[2][0] + _m[1][3] * _m2[3][0];
			out[1][1] = _m[1][0] * _m2[0][1] + _m[1][1] * _m2[1][1] + _m[1][2] * _m2[2][1] + _m[1][3] * _m2[3][1];
			out[1][2] = _m[1][0] * _m2[0][2] + _m[1][1] * _m2[1][2] + _m[1][2] * _m2[2][2] + _m[1][3] * _m2[3][2];
			out[1][3] = _m[1][0] * _m2[0][3] + _m[1][1] * _m2[1][3] + _m[1][2] * _m2[2][3] + _m[1][3] * _m2[3][3];
			out[2][0] = _m[2][0] * _m2[0][0] + _m[2][1] * _m2[1][0] + _m[2][2] * _m2[2][0] + _m[2][3] * _m2[3][0];
			out[2][1] = _m[2][0] * _m2[0][1] + _m[2][1] * _m2[1][1] + _m[2][2] * _m2[2][1] + _m[2][3] * _m2[3][1];
			out[2][2] = _m[2][0] * _m2[0][2] + _m[2][1] * _m2[1][2] + _m[2][2] * _m2[2][2] + _m[2][3] * _m2[3][2];
			out[2][3] = _m[2][0] * _m2[0][3] + _m[2][1] * _m2[1][3] + _m[2][2] * _m2[2][3] + _m[2][3] * _m2[3][3];
			out[3][0] = _m[3][0] * _m2[0][0] + _m[3][1] * _m2[1][0] + _m[3][2] * _m2[2][0] + _m[3][3] * _m2[3][0];
			out[3][1] = _m[3][0] * _m2[0][1] + _m[3][1] * _m2[1][1] + _m[3][2] * _m2[2][1] + _m[3][3] * _m2[3][1];
			out[3][2] = _m[3][0] * _m2[0][2] + _m[3][1] * _m2[1][2] + _m[3][2] * _m2[2][2] + _m[3][3] * _m2[3][2];
			out[3][3] = _m[3][0] * _m2[0][3] + _m[3][1] * _m2[1][3] + _m[3][2] * _m2[2][3] + _m[3][3] * _m2[3][3];

			return out;
		}

		float* operator[](size_t i) { return m[i]; }
		const float* operator[](size_t i) const { return m[i]; }

		union {
			struct {
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float m[4][4];
		};
	};

	struct Screen {
		IL2CPP_CLASS("Screen");


		IL2CPP_STATIC_PROPERTY(int, width);
		IL2CPP_STATIC_PROPERTY(int, height);
	};

	struct Sprite : Object {
		IL2CPP_CLASS("Sprite");
		IL2CPP_PROPERTY(Texture2D*, texture);
		IL2CPP_PROPERTY(Rect, rect);

	};

	struct Camera : Behaviour
	{
		struct View
		{
			FIELD_PAD(0x00DC);				// 0x0000
			VMatrix viewMatrix = { };	        // 0x00DC
		};

		View* view = nullptr;					// 0x0018
		bool WorldToScreen2(const Vector3& position, Vector3& output);

		bool WorldToScreen(const Vector3& position, Vector2& screen_pos);
		Vector3 WorldToScreen(Vector3 position);
		IL2CPP_CLASS("Camera");
		IL2CPP_STATIC_PROPERTY(Camera*, main);
		IL2CPP_STATIC_PROPERTY(Camera*, current);
		IL2CPP_PROPERTY(float, fieldOfView);
		//IL2CPP_PROPERTY(Matrix4x4, worldToCameraMatrix);
		//IL2CPP_PROPERTY(Matrix4x4, projectionMatrix);

		static auto viewMatrix() -> VMatrix;
	};

	enum FocusType
	{
		Native,
		Keyboard,
		Passive
	};
	struct ScrollViewState : Il2CppObject {
		Rect position;
		Rect visibleRect;
		Rect viewRect;
		Vector2 scrollPosition;
		bool apply;
	};

	struct GUIUtility : Il2CppObject {
		IL2CPP_CLASS("GUIUtility");

		static ScrollViewState* GetStateObject(System::Type* type, int controlID)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetStateObject"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<ScrollViewState*>(procedure, type, controlID);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}

		static void CheckOnGUI()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("CheckOnGUI"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		static int GetControlID(int hash, FocusType type)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetControlID"), 2);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<int>(procedure, hash, type);
			}

			TRACE("%s: procedure not a valid!", FN);
			return 0;
		}
	};

	struct GUIContent;


	struct GUILayout {
		IL2CPP_CLASS("GUILayout");


		static void BeginArea(rect_t screenRect)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("BeginArea"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, screenRect);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		static void EndArea()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("EndArea"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}
	};

	struct GUI {
		IL2CPP_CLASS("GUI");

		IL2CPP_STATIC_FIELD(Material*, roundedRectMaterial);
		IL2CPP_STATIC_FIELD(int, s_ScrollviewHash);


		static GUISkin* GetSkin();
		static void SetColor(Color clr);
		static void DrawTexture(UnityEngine::rect_t a1, UnityEngine::Texture2D* a2);
		static void Label(UnityEngine::rect_t position, Unity_String text, uintptr_t style);
		static void LabelTest(UnityEngine::rect_t position, Il2CppString* text, uintptr_t style);
		static void BeginGroup(UnityEngine::rect_t position, GUIContent* content, GUIStyle* style, Vector2 ScrollOffset)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("BeginGroup"), 4);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, position, content, style, ScrollOffset);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		static void EndGroup()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("EndGroup"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}
	};

	struct GUIContent : GUI {
		IL2CPP_CLASS("GUIContent");

		static uintptr_t Temp(Unity_String lol)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Temp"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<uintptr_t>(procedure, lol);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}
	};

	struct DDraw : Il2CppObject {
		IL2CPP_CLASS("DDraw");

		static GUISkin* Arrow(Vector3 vPos, Vector3 vPosB, float headSize, Color color, float fDuration);
		static void Sphere(Vector3 vPos, float fRadius, Color color, float fDuration, bool distanceFade);
		static void Line(Vector3 vPos, Vector3 vPosB, Color color, float fDuration, bool fade, bool ztest);
		static void Text(const char* text, Vector3 vPos, Color color, float fDuration);
		static void SphereGizmo(Vector3 vPos, float fRadius, Color color, float fDuration, bool distanceFade, bool ztest);
	};

	struct Collider : GameObject {
		IL2CPP_CLASS("Collider");


		

		void set_enabled(bool value)
		{
			if (!this)return;
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("set_enabled"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this, value);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}
	};
	
	inline UnityEngine::Material* LineMat = nullptr;
	struct GL {
		IL2CPP_CLASS("GL");
		static void PushMatrix() {
			const auto draw_texture = reinterpret_cast<void(*)()>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("PushMatrix"), 0)));
			return memory::Call<void>(memory::ToAddress(draw_texture));
		}
		static void PopMatrix() {
			const auto draw_texture = reinterpret_cast<void(*)()>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("PopMatrix"), 0)));
			return memory::Call<void>(memory::ToAddress(draw_texture));
		}
		static void Begin(int mode) {
			const auto draw_texture = reinterpret_cast<void(*)(int)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("Begin"), 1)));
			return memory::Call<void>(memory::ToAddress(draw_texture), mode);
		}
		static void End() {
			const auto draw_texture = reinterpret_cast<void(*)()>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("End"), 0)));
			return memory::Call<void>(memory::ToAddress(draw_texture));
		}

		static void Colorz(Color clr) {
			const auto draw_texture = reinterpret_cast<void(*)(Color)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("Color"), 1)));
			return memory::Call<void>(memory::ToAddress(draw_texture), clr);
		}

		static void Vertex(Vector3 zzz) {
			const auto draw_texture = reinterpret_cast<void(*)(Vector3)>(*reinterpret_cast<uintptr_t*>((uintptr_t)CIl2Cpp::FindMethod(StaticClass(), HASH("Vertex"), 1)));
			return memory::Call<void>(memory::ToAddress(draw_texture), zzz);
		}

		static auto gl_start(int mode) -> void
		{
			if (!LineMat)
			{
				LineMat = (UnityEngine::Material*)CIl2Cpp::il2cpp_object_new((void*)CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material")));
				UnityEngine::Material::CreateWithShader(LineMat, UnityEngine::Shader::Find((SECURE("Hidden/Internal-Colored"))));
			}

			PushMatrix();
			LineMat->SetPass(0);
			Begin(mode);
		}

		static auto gl_end() -> void
		{
			End();
			PopMatrix();
		}

		static void Rectangle(Vector2 pos, Vector2 size, Color clr)
		{
			//if (!LineMat)
			//{
				LineMat = (UnityEngine::Material*)CIl2Cpp::il2cpp_object_new((void*)CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material")));
				UnityEngine::Material::CreateWithShader(LineMat, UnityEngine::Shader::Find((SECURE("Hidden/Internal-Colored"))));

				LineMat->SetInt((SECURE("_SrcBlend")), 5);
				LineMat->SetInt((SECURE("_DstBlend")), 10);
				LineMat->SetInt((SECURE("_Cull")), 0);
				LineMat->SetInt((SECURE("_ZWrite")), 0);
				UnityEngine::Object::DontDestroyOnLoad((UnityEngine::Object*)LineMat);
			//}

			gl_start(RustStructs::primitive_mode::line_strip);
			Colorz(clr);
			Vertex(Vector3(pos.x + 0.5f, pos.y + 0.5f, 0));
			Vertex(Vector3(pos.x + size.x + 0.5f, pos.y + 0.5f, 0));
			Vertex(Vector3(pos.x + size.x + 0.5f, pos.y + size.y + 0.5f, 0));
			Vertex(Vector3(pos.x + 0.5f, pos.y + size.y + 0.5f, 0));
			Vertex(Vector3(pos.x + 0.5f, pos.y + 0.5f, 0));
			gl_end();
		}

		static void Line(Vector2 start, Vector2 end, Color color, float size = 1.f) {

			//if (!LineMat)
			//{
				LineMat = (UnityEngine::Material*)CIl2Cpp::il2cpp_object_new((void*)CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material")));
				UnityEngine::Material::CreateWithShader(LineMat, UnityEngine::Shader::Find((SECURE("Hidden/Internal-Colored"))));

				LineMat->SetInt((SECURE("_SrcBlend")), 5);
				LineMat->SetInt((SECURE("_DstBlend")), 10);
				LineMat->SetInt((SECURE("_Cull")), 0);
				LineMat->SetInt((SECURE("_ZWrite")), 0);
				UnityEngine::Object::DontDestroyOnLoad((UnityEngine::Object*)LineMat);
			//}

			gl_start(RustStructs::primitive_mode::lines);
			Colorz(color);

			Vertex(Vector3(start.x, start.y, size));
			Vertex(Vector3(end.x, end.y, size));
			gl_end();
		}


		static void GlFillRectangle(Vector2 pos, Vector2 size, Color col) {
			if (!LineMat)
			{
				LineMat = (UnityEngine::Material*)CIl2Cpp::il2cpp_object_new((void*)CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material")));
				UnityEngine::Material::CreateWithShader(LineMat, UnityEngine::Shader::Find((SECURE("Hidden/Internal-Colored"))));

				LineMat->SetInt((SECURE("_SrcBlend")), 5);
				LineMat->SetInt((SECURE("_DstBlend")), 10);
				LineMat->SetInt((SECURE("_Cull")), 0);
				LineMat->SetInt((SECURE("_ZWrite")), 0);
				UnityEngine::Object::DontDestroyOnLoad((UnityEngine::Object*)LineMat);
			}

			gl_start(RustStructs::primitive_mode::quads);
			Colorz(col);
			Vertex(Vector3(pos.x, pos.y, 0));
			Vertex(Vector3(pos.x + size.x, pos.y, 0));
			Vertex(Vector3(pos.x + size.x, pos.y + size.y, 0));
			Vertex(Vector3(pos.x, pos.y + size.y, 0));
			gl_end();
		}
		static void GradientFillRect(Vector2 pos, Vector2 size, Color col, Color col2) {
			if (!LineMat)
			{
				LineMat = (UnityEngine::Material*)CIl2Cpp::il2cpp_object_new((void*)CIl2Cpp::FindClass(SECURE("UnityEngine"), SECURE("Material")));
				UnityEngine::Material::CreateWithShader(LineMat, UnityEngine::Shader::Find((SECURE("Hidden/Internal-Colored"))));

				LineMat->SetInt((SECURE("_SrcBlend")), 5);
				LineMat->SetInt((SECURE("_DstBlend")), 10);
				LineMat->SetInt((SECURE("_Cull")), 0);
				LineMat->SetInt((SECURE("_ZWrite")), 0);
				UnityEngine::Object::DontDestroyOnLoad((UnityEngine::Object*)LineMat);
			}

			gl_start(RustStructs::primitive_mode::quads);
			Colorz(col.GetUnityColor());
			Vertex(Vector3(pos.x, pos.y, 0));
			Vertex(Vector3(pos.x + size.x, pos.y, 0));
			Colorz(col2.GetUnityColor());
			Vertex(Vector3(pos.x + size.x, pos.y + size.y, 0));
			Vertex(Vector3(pos.x, pos.y + size.y, 0));
			gl_end();
		}
	};


	struct CapsuleCollider : Object {
		IL2CPP_CLASS("CapsuleCollider");

		auto set_radius(float value) -> void;
	};

	struct Rigidbody : Il2CppObject {
		IL2CPP_CLASS("Rigidbody");

		void set_velocity(Vector3 vel);
	};


	struct Terrain_C : Il2CppObject {
		IL2CPP_CLASS("Terrain");

		float SampleHeight(Vector3 pos)
		{
			if (!this)
				return 0.f;

			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SampleHeight"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<float>(procedure, this, pos);
			}

			TRACE("%s: procedure not a valid!", FN);
			return 0.f;
		}
	};

	bool WorldToScreen(const Vector3& position, Vector2& output);
	bool WorldToScreen2(const Vector3& position, Vector3& output);
}


using UnityEngine::Vector2;
using UnityEngine::Vector3;
using UnityEngine::Vector4;

inline Vector2 screen_size = { 0.f,0.f };
inline Vector2 screen_center = { 0.f,0.f };
inline Vector3 spoofed_eye_pos = Vector3();

namespace UnityEngineNetworking {
	IL2CPP_NAME_SPACE(SECURE("UnityEngine.Networking"));

	struct DownloadHandlerBuffer : Il2CppObject {
		IL2CPP_CLASS("DownloadHandlerBuffer");

	};

	struct DownloadHandler : Il2CppObject {
		IL2CPP_CLASS("DownloadHandler");


		System::c_system_array<System::Byte*>* GetData()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("GetData"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<System::c_system_array<System::Byte*>*>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}

		DownloadHandler* NewInstance()
		{
			if (!this) return {};

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


				uintptr_t Texture2DClass = 0;
				if (!Texture2DClass)
				{
					Texture2DClass = (uintptr_t)CIl2Cpp::FindClass(SECURE("UnityEngine.Networking"), SECURE("DownloadHandler"));
				}
				DownloadHandler* texture2D = reinterpret_cast<DownloadHandler*>(CIl2Cpp::il2cpp_object_new(memory::ToPointer(Texture2DClass)));
				memory::Call<DownloadHandler*>(procedure, this);

				return texture2D;
			}
			else
			{
				TRACE("%s: FindMethod( ... ) error! Returned nullptr", FN);
				return {};
			}

		}
	};

	struct UnityWebRequest : Il2CppObject {
		IL2CPP_CLASS("UnityWebRequest");

		IL2CPP_FIELD(DownloadHandler*, m_DownloadHandler);


		bool get_isDone()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("get_isDone"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<bool>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return false;
		}

		void SendWebRequest()
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("SendWebRequest"), 0);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<void>(procedure, this);
			}

			TRACE("%s: procedure not a valid!", FN);
			return;
		}

		static UnityWebRequest* Get(const char* uri)
		{
			static uintptr_t procedure = 0;
			if (!memory::IsAddressValid(procedure))
			{
				const auto method = CIl2Cpp::FindMethod(StaticClass(), HASH("Get"), 1);
				if (memory::IsAddressValid(method))
				{
					procedure = memory::ToAddress(method->methodPointer);
				}
			}

			if (memory::IsAddressValid(procedure))
			{
				return memory::Call<UnityWebRequest*>(procedure, CIl2Cpp::il2cpp_string_new(uri));
			}

			TRACE("%s: procedure not a valid!", FN);
			return {};
		}
	};
}