#include "Circle.h"

Circle::Circle(const Circle& other): Shape(EType::circle)
{
	radius = other.radius;
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

MassInfo Circle::ComputeMass(float density) const
{
	float mass = PI * radius * radius * density;
	float inertia = mass * radius * radius;

	MassInfo massInfo{ .mass{mass}, .invMass{1.f / mass}, .inertia{inertia}, .invInertia{1.f / inertia} };
	return massInfo;
}


Shape* Circle::Clone() const
{
	return new Circle(*this);
}
