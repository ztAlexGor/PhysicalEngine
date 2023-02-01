#include "Body.h"

Body::Body(const BodyInit& init)
{
	position = init.position;
	velocity = init.velocity;
	angle = init.angle;
	angularVelocity = init.angularVelocity;
	bIsStatic = init.bIsStatic;
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
