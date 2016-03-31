#include <float2.hpp>
#include <iostream>

struct vec2: public Float2<vec2>
{
	vec2() {}
	vec2(float x0, float y0) { x = x0; y = y0; }
};

int main()
{
	vec2 x(1,2), y(3,4);
	std::cout << x << "\n";
	std::cout << y << "\n";
	std::cout << x+y << "\n";
	std::cout << (x+=y) << "\n";
	return 0;
}
