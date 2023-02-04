#include "Body.h"

Body::Body(const BodyInit& init)
{
	position = init.position;
	velocity = init.velocity;
	angle = init.angle;
	angularVelocity = init.angularVelocity;
	bIsStatic = init.bIsStatic;
	gravityScale = init.gravityScale;
	adittionalData = init.adittionalData;

	for (Vector f : init.forces)
	{
		forces.emplace_back(f);
	}
}

void Body::SetShape(Shape* newShape, MaterialInfo shapeMaterial, float density)
{
	shape = newShape->Clone();
	material = shapeMaterial;
	massInfo = shape->ComputeMass(density);
}

const Shape* Body::GetShape() const
{
	return shape;
}

const MassInfo& Body::GetMassInfo() const
{
	return massInfo;
}

const MaterialInfo& Body::GetMaterial() const
{
	return material;
}

Vector Body::GetPosition() const
{
	return position;
}

Vector Body::GetVelocity() const
{
	return velocity;
}

float Body::GetAngle() const
{
	return angle;
}

float Body::GetAngularVelocity() const
{
	return angularVelocity;
}

bool Body::IsStatic() const
{
	return bIsStatic;
}

float Body::GetGravityScale() const
{
	return gravityScale;
}

const std::vector<Vector>& Body::GetForces() const
{
	return forces;
}

std::string Body::GetAdittionalData() const
{
	return adittionalData;
}

Shape::EType Body::GetShapeType() const
{
	return shape->GetType();
}

void Body::SetMaterial(MaterialInfo material)
{
	this->material = material;
}

void Body::SetPosition(Vector pos)
{
	position = pos;
}

void Body::SetVelocity(Vector vel)
{
	velocity = vel;
}

void Body::SetAngle(float newAngle)
{
	angle = newAngle;
	shape->Rotate(angle);
}

void Body::SetAngularVelocity(float newAngularVelocity)
{
	angularVelocity = newAngularVelocity;
}

void Body::SetStatic(bool arg)
{
	bIsStatic = arg;
}

void Body::SetGravityScale(float newGravityScale)
{
	gravityScale = newGravityScale;
}

void Body::SetForces(std::vector<Vector>& forces)
{
	for (Vector f : forces)
	{
		this->forces.emplace_back(f);
	}
}

void Body::SetAdittionalData(std::string data)
{
	adittionalData = data;
}


