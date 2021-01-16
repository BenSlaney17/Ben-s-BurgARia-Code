#pragma once
#include <math.h>

static class Maths
{
public:
	static inline float Lerp(float a, float b, float t)
	{
		return (1.0f - t) * a + b * t;
	}

	static const float PI(){ 3.14159265359f;}
	static const float TWO_PI() { return 6.28318530718f; }

};

