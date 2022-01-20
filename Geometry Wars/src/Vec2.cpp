#include "Vec2.h"

Vec2::Vec2()
{
}

Vec2::Vec2(double xin, double yin)
	: x(xin),
	y(yin) 
{
}

bool Vec2::operator==(Vec2 lhs)
{
	return (x == lhs.x) && (y == lhs.y);
}

bool Vec2::operator!=(Vec2 lhs)
{
	return (x != lhs.x) && (y != lhs.y);
}

Vec2 Vec2::operator+(Vec2 lhs)
{
	return Vec2(x + lhs.x, y + lhs.y);
}

Vec2 Vec2::operator-(Vec2 lhs)
{
	return Vec2(x - lhs.x, y - lhs.y);
}

Vec2 Vec2::operator*(Vec2 lhs)
{
	return Vec2(x * lhs.x, y * lhs.y);
}

Vec2 Vec2::operator/(Vec2 lhs)
{
	return Vec2(x / lhs.x, y / lhs.y);
}

void Vec2::operator+=(const Vec2& lhs)
{
	x += lhs.x;
	y += lhs.y;
}

void Vec2::operator-=(const Vec2& lhs)
{
	x -= lhs.x;
	y -= lhs.y;

}

void Vec2::operator*=(const float lhs)
{
	x *= lhs;
	y *= lhs;
}

void Vec2::operator/=(const float lhs)
{
	x /= lhs;
	y /= lhs;
}

void Vec2::normalize()
{
	double L{ std::sqrt(x * x + y * y) };
	x = x / L;
	y = y / L;
}

double Vec2::lenght()
{
	return std::sqrt(x * x + y * y);
}