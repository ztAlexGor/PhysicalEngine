#include "Shape.h"

AABB Shape::GetAABB() const
{
	return AABB(aabb);
}

Shape::EType Shape::GetType() const
{
	return type;
}
