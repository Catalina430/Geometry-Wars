#pragma once

#include "Common.h"

class CTransform
{
public:
	Vec2 pos{ 0.0,0.0 };
	Vec2 velocity{ 0.0,0.0 };
	float angle{ 0 };

	CTransform(const Vec2& p, const Vec2& s,float a)
		: pos(p),
		velocity(s),
		angle(a) {}
};

