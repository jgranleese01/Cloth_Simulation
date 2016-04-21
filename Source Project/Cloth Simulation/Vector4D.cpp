#include "Vector4D.h"

void Vector4D::RotateX(double angle)
{
	(*this) = GetRotatedX(angle);
}

Vector4D Vector4D::GetRotatedX(double angle) const
{
	Vector3D v3d(x, y, z);

	v3d.RotateX(angle);

	return Vector4D(v3d.x, v3d.y, v3d.z, w);
}

void Vector4D::RotateY(double angle)
{
	(*this) = GetRotatedY(angle);
}

Vector4D Vector4D::GetRotatedY(double angle) const
{
	Vector3D v3d(x, y, z);

	v3d.RotateY(angle);

	return Vector4D(v3d.x, v3d.y, v3d.z, w);
}

void Vector4D::RotateZ(double angle)
{
	(*this) = GetRotatedZ(angle);
}

Vector4D Vector4D::GetRotatedZ(double angle) const
{
	Vector3D v3d(x, y, z);

	v3d.RotateZ(angle);

	return Vector4D(v3d.x, v3d.y, v3d.z, w);
}

void Vector4D::RotateAxis(double angle, const Vector3D & axis)
{
	(*this) = GetRotatedAxis(angle, axis);
}

Vector4D Vector4D::GetRotatedAxis(double angle, const Vector3D & axis) const
{
	Vector3D v3d(x, y, z);

	v3d.RotateAxis(angle, axis);

	return Vector4D(v3d.x, v3d.y, v3d.z, w);
}


Vector4D operator*(float scaleFactor, const Vector4D & rhs)
{
	return rhs*scaleFactor;
}

bool Vector4D::operator==(const Vector4D & rhs) const
{
	if (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w)
		return true;

	return false;
}

Vector4D::operator Vector3D()
{
	if (w == 0.0f || w == 1.0f)
		return Vector3D(x, y, z);
	else
		return Vector3D(x / w, y / w, z / w);
}
