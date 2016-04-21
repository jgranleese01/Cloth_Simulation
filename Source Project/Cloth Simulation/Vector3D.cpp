#include "Vector3D.h"

void Vector3D::Normalize()
{
	float length = GetLength();

	if (length == 1 || length == 0)
		return;

	float scalefactor = 1.0f / length;
	x *= scalefactor;
	y *= scalefactor;
	z *= scalefactor;
}

Vector3D Vector3D::GetNormalized() const
{
	Vector3D result(*this);

	result.Normalize();

	return result;
}

Vector3D Vector3D::GetRotatedX(double angle) const
{
	if (angle == 0.0)
		return (*this);

	float sinAngle = (float)sin(M_PI * angle / 180);
	float cosAngle = (float)cos(M_PI * angle / 180);

	return Vector3D(x,
		y*cosAngle - z*sinAngle,
		y*sinAngle + z*cosAngle);
}

void Vector3D::RotateX(double angle)
{
	(*this) = GetRotatedX(angle);
}

Vector3D Vector3D::GetRotatedY(double angle) const
{
	if (angle == 0.0)
		return (*this);

	float sinAngle = (float)sin(M_PI * angle / 180);
	float cosAngle = (float)cos(M_PI * angle / 180);

	return Vector3D(x*cosAngle + z*sinAngle,
		y,
		-x*sinAngle + z*cosAngle);
}

void Vector3D::RotateY(double angle)
{
	(*this) = GetRotatedY(angle);
}

Vector3D Vector3D::GetRotatedZ(double angle) const
{
	if (angle == 0.0)
		return (*this);

	float sinAngle = (float)sin(M_PI * angle / 180);
	float cosAngle = (float)cos(M_PI * angle / 180);

	return Vector3D(x*cosAngle - y*sinAngle,
		x*sinAngle + y*cosAngle,
		z);
}

void Vector3D::RotateZ(double angle)
{
	(*this) = GetRotatedZ(angle);
}

Vector3D Vector3D::GetRotatedAxis(double angle, const Vector3D & axis) const
{
	if (angle == 0.0)
		return (*this);

	Vector3D u = axis.GetNormalized();

	Vector3D rotMatrixRow0, rotMatrixRow1, rotMatrixRow2;

	float sinAngle = (float)sin(M_PI * angle / 180);
	float cosAngle = (float)cos(M_PI * angle / 180);
	float oneMinusCosAngle = 1.0f - cosAngle;

	rotMatrixRow0.x = (u.x)*(u.x) + cosAngle*(1 - (u.x)*(u.x));
	rotMatrixRow0.y = (u.x)*(u.y)*(oneMinusCosAngle)-sinAngle*u.z;
	rotMatrixRow0.z = (u.x)*(u.z)*(oneMinusCosAngle)+sinAngle*u.y;

	rotMatrixRow1.x = (u.x)*(u.y)*(oneMinusCosAngle)+sinAngle*u.z;
	rotMatrixRow1.y = (u.y)*(u.y) + cosAngle*(1 - (u.y)*(u.y));
	rotMatrixRow1.z = (u.y)*(u.z)*(oneMinusCosAngle)-sinAngle*u.x;

	rotMatrixRow2.x = (u.x)*(u.z)*(oneMinusCosAngle)-sinAngle*u.y;
	rotMatrixRow2.y = (u.y)*(u.z)*(oneMinusCosAngle)+sinAngle*u.x;
	rotMatrixRow2.z = (u.z)*(u.z) + cosAngle*(1 - (u.z)*(u.z));

	return Vector3D(this->DotProduct(rotMatrixRow0),
		this->DotProduct(rotMatrixRow1),
		this->DotProduct(rotMatrixRow2));
}

void Vector3D::RotateAxis(double angle, const Vector3D & axis)
{
	(*this) = GetRotatedAxis(angle, axis);
}


void Vector3D::PackTo01()
{
	(*this) = GetPackedTo01();
}

Vector3D Vector3D::GetPackedTo01() const
{
	Vector3D temp(*this);

	temp.Normalize();

	temp = temp*0.5f + Vector3D(0.5f, 0.5f, 0.5f);

	return temp;
}

Vector3D operator*(float scaleFactor, const Vector3D & rhs)
{
	return rhs*scaleFactor;
}

bool Vector3D::operator==(const Vector3D & rhs) const
{
	if (x == rhs.x && y == rhs.y && z == rhs.z)
		return true;

	return false;
}

