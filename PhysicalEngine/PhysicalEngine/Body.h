#pragma once
#include "Shapes/Shape.h"
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
	float angularVelocity;
	bool bIsStatic;
	float gravityScale;
	std::vector<Vector> forces;
	std::string adittionalData;
};


class Body
{
	Shape* shape;
	MassInfo massInfo;
	MaterialInfo material;
	
	Vector position;
	Vector velocity;
	float angle;
	float angularVelocity;
	bool bIsStatic;
	float gravityScale;
	std::vector<Vector> forces;
	std::string adittionalData;

public:
	Body(const BodyInit& init);
	
	// getters
	const Shape* GetShape() const;

	const MassInfo& GetMassInfo() const;

	const MaterialInfo& GetMaterial() const;

	Vector GetPosition() const;

	Vector GetVelocity() const;

	float GetAngle() const;

	float GetAngularVelocity() const;

	bool IsStatic() const;

	float GetGravityScale() const;

	const std::vector<Vector>& GetForces() const;

	std::string GetAdittionalData() const;

	// setters
	void SetShape(Shape* newShape, MaterialInfo material, float density);

	void SetMaterial(MaterialInfo material);

	void SetPosition(Vector pos);

	void SetVelocity(Vector vel);

	void SetAngle(float newAngle);

	void SetAngularVelocity(float newAngularVelocity);

	void SetStatic(bool arg);

	void SetGravityScale(float newGravityScale);

	void SetForces(std::vector<Vector>& forces);

	void SetAdittionalData(std::string data);
};

