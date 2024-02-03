#include "math.hpp"

#include "../core/map_data.hpp"

#include <xmmintrin.h>

#include <cassert>

#define GET_FLOAT_WORD( i, d )			\
	do																\
	{																	\
		ieee_float_shape_type gf_u;			\
		gf_u.value = ( d );							\
		( i ) = gf_u.word;							\
	}																	\
	while( 0 )

#define SET_FLOAT_WORD( d, i )			\
	do																\
	{																	\
		ieee_float_shape_type sf_u;			\
		sf_u.word = ( i );							\
		( d ) = sf_u.value;							\
	}																	\
	while( 0 )

namespace horizon::win32
{

// 
// constants
// 

constexpr float XM_PI = 3.141592654f;
constexpr float XM_2PI = 6.283185307f;
constexpr float XM_1DIVPI = 0.318309886f;
constexpr float XM_1DIV2PI = 0.159154943f;
constexpr float XM_PIDIV2 = 1.570796327f;
constexpr float XM_PIDIV4 = 0.785398163f;
constexpr double PI = 3.14159265358979323846;
constexpr double TWO_PI = 2.0 * PI;
// 
// imports
// 

double pow( double x, double y )
{
	if (y == 0) {
		return 1.0;
	}

	if (y < 0) {
		// For negative exponents, calculate the reciprocal of the result
		x = 1.0 / x;
		y = -y;
	}

	double result = 1.0;
	while (y > 0) {
		if ((int)y % 2 == 1) {
			// If the current exponent is odd, multiply by the base (x)
			result *= x;
		}
		// Square the base (x) and halve the exponent (y) for the next iteration
		x *= x;
		y /= 2;
	}

	return result;
}

float powf( float x, float y )
{
	return static_cast< float >( pow( static_cast< double >( x ), static_cast< double >( y ) ) );
}

double fmod( double x, double y )
{
	if (y == 0.0) {
		// Handle division by zero (or very close to zero)
		return 0.0; // You can choose an appropriate return value here
	}

	// Calculate the integer quotient
	double quotient = x / y;

	// Calculate the truncated quotient (integer part)
	double truncatedQuotient = static_cast<long long>(quotient); // Casting to long long for floor-like behavior

	// Calculate the floating-point remainder
	double remainder = x - truncatedQuotient * y;

	// Handle the case when the remainder might be negative
	if ((y < 0.0 && remainder > 0.0) || (y > 0.0 && remainder < 0.0))
		remainder += y;

	return remainder;
}

float fmodf( float x, float y )
{
	return static_cast< float >( fmod( static_cast< double >( x ), static_cast< double >( y ) ) );
}

double atof( const char* data )
{
	double result = 0.0;
	int sign = 1;
	int decimalPlace = 0;
	bool hasDecimal = false;

	// Handle leading whitespaces
	while (*data == ' ')
		data++;

	// Check for a sign character (+/-)
	if (*data == '-' || *data == '+') {
		sign = (*data == '-') ? -1 : 1;
		data++;
	}

	// Convert the integer part of the number
	while (*data != '\0' && (*data >= '0' && *data <= '9')) {
		result = result * 10.0 + (*data - '0');
		data++;
	}

	// Convert the decimal part of the number
	if (*data == '.') {
		hasDecimal = true;
		data++;

		while (*data != '\0' && (*data >= '0' && *data <= '9')) {
			result = result * 10.0 + (*data - '0');
			decimalPlace++;
			data++;
		}
	}

	// Handle any remaining characters
	while (*data != '\0' && (*data == ' ' || (*data >= '\t' && *data <= '\r')))
		data++;

	// Adjust the result based on the decimal place
	if (hasDecimal)
		result /= pow(10.0, decimalPlace);

	// Apply the sign
	return result * sign;
}

double floor( double x )
{
	int integerPart = static_cast<int>(x); // Cast to int truncates the decimal part

	// If the number is already an integer or positive, return the integer part
	if (x >= 0 || x == static_cast<float>(integerPart)) {
		return integerPart;
	}

	// For negative numbers, subtract 1 from the integer part to get the floor value
	return integerPart - 1;
}

float floorf( float x )
{
	return static_cast< float >( floor( static_cast< double >( x ) ) );
}

double ceil( double x )
{
	double integerPart = floor(x);
	if (x == integerPart) {
		return x;
	}
	return integerPart + 1.0;
}

float ceilf( float x )
{
	return static_cast< float >( ceil( static_cast< double >( x ) ) );
}

double myAtan(double x) {
	if (x == 0.0) {
		return 0.0;
	}

	double result = 0.0;
	bool isNegative = false;

	// Handle negative values to calculate arctan in the appropriate quadrant
	if (x < 0.0) {
		x = -x;
		isNegative = true;
	}

	if (x > 1.0) {
		// Use arctan formula for values greater than 1 to bring the value within (-PI/2, PI/2)
		result = PI / 2 - myAtan(1 / x);
	}
	else {
		// Use Taylor series approximation for values between -1 and 1
		double term = x;
		double denominator = 1.0;
		double nextTerm;

		for (int i = 1; i <= 1000; i++) { // Limit the number of iterations for accuracy
			denominator += 2.0;
			nextTerm = term * (x * x) / denominator;
			if (i % 2 == 0) {
				result -= nextTerm;
			}
			else {
				result += nextTerm;
			}
			term = nextTerm;
			if (nextTerm == 0.0) {
				break; // Exit loop if the term becomes zero (no significant contribution)
			}
		}
	}

	// Adjust the result based on the quadrant
	if (isNegative) {
		result = -result;
	}

	return result;
}

double atan2( double y, double x )
{
	if (x == 0.0) {
		if (y > 0.0) return PI / 2.0;
		if (y < 0.0) return -PI / 2.0;
		return 0.0; // atan2(0, 0) is undefined, this is just an example implementation.
	}

	double angle = myAtan(y / x);
	if (x < 0.0) {
		angle += PI;
	}
	else if (y < 0.0) {
		angle += TWO_PI;
	}

	return angle;
}

// 
// implementation
// 

union ieee_float_shape_type
{
	float value;
	std::uint32_t word;
};

float roundf( float x )
{
	std::uint32_t w;
	GET_FLOAT_WORD( w, x );

	const auto exponent_less_127 = static_cast< std::int32_t >( ( w & 0x7f800000 ) >> 23 ) - 127;

	if( exponent_less_127 < 23 )
	{
		if( exponent_less_127 < 0 )
		{
			w &= 0x80000000;

			if( exponent_less_127 == -1 )
			{
				w |= ( static_cast< std::uint32_t >( 127 ) << 23 );
			}
		}
		else
		{
			const auto exponent_mask = 0x007fffffu >> exponent_less_127;

			if( ( w & exponent_mask ) == 0 )
			{
				return x;
			}

			w += 0x00400000 >> exponent_less_127;
			w &= ~exponent_mask;
		}
	}
	else
	{
		if( exponent_less_127 == 128 )
		{
			return x + x;
		}
		else
		{
			return x;
		}
	}

	SET_FLOAT_WORD( x, w );
	return x;
}

float fabsf( float x )
{
	return ( x < 0.f ? -x : x );
}

float sqrtf( float x )
{
	auto root = _mm_set_ss( x );
	root = _mm_sqrt_ss( root );
	return _mm_cvtss_f32( root );
}

float sinf( float x )
{
	auto quotient = XM_1DIV2PI * x;

	if( x >= 0.f )
	{
		quotient = static_cast< float >( static_cast< int >( quotient + 0.5f ) );
	}
	else
	{
		quotient = static_cast< float >( static_cast< int >( quotient - 0.5f ) );
	}

	auto y = x - XM_2PI * quotient;
	auto sign = 0.f;

	if( y > XM_PIDIV2 )
	{
		y = XM_PI - y;
		sign = -1.f;
	}
	else if( y < -XM_PIDIV2 )
	{
		y = -XM_PI - y;
		sign = -1.f;
	}
	else
	{
		sign = +1.f;
	}

	auto y2 = y * y;
	return ( ( ( ( ( ( -2.3889859e-08f * y2 + 2.7525562e-06f ) * y2 - 0.00019840874f ) * y2 + 0.0083333310f ) * y2 - 0.16666667f ) * y2 + 1.f ) * y );
}

float cosf( float x )
{
	auto quotient = XM_1DIV2PI * x;

	if( x >= 0.f )
	{
		quotient = static_cast< float >( static_cast< int >( quotient + 0.5f ) );
	}
	else
	{
		quotient = static_cast< float >( static_cast< int >( quotient - 0.5f ) );
	}

	auto y = x - XM_2PI * quotient;
	auto sign = 0.f;

	if( y > XM_PIDIV2 )
	{
		y = XM_PI - y;
		sign = -1.f;
	}
	else if( y < -XM_PIDIV2 )
	{
		y = -XM_PI - y;
		sign = -1.f;
	}
	else
	{
		sign = +1.f;
	}

	auto y2 = y * y;
	auto p = ( ( ( ( ( -2.6051615e-07f * y2 + 2.4760495e-05f ) * y2 - 0.0013888378f ) * y2 + 0.041666638f ) * y2 - 0.5f ) * y2 + 1.f );

	return ( sign * p );
}

float tanf( float x )
{
	auto s = sinf( x );
	auto c = cosf( x );

	return ( s / c );
}

float asinf( float x )
{
	auto non_negative = ( x >= 0.f );

	auto pox = fabsf( x );
	auto omx = 1.f - pox;

	if( omx < 0.f )
	{
		omx = 0.f;
	}

	auto root = sqrtf( omx );
	auto result = ( ( ( ( ( ( ( -0.0012624911f * pox + 0.0066700901f ) * pox - 0.0170881256f ) * pox + 0.0308918810f ) * pox - 0.0501743046f ) * pox + 0.0889789874f ) * pox - 0.2145988016f ) * pox + 1.5707963050f );

	result *= root;
	return ( non_negative ? ( XM_PIDIV2 - result ) : ( result - XM_PIDIV2 ) );
}

float acosf( float x )
{
	auto non_negative = ( x >= 0.f );

	auto pox = fabsf( x );
	auto omx = 1.f - pox;

	if( omx < 0.f )
	{
		omx = 0.f;
	}

	auto root = sqrtf( omx );
	auto result = ( ( ( ( ( ( ( -0.0012624911f * pox + 0.0066700901f ) * pox - 0.0170881256f ) * pox + 0.0308918810f ) * pox - 0.0501743046f ) * pox + 0.0889789874f ) * pox - 0.2145988016f ) * pox + 1.5707963050f );

	result *= root;
	return ( non_negative ? result : ( XM_PI - result ) );
}

float atanf( float x )
{
	auto x2 = x * x;
	auto root = sqrtf( 1.f + x2 );
	return asinf( x / root );
}

float atan2f( float y, float x )
{
	return static_cast< float >( atan2( static_cast< double >( y ), static_cast< double >( x ) ) );
}

long atol( const char* const data )
{
	long result = 0;
	int sign = 1;

	// Handle leading whitespaces
	const char* str = data;
	while (*str == ' ')
		str++;

	// Check for a sign character (+/-)
	if (*str == '-' || *str == '+') {
		sign = (*str == '-') ? -1 : 1;
		str++;
	}

	// Convert the number
	while (*str != '\0' && (*str >= '0' && *str <= '9')) {
		result = result * 10 + (*str - '0');
		str++;
	}

	// Apply the sign
	return result * sign;
}

int atoi( const char* const data )
{
	return static_cast< int >( atol( data ) );
}

} // namespace horizon::win32