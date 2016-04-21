#include "Colour.h"

void Colour::ClampTo01()
{
	if (r>1.0f)
		r = 1.0f;
	if (r<0.0f)
		r = 0.0f;

	if (g>1.0f)
		g = 1.0f;
	if (g<0.0f)
		g = 0.0f;

	if (b>1.0f)
		b = 1.0f;
	if (b<0.0f)
		b = 0.0f;

	if (a>1.0f)
		a = 1.0f;
	if (a<0.0f)
		a = 0.0f;
}

Colour operator*(float scaleFactor, const Colour & rhs)
{
	return rhs*scaleFactor;
}

bool Colour::operator ==(const Colour & rhs) const
{
	if (r != rhs.r)
		return false;
	if (g != rhs.g)
		return false;
	if (b != rhs.b)
		return false;
	if (a != rhs.a)
		return false;

	return true;
}
