#include "AABB.h"

AABB::AABB(const AABB& aabb)
{
	min = aabb.min;
	max = aabb.max;
}

AABB& AABB::operator=(const AABB other)
{
	min = other.min;
	max = other.max;
	return *this;
}

//bool AABB::IsIntersect(const AABB other)
//{
//	if (leftBottom <)
//	return false;
//}
