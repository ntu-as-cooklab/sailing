#ifndef FLOAT2_HPP
#define FLOAT2_HPP

#include <math.h>
#define PI 3.14159265358979

template <class Derived>
struct Float2
{
	float x = 0, y = 0;

	Float2(float x, float y) : x(x), y(y) {}
	Float2(float c) : x(c), y(c) {}
	Float2() {}

	/* Addition */

	// compound assignment (does not need to be a member, but often is, to modify the private members)
	// return the result by reference
	Derived& operator+=(const Derived& rhs) { return x += rhs.x, y += rhs.y, *static_cast<Derived*>(this); }

	// friends defined inside class body are inline and are hidden from non-ADL lookup
	// passing lhs by value helps optimize chained a+b+c; otherwise, both parameters may be const references
	// reuse compound assignment; return the result by value (uses move constructor)
  	constexpr friend Derived operator+(const Derived& lhs, const Derived& rhs) { return Derived(lhs.x + rhs.x, lhs.y + rhs.y); }

	/* Subtraction */

	Derived& operator-=(const Derived& rhs) { return x -= rhs.x, y -= rhs.y, *static_cast<Derived*>(this); }
	constexpr friend Derived operator-(const Derived& lhs, const Derived& rhs) { return Derived(lhs.x - rhs.x, lhs.y - rhs.y); }

	/* Multiplication */

	Derived& operator*=(const float& rhs) { return x *= rhs, y *= rhs, *static_cast<Derived*>(this); }
	constexpr friend Derived operator*(const Derived& lhs, const float& rhs) { return Derived(lhs.x * rhs, lhs.y * rhs); }
	constexpr friend Derived operator*(const float& lhs, const Derived& rhs) { return Derived(lhs * rhs.x, lhs * rhs.y); }

	/* Division */

	Derived& operator/=(const float& rhs) { return x /= rhs, y /= rhs, *static_cast<Derived*>(this); }
	constexpr friend Derived operator/(const Derived& lhs, const float& rhs) { return Derived(lhs.x / rhs, lhs.y / rhs); }

	/* Comparison */

	constexpr friend bool operator==(const Derived& lhs, const Derived& rhs){ return lhs.x == rhs.x && lhs.y == rhs.y; }
	constexpr friend bool operator!=(const Derived& lhs, const Derived& rhs){ return !(lhs == rhs); }

	/* Dot product */

	constexpr friend float dot(const Derived& lhs, const Derived& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
	constexpr friend float operator&(const Derived& lhs, const Derived& rhs) { return dot(lhs, rhs); }

	/* Norm */

	constexpr float norm() const { return sqrt(x*x + y*y); }
	constexpr friend float norm(const Derived& v) { return v.norm(); }

	/* Normalization */

	Derived& normalize() { return *static_cast<Derived*>(&(*this/=norm())); }
	constexpr friend Derived normalize(Derived v) { return v.normalize(); }
};

#endif
