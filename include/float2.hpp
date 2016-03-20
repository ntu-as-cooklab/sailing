#ifndef FLOAT2_HPP
#define FLOAT2_HPP

#include <math.h>
#define PI 3.14159265358979

template<class Derived>
struct Float2
{
	float x = 0, y = 0;

	Float2(float x, float y) : x(x), y(y) {}
	Float2(float c) : x(c), y(c) {}
	Float2() {}

	/* Addition */

	// compound assignment (does not need to be a member, but often is, to modify the private members)
	// return the result by reference
	Derived& operator+=(const Derived& rhs)
  	{
    	x += rhs.x; y += rhs.y;
    	return *static_cast<Derived*>(this);
  	}

	// friends defined inside class body are inline and are hidden from non-ADL lookup
	// passing lhs by value helps optimize chained a+b+c; otherwise, both parameters may be const references
	// reuse compound assignment; return the result by value (uses move constructor)
  	friend Derived operator+(Derived lhs, const Derived& rhs) { return lhs += rhs; }

	/* Subtraction */

	Derived& operator-=(const Derived& rhs)
	{
		x -= rhs.x; y -= rhs.y;
		return *static_cast<Derived*>(this);
	}
	friend Derived operator-(Derived lhs, const Derived& rhs) { return lhs -= rhs; }

	/* Multiplication */

	Derived& operator*=(const float& rhs)
	{
		x *= rhs; y *= rhs;
		return *static_cast<Derived*>(this);
	}
	friend Derived operator*(Derived lhs, const float& rhs) { return lhs *= rhs; }
	friend Derived operator*(const float& lhs, Derived rhs) { return rhs *= lhs; }

	/* Division */

	Derived& operator/=(const float& rhs)
	{
		x /= rhs; y /= rhs;
		return *static_cast<Derived*>(this);
	}
	friend Derived operator/(Derived lhs, const float& rhs) { return lhs /= rhs; }

	/* Comparison */

	friend bool operator==(const Derived& lhs, const Derived& rhs){ return lhs.x == rhs.x && lhs.y == rhs.y; }
	friend bool operator!=(const Derived& lhs, const Derived& rhs){ return !(lhs == rhs); }

	/* Dot product */

	friend float dot(const Derived& lhs, const Derived& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }

	/* Norm */

	float norm() const { return sqrt(x*x + y*y); }
	friend float norm(const Derived& v) { return v.norm(); }

	/* Normalization */

	Derived& normalize()
	{
		x /= fabs(x); y /= fabs(y);
		return *static_cast<Derived*>(this);
	}
	friend Derived normalize(Derived v) { return v.normalize(); }
};

#endif
