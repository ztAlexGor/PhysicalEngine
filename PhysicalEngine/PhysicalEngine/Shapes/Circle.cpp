#include "Circle.h"

Circle::Circle(Circle& other): Shape(EType::circle)
{
	this->radius = other.radius;
}

float Circle::GetRadius() const
{
	return radius;
}

void Circle::SetRadius(float radius)
{
	this->radius = radius;
}

AABB Circle::ComputeAABB() const
{
	return AABB(Vector(-radius, -radius), Vector(radius, radius));
}

float Circle::ComputeMass() const
{
	return PI * radius * radius;
}
