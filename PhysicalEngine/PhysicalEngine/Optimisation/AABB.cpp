#include "AABB.h"

AABB::AABB(const AABB& aabb)
{
	this->min = aabb.min;
	this->max = aabb.max;
}

AABB& AABB::operator=(const AABB aabb)
{
	this->min = aabb.min;
	this->max = aabb.max;
	return *this;
}

//bool AABB::IsIntersect(const AABB other)
//{
//	if (leftBottom <)
//	return false;
//}
