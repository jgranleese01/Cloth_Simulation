#ifndef Colour_H
#define Colour_H

class Colour
{
public:
	Colour()
	{
		r = g = b = a = 0.0f;
	}

	Colour(float newR, float newG, float newB, float newA = 0.0f)
	{
		r = newR;	g = newG;	b = newB;	a = newA;
	}

	Colour(const float * rhs)
	{
		r = *rhs;	g = *(rhs + 1);	b = *(rhs + 2); a = *(rhs + 3);
	}

	Colour(const Colour & rhs)
	{
		r = rhs.r;	g = rhs.g;	b = rhs.b;	a = rhs.a;
	}

	~Colour() {}

	void Set(float newR, float newG, float newB, float newA = 0.0f)
	{
		r = newR;	g = newG;	b = newB;	a = newA;
	}

	void SetR(float newR) { r = newR; }
	void SetG(float newG) { g = newG; }
	void SetB(float newB) { b = newB; }
	void SetA(float newA) { a = newA; }

	float GetR() const { return r; }
	float GetG() const { return g; }
	float GetB() const { return b; }
	float GetA() const { return a; }

	void ClampTo01(void);

	void SetBlack(void) { r = g = b = a = 1.0f; }
	void SetWhite(void) { r = g = b = a = 0.0f; }
	void SetGrey(float shade) { r = g = b = a = shade; }

	Colour lerp(const Colour & c2, float factor)
	{
		return (*this)*(1.0f - factor) + c2*factor;
	}

	Colour operator+(const Colour & rhs) const
	{
		return Colour(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a);
	}

	Colour operator-(const Colour & rhs) const
	{
		return Colour(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a);
	}

	Colour operator*(const Colour & rhs) const
	{
		return Colour(r*rhs.r, g*rhs.g, b*rhs.b, a*rhs.a);
	}

	Colour operator/(const Colour & rhs) const
	{
		return Colour(r / rhs.r, g / rhs.g, b / rhs.b, a / rhs.a);
	}

	Colour operator*(const float rhs) const
	{
		return Colour(r*rhs, g*rhs, b*rhs, a*rhs);
	}

	Colour operator/(const float rhs) const
	{
		return Colour(r / rhs, g / rhs, b / rhs, a / rhs);
	}

	friend Colour operator*(float scaleFactor, const Colour & rhs);

	bool operator==(const Colour & rhs) const;
	bool operator!=(const Colour & rhs) const
	{
		return !((*this) == rhs);
	}

	Colour operator+=(const Colour & rhs)
	{
		(*this) = (*this) + rhs;	return (*this);
	}

	Colour operator-=(const Colour & rhs)
	{
		(*this) = (*this) - rhs;	return (*this);
	}

	Colour operator*=(const Colour & rhs)
	{
		(*this) = (*this)*rhs;	return (*this);
	}

	Colour operator/=(const Colour & rhs)
	{
		(*this) = (*this) / rhs;	return (*this);
	}

	Colour operator*=(const float rhs)
	{
		(*this) = (*this)*rhs;	return (*this);
	}

	Colour operator/=(const float rhs)
	{
		(*this) = (*this) / rhs;	return (*this);
	}

	Colour operator-(void) const { return Colour(-r, -g, -b, -a); }
	Colour operator+(void) const { return (*this); }

	operator float* () const { return (float*) this; }
	operator const float* () const { return (const float*) this; }

	float r;
	float g;
	float b;
	float a;
};

const Colour white(1.0f, 1.0f, 1.0f, 1.0f);
const Colour black(0.0f, 0.0f, 0.0f, 0.0f);

const Colour red(1.0f, 0.0f, 0.0f, 1.0f);
const Colour green(0.0f, 1.0f, 0.0f, 1.0f);
const Colour blue(0.0f, 0.0f, 1.0f, 1.0f);

#endif