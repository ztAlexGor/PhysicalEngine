#pragma once
#include "../MathUtils/Vector.h"

struct AABB
{
	Vector min;
	Vector max;

	AABB(Vector leftBottom, Vector rightTop): min(leftBottom), max(rightTop) {};

	AABB(const AABB& other);

	AABB& operator=(const AABB other);

	//bool IsIntersect(const AABB other);
};

