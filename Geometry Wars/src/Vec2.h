#pragma once

#include "Common.h"

class Vec2
{
public:
	double x{ 0 };
	double y{ 0 };

	Vec2();
	Vec2(double xin, double yin);

	bool operator == (Vec2 lhs);
	bool operator != (Vec2 lhs);

	Vec2 operator +  (Vec2 lhs);
	Vec2 operator -  (Vec2 lhs);
	Vec2 operator *  (Vec2 lhs);
	Vec2 operator /  (Vec2 lhs);

	void operator +=  (const Vec2& lhs);
	void operator -=  (const Vec2& lhs);
	void operator *=  (const float lhs);
	void operator /=  (const float lhs);

	void normalize();
	double lenght();
};

