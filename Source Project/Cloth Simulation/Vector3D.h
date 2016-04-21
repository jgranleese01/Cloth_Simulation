#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <math.h>

#define M_PI 3.14159265358979323846

class Vector3D
{
public:
	Vector3D(void) : x(0.0f), y(0.0f), z(0.0f)
	{}

	Vector3D(float newX, float newY, float newZ) : x(newX), y(newY), z(newZ)
	{}

	Vector3D(const float * rhs) : x(*rhs), y(*(rhs + 1)), z(*(rhs + 2))
	{}

	Vector3D(const Vector3D & rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
	{}

	~Vector3D() {}

	void Set(float newX, float newY, float newZ)
	{
		x = newX;	y = newY;	z = newZ;
	}

	void SetX(float newX) { x = newX; }
	void SetY(float newY) { y = newY; }
	void SetZ(float newZ) { z = newZ; }

	float GetX() const { return x; }
	float GetY() const { return y; }
	float GetZ() const { return z; }

	void LoadZero(void)
	{
		x = y = z = 0.0f;
	}
	void LoadOne(void)
	{
		x = y = z = 1.0f;
	}

	Vector3D CrossProduct(const Vector3D & rhs) const
	{
		return Vector3D(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
	}

	float DotProduct(const Vector3D & rhs) const
	{
		return x*rhs.x + y*rhs.y + z*rhs.z;
	}

	void Normalize();
	Vector3D GetNormalized() const;

	float GetLength() const
	{
		return (float)sqrt((x*x) + (y*y) + (z*z));
	}

	float GetSquaredLength() const
	{
		return (x*x) + (y*y) + (z*z);
	}

	void RotateX(double angle);
	Vector3D GetRotatedX(double angle) const;
	void RotateY(double angle);
	Vector3D GetRotatedY(double angle) const;
	void RotateZ(double angle);
	Vector3D GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const Vector3D & axis);
	Vector3D GetRotatedAxis(double angle, const Vector3D & axis) const;

	void PackTo01();
	Vector3D GetPackedTo01() const;

	Vector3D lerp(const Vector3D & v2, float factor) const
	{
		return (*this)*(1.0f - factor) + v2*factor;
	}

	Vector3D QuadraticInterpolate(const Vector3D & v2, const Vector3D & v3, float factor) const
	{
		return (*this)*(1.0f - factor)*(1.0f - factor) + 2 * v2*factor*(1.0f - factor) + v3*factor*factor;
	}


	Vector3D operator+(const Vector3D & rhs) const
	{
		return Vector3D(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vector3D operator-(const Vector3D & rhs) const
	{
		return Vector3D(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Vector3D operator*(const float rhs) const
	{
		return Vector3D(x*rhs, y*rhs, z*rhs);
	}

	Vector3D operator/(const float rhs) const
	{
		return (rhs == 0.0f) ? Vector3D(0.0f, 0.0f, 0.0f) : Vector3D(x / rhs, y / rhs, z / rhs);
	}

	friend Vector3D operator*(float scaleFactor, const Vector3D & rhs);

	void Add(const Vector3D & v2, Vector3D & result)
	{
		result.x = x + v2.x;
		result.y = y + v2.y;
		result.z = z + v2.z;
	}

	void Subtract(const Vector3D & v2, Vector3D & result)
	{
		result.x = x - v2.x;
		result.y = y - v2.y;
		result.z = z - v2.z;
	}

	bool operator==(const Vector3D & rhs) const;
	bool operator!=(const Vector3D & rhs) const
	{
		return !((*this) == rhs);
	}

	void operator+=(const Vector3D & rhs)
	{
		x += rhs.x;	y += rhs.y;	z += rhs.z;
	}

	void operator-=(const Vector3D & rhs)
	{
		x -= rhs.x;	y -= rhs.y;	z -= rhs.z;
	}

	void operator*=(const float rhs)
	{
		x *= rhs;	y *= rhs;	z *= rhs;
	}

	void operator/=(const float rhs)
	{
		if (rhs == 0.0f)
			return;
		else
		{
			x /= rhs; y /= rhs; z /= rhs;
		}
	}


	Vector3D operator-(void) const { return Vector3D(-x, -y, -z); }
	Vector3D operator+(void) const { return *this; }

	operator float* () const { return (float*) this; }
	operator const float* () const { return (const float*) this; }

	float x;
	float y;
	float z;
};

#endif