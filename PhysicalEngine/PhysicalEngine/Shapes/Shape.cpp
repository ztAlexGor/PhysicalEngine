#include "Shape.h"

Shape::Shape(EType type) : type(type) {}

AABB Shape::GetAABB() const
{
	return AABB(aabb);
}

Shape::EType Shape::GetType() const
{
	return type;
}

Matrix Shape::GetMatrix() const
{
	return matrix;
}
