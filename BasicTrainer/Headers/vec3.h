#pragma once
#include <math.h>

class vec3
{
public:

	union
	{
		struct { float x, y, z; };
		float v[3];
	};

	vec3() { x = y = z = 0; }
	vec3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	vec3 operator+(vec3 d)const { return { x + d.x, y + d.y, z + d.z }; }
	vec3 operator-(vec3 d)const { return { x - d.x, y - d.y, z - d.z }; }
	vec3 operator / (const float& rhs) const { return vec3(x / rhs, y / rhs, z / rhs); }
	vec3 operator*(float d)const { return { x * d, y * d, z * d }; }
	vec3& operator + (vec3 rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	vec3& operator += (const vec3& rhs) { return *this = *this + rhs; }
	vec3& operator -= (const vec3& rhs) { return *this = *this - rhs; }
	vec3& operator *= (const float& rhs) { return *this = *this * rhs; }
	vec3& operator /= (const float& rhs) { return *this = *this / rhs; }
	void NormalizeAngles() { while (y < -180) { y += 360; }while (y > 180) { y -= 360; }if (x > 89) { x = 89; }if (x < -89) { x = -89; } }
};
