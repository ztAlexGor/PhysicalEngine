#pragma once
#include "../Shapes/Shape.h"
#include <vector>
#include <string>


struct MaterialInfo
{
	float restitution;
	float staticFriction;
	float dynamicFriction;
};


struct BodyInit
{
	Vector position;
	Vector velocity;
	float angle;
	float torque;
	float angularVelocity;
	bool bIsStatic = false;
	float gravityScale = 1;
	std::vector<Vector> forces;
	std::string adittionalData;

	//BodyInit()
	//{
	//	position = Vector(0.f, 0.f);
	//	velocity = Vector(0.f, 0.f);
	//	angle = 0;
	//	torque = 0;
	//	angularVelocity = 0;
	//	bIsStatic = false;
	//	float gravityScale = 1.f;
	//}
};


class Body
{
	Shape* m_shape;
	MassInfo m_massInfo;
	MaterialInfo m_material;
	
	Vector m_position;
	Vector m_velocity;
	float m_angle;
	float m_torque;
	float m_angularVelocity;
	bool bIsStatic;
	float m_gravityScale;
	std::vector<Vector> m_forces;
	std::string m_adittionalData;

	Vector m_resultForce;

public:
	Body(const BodyInit& init);
	
	// getters
	const Shape* GetShape() const;

	const MassInfo& GetMassInfo() const;

	const MaterialInfo& GetMaterial() const;

	Vector GetPosition() const;

	Vector GetVelocity() const;

	float GetAngleR() const;

	float GetAngleD() const;

	float GetTorque() const;

	float GetAngularVelocity() const;

	bool IsStatic() const;

	float GetGravityScale() const;

	const std::vector<Vector>& GetForces() const;

	std::string GetAdittionalData() const;

	Shape::EType GetShapeType() const;

	// setters
	void SetShape(Shape* shape, MaterialInfo material, float density);

	void SetMaterial(MaterialInfo material);

	void SetPosition(Vector position);

	void SetVelocity(Vector velocity);

	void SetAngleR(float angle);

	void SetAngleD(float angle);

	void SetTorque(float torque);

	void SetAngularVelocity(float angularVelocity);

	void SetStatic(bool arg);

	void SetGravityScale(float gravityScale);

	void SetForces(std::vector<Vector>& forces);

	void SetAdittionalData(std::string data);

	// main function
	void ClearForces();

	void AddForce(Vector force);

	void ApplyForces(float time, const Vector& gravity);

	void CalculatePosition(float time);

	void ApplyImpulse(Vector impulse, Vector contactVector);

	void ApplyImpulse(Vector impulse, float time);
};

