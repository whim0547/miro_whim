#ifndef MIRO_VECTOR3_H_INCLUDED
#define MIRO_VECTOR3_H_INCLUDED

#include <math.h>
#include <float.h>
#include <iostream>
#include <random>
#include "my_rand.h"

#ifdef _WIN32
	#pragma warning(disable:4305) // disable useless warnings
	#pragma warning(disable:4244)
#endif

class Vector3
{

public:
	float x, y, z; // The x & y & z coordinates.

	Vector3() : x(0), y(0), z(0) {}

	Vector3(float s) : x(s), y(s), z(s) {}

	Vector3(float xVal, float yVal, float zVal) : x(xVal), y(yVal), z(zVal) {}

	// Assignment operator.
	const Vector3 & operator=(const Vector3& a) {x = a.x; y = a.y; z = a.z; return *this;}

	// Assignment operator.
	const Vector3 & operator=(float a) {x = y = z = a; return *this;}

	void set(float a) {x = y = z = a;}
	void set(float a, float b, float c) {x = a; y = b; z = c;}
	void set(const Vector3 & v) {x = v.x; y = v.y; z = v.z;}

	// Access operator.
	/*
		Returns the ith component of the vector.
		\param i The component to return.
		\warning i must be either 0, 1, or 2 in order to get expected results.
	*/
	float & operator[](int i) {return (&x)[i];}

	// Constant access operator.
	/*
		Returns the ith component of a constant vector.
		\param i The component to return.
		\warning i must be either 0, 1, or 2 in order to get expected results.
	*/
	const float & operator[](int i) const {return (&x)[i];}


	// Component-wise vector addition operator.
	Vector3 operator+(const Vector3& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	// Component-wise vector addition-assignment operator.
	const Vector3 & operator+=(const Vector3& v)
	{
		x += v.x; y += v.y; z += v.z; return *this;
	}

	// Scalar addition-assignment operator.
	const Vector3 & operator+=(float a) {x += a; y += a; z += a; return *this;}


	// Component-wise vector subtraction operator.
	Vector3 operator-(const Vector3& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	// Component-wise vector subtraction-assignment operator.
	const Vector3 & operator-=(const Vector3& v)
	{
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	// Component-wise scalar subtraction assignment operator.
	const Vector3 & operator-=(float a) {x -= a; y -= a; z -= a; return *this;}


	// Scalar multiplication operator.
	Vector3 operator*(float a) const {return Vector3(x * a, y * a, z * a);}

	// Component-wise vector multiplication operator.
	Vector3 operator*(const Vector3& v) const
	{
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	// Scalar multiplication-assignment operator.
	const Vector3 & operator*=(float a) {x *= a; y *= a; z *= a; return *this;}

	// Component-wise vector multiplication-assignment operator.
	const Vector3 & operator*=(const Vector3& v)
	{
		x *= v.x; y *= v.y; z *= v.z; return *this;
	}

	// Negation operator.
	Vector3 operator-() const {return Vector3(-x, -y, -z);}
	const Vector3 & negate() {x = -x; y = -y; z = -z; return *this;}


	// Scalar division operator.
	Vector3 operator/(float a) const
	{
		float inv = float(1) / a;
		return Vector3(x * inv, y * inv, z * inv);
	}

	// Component-wise vector division operator.
	Vector3 operator/(const Vector3 & v) const
	{
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	// Scalar division-assignment operator.
	const Vector3 & operator/=(float a)
	{
		float inv = float(1) / a;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	// Component-wise vector division-assignment operator.
	const Vector3 & operator/=(const Vector3 & v)
	{
		x /= v.x; y /= v.y; z /= v.z; return *this;
	}


	// Vector equivalence operator.
	/*
		Tests to see if each component of \a v is equal to each component of
		this Vector3.
	*/
	bool operator==(const Vector3 & v) const
	{
		return(v.x == x && v.y == y && v.z == z);
	}

	// Vector difference operator.
	/*
		Tests to see if any component is different between the two Vec3s.
	*/
	bool operator!=(const Vector3 & v) const
	{
		return(v.x != x || v.y != y || v.z != z);
	}


	// Length<sup>2</sup>.
	/*
		Returns the geometric length<sup>2</sup> of the vector.
	*/
	float length2() const;

	// Length.
	/*
		Returns the geometric length of the vector.
	*/
	float length() const {return sqrtf(length2());}

	// Normalizes the vector and return its length.
	/*
		Scales each component of the vector in order to get unit
		length without changing direction.

		\return The length of the vector prior to normalization.
	*/
	float unitize()
	{
		float l = length();
		*this /= l;
		return l;
	}

	// Normalize a vector and return a reference to it.
	/*
		Scales each component of the vector in order to get unit
		length without changing direction.

		\return A reference to the vector.
	*/
	const Vector3 & normalize()
	{
		return (*this /= length());
	}

	// Return a normalized copy of the vector.
	Vector3 normalized() const
	{
		return( *this / length());
	}

	// Return a rotated copy of the vector
	Vector3 rotated(float theta, const Vector3 & w) const;

	// Rotate this vector about another vector, w, by theta radians.
	const Vector3 & rotate(float theta, const Vector3 & w)
	{
		return *this = rotated(theta, w);
	}
};


// Multiply a scalar by a Vec3.
inline Vector3
operator*(float s, const Vector3& v)
{
	return Vector3(v.x * s, v.y * s, v.z * s);
}


// The dot product of two Vec3s.
inline float
dot(const Vector3 & a, const Vector3 & b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


// The cross product of two Vec3s.
inline Vector3
cross(const Vector3 & a, const Vector3 & b)
{
	return Vector3(a.y * b.z - a.z * b.y,
	               a.z * b.x - a.x * b.z,
	               a.x * b.y - a.y * b.x);
}


inline float
Vector3::length2() const
{
	return dot(*this, *this);
}


// Return a rotated copy of the vector
inline Vector3
Vector3::rotated(float theta, const Vector3 & w) const
{
	float c = cosf(theta);
	float s = sinf(theta);

	Vector3 v0 = dot(*this, w) * w;
	Vector3 v1 = *this - v0;
	Vector3 v2 = cross(w, v1);

	return v0 + c * v1 + s * v2;
}


inline std::ostream &
operator<<(std::ostream& out, const Vector3& v)
{
	return out << v.x << " " << v.y << " " << v.z;
}

inline Vector3
generateRandomDir()
{
	std::random_device seed_gen;
  std::default_random_engine engine(seed_gen());
	std::uniform_real_distribution<> dist(0.0, 1.0);
	float theta = M_PI * dist(engine);
	float phi = 2.0f * M_PI * dist(engine);
	float x = sin(theta) * cos(phi);
	float y = sin(theta) * sin(phi);
	float z = cos(theta);
	return Vector3(x, y, z);
}

inline Vector3
generateRandomHemiDir(Vector3 dir) //dirを中心に半球面上方向のdirをcos(w_dir)に撮るペースに
{
	float phi = 2.0f * uniform_0_1() * M_PI;
	float sin2_theta = 1.0f * uniform_0_1();
	float sin_theta = sqrt(sin2_theta);
	Vector3 w = dir;
	Vector3 u = cross(fabs(dir.x) > 0.1f ? Vector3(0.0f, 1.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f), w).normalize();
	Vector3 v = cross(w, u).normalize();
	return (u * cos(phi) * sin_theta + v * sin(phi) * sin_theta + w * sqrt(1-sin2_theta)).normalize();
	//ORIGINAL
	// while(true) {
	// 	float theta = M_PI / 2.0 * dist(engine);
	// 	float phi = 2.0f * M_PI * dist(engine);
	// 	float x = sin(theta) * cos(phi);
	// 	float y = sin(theta) * sin(phi);
	// 	float z = cos(theta);
	// 	Vector3 ret = Vector3(x, y, z);
	// 	if(dot(ret, dir) >= 0.0f) return ret;
	// }
}

inline Vector3
generateRandomHemiDir(Vector3 dir, std::default_random_engine engine)//dirを中心に半球面上方向のdirを適当に撮る
{
	std::uniform_real_distribution<> dist(0.0, 1.0);
	float r1 = 2.0f * M_PI * dist(engine);
	float r2 = dist(engine);
	float r2s = sqrt(r2);
	Vector3 w = dir;
	Vector3 u = cross(fabs(dir.x) > 0.1f ? Vector3(0.0f, 1.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f), w).normalize();
	Vector3 v = cross(dir, u).normalize();
	return (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1-r2)).normalize();

	// while(true) {
	// 	float theta = M_PI / 2.0 * dist(engine);
	// 	float phi = 2.0f * M_PI * dist(engine);
	// 	float x = sin(theta) * cos(phi);
	// 	float y = sin(theta) * sin(phi);
	// 	float z = cos(theta);
	// 	Vector3 ret = Vector3(x, y, z);
	// 	if(dot(ret, dir) >= 0.0f) return ret;
	// }
}

inline Vector3
generateUniformRandomHemiDir(Vector3 dir) //dirを中心に半球面上方向のdirをcos(w_dir)に撮るペースに
{
	std::random_device seed_gen;
	std::default_random_engine engine(seed_gen());
	std::uniform_real_distribution<> dist(0.0, 1.0);
	//ORIGINAL
	while(true) {
		float theta = M_PI / 2.0 * dist(engine);
		float phi = 2.0f * M_PI * dist(engine);
		float x = sin(theta) * cos(phi);
		float y = sin(theta) * sin(phi);
		float z = cos(theta);
		Vector3 ret = Vector3(x, y, z);
		if(dot(ret, dir) >= 0.0f) return ret;
	}
}

inline Vector3
generateUniformRandomHemiDir(Vector3 dir, 	std::default_random_engine engine) //dirを中心に半球面上方向のdirを適当に撮る
{
	std::uniform_real_distribution<> dist(0.0, 1.0);
	while(true) {
		float theta = M_PI / 2.0 * dist(engine);
		float phi = 2.0f * M_PI * dist(engine);
		float x = sin(theta) * cos(phi);
		float y = sin(theta) * sin(phi);
		float z = cos(theta);
		Vector3 ret = Vector3(x, y, z);
		if(dot(ret, dir) >= 0.0f) return ret;
	}
}

inline float clamp(float v, float low, float high) {
	return v < low ? low : v > high ? high : v;
}

inline Vector3
clamp(const Vector3 v, float min, float max)
{
	return Vector3(clamp(v.x, min, max), clamp(v.y, min, max), clamp(v.z, min, max));
}

#endif // MIRO_VECTOR3_H_INCLUDED
