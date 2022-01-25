#pragma once

#include "Common.h"

class Vec2
{
public:
	double x{ 0 };
	double y{ 0 };

	Vec2();
	Vec2(double xin, double yin);

	friend std::ostream& operator << (std::ostream& stream, const Vec2& lhs);

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
	static Vec2 normalize(Vec2 vector);
	double lenght();
};

