#include "Body.h"

Body::Body(const BodyInit& init)
{
	m_position = init.position;
	m_velocity = init.velocity;
	m_angle = init.angle;
	m_torque = init.torque;
	m_angularVelocity = init.angularVelocity;
	bIsStatic = init.bIsStatic;
	m_gravityScale = init.gravityScale;
	m_adittionalData = init.adittionalData;

	for (Vector f : init.forces)
	{
		m_forces.emplace_back(f);
	}

	m_resultForce.SetZero();
}

void Body::SetShape(Shape* shape, MaterialInfo shapeMaterial, float density)
{
	m_shape = shape->Clone();
	m_material = shapeMaterial;
	m_massInfo = m_shape->ComputeMass(density);
}

const Shape* Body::GetShape() const
{
	return m_shape;
}

const MassInfo& Body::GetMassInfo() const
{
	return m_massInfo;
}

const MaterialInfo& Body::GetMaterial() const
{
	return m_material;
}

Vector Body::GetPosition() const
{
	return m_position;
}

Vector Body::GetVelocity() const
{
	return m_velocity;
}

float Body::GetAngle() const
{
	return m_angle;
}

float Body::GetAngularVelocity() const
{
	return m_angularVelocity;
}

bool Body::IsStatic() const
{
	return bIsStatic;
}

float Body::GetGravityScale() const
{
	return m_gravityScale;
}

const std::vector<Vector>& Body::GetForces() const
{
	return m_forces;
}

std::string Body::GetAdittionalData() const
{
	return m_adittionalData;
}

Shape::EType Body::GetShapeType() const
{
	return m_shape->GetType();
}

void Body::SetMaterial(MaterialInfo material)
{
	m_material = material;
}

void Body::SetPosition(Vector position)
{
	m_position = position;
}

void Body::SetVelocity(Vector velocity)
{
	m_velocity = velocity;
}

void Body::SetAngle(float angle)
{
	m_angle = angle;
	m_shape->Rotate(m_angle);
}

void Body::SetAngularVelocity(float angularVelocity)
{
	m_angularVelocity = angularVelocity;
}

void Body::SetStatic(bool arg)
{
	bIsStatic = arg;
}

void Body::SetGravityScale(float gravityScale)
{
	m_gravityScale = gravityScale;
}

void Body::SetForces(std::vector<Vector>& forces)
{
	for (Vector force : forces)
	{
		m_resultForce += force;
		m_forces.emplace_back(force);
	}
}

void Body::SetAdittionalData(std::string data)
{
	m_adittionalData = data;
}


void Body::ClearForces()
{
	m_forces.clear();
}


void Body::AddForce(Vector force)
{
	m_resultForce += force;
	m_forces.emplace_back(force);
}


void Body::ApplyForces(float time, const Vector& gravity)
{
	if (bIsStatic)return;

	m_velocity += (m_resultForce * m_massInfo.invMass + gravity * m_gravityScale) * time;
	m_angularVelocity += m_torque * m_massInfo.invInertia * time;
}


