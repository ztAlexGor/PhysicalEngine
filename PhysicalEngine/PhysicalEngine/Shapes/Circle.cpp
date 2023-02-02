#include "Circle.h"

Circle::Circle(float radius) : Shape(EType::circle), radius(radius)
{
	InitAABB();
}


float Circle::GetRadius() const
{
	return radius;
}


void Circle::SetRadius(float radius)
{
	this->radius = radius;
}


void Circle::InitAABB()
{
	aabb = AABB(Vector(-radius, -radius), Vector(radius, radius));
}


MassInfo Circle::ComputeMass(float density) const
{
	float mass = PI * radius * radius * density;
	float inertia = mass * radius * radius / 2.f;

	MassInfo massInfo{ .mass{mass}, .invMass{1.f / mass}, .inertia{inertia}, .invInertia{1.f / inertia} };
	return massInfo;
}


Shape* Circle::Clone() const
{
	return new Circle(*this);
}
