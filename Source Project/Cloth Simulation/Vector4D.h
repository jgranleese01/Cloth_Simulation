#ifndef Vector4D_H
#define Vector4D_H

#include "Vector3D.h"

class Vector4D
{
public:
	Vector4D(void) : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
	{}

	Vector4D(float newX, float newY, float newZ, float newW)
		: x(newX), y(newY), z(newZ), w(newW)
	{}

	Vector4D(const float * rhs) : x(*rhs), y(*(rhs + 1)), z(*(rhs + 2)), w(*(rhs + 3))
	{}

	Vector4D(const Vector4D & rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
	{}

	Vector4D(const Vector3D & rhs) : x(rhs.x), y(rhs.y), z(rhs.z), w(1.0f)
	{}

	~Vector4D() {}

	void Set(float newX, float newY, float newZ, float newW)
	{
		x = newX;	y = newY;	z = newZ; w = newW;
	}

	void SetX(float newX) { x = newX; }
	void SetY(float newY) { y = newY; }
	void SetZ(float newZ) { z = newZ; }
	void SetW(float newW) { w = newW; }

	float GetX() const { return x; }
	float GetY() const { return y; }
	float GetZ() const { return z; }
	float GetW() const { return w; }

	void LoadZero(void)
	{
		x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f;
	}

	void LoadOne(void)
	{
		x = 1.0f; y = 1.0f; z = 1.0f; w = 1.0f;
	}

	float DotProduct(const Vector4D & rhs)
	{
		return x*rhs.x + y*rhs.y + z*rhs.z + w*rhs.w;
	}

	void RotateX(double angle);
	Vector4D GetRotatedX(double angle) const;
	void RotateY(double angle);
	Vector4D GetRotatedY(double angle) const;
	void RotateZ(double angle);
	Vector4D GetRotatedZ(double angle) const;
	void RotateAxis(double angle, const Vector3D & axis);
	Vector4D GetRotatedAxis(double angle, const Vector3D & axis) const;

	Vector4D lerp(const Vector4D & v2, float factor) const
	{
		return (*this)*(1.0f - factor) + v2*factor;
	}

	Vector4D QuadraticInterpolate(const Vector4D & v2, const Vector4D & v3, float factor) const
	{
		return (*this)*(1.0f - factor)*(1.0f - factor) + 2 * v2*factor*(1.0f - factor) + v3*factor*factor;
	}

	Vector4D operator+(const Vector4D & rhs) const
	{
		return Vector4D(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	Vector4D operator-(const Vector4D & rhs) const
	{
		return Vector4D(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	Vector4D operator*(const float rhs) const
	{
		return Vector4D(x*rhs, y*rhs, z*rhs, w*rhs);
	}

	Vector4D operator/(const float rhs) const
	{
		return rhs == 0.0f ? Vector4D(0.0f, 0.0f, 0.0f, 0.0f)
			: Vector4D(x / rhs, y / rhs, z / rhs, w / rhs);
	}

	friend Vector4D operator*(float scaleFactor, const Vector4D & rhs);

	bool operator==(const Vector4D & rhs) const;
	bool operator!=(const Vector4D & rhs) const
	{
		return !((*this) == rhs);
	}

	void operator+=(const Vector4D & rhs)
	{
		x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w;
	}

	void operator-=(const Vector4D & rhs)
	{
		x -= rhs.x; y -= rhs.y; z -= rhs.z; w -= rhs.w;
	}

	void operator*=(const float rhs)
	{
		x *= rhs; y *= rhs; z *= rhs; w *= rhs;
	}

	void operator/=(const float rhs)
	{
		if (rhs == 0.0f)
			return;
		else
		{
			x /= rhs; y /= rhs; z /= rhs; w /= rhs;
		}
	}

	Vector4D operator-(void) const { return Vector4D(-x, -y, -z, -w); }
	Vector4D operator+(void) const { return (*this); }

	operator float* () const { return (float*) this; }
	operator const float* () const { return (const float*) this; }

	operator Vector3D();

	float x;
	float y;
	float z;
	float w;
};

#endif