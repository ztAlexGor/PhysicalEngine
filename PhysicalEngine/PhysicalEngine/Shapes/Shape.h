#pragma once
#include "..\Optimisation\AABB.h"


struct MassInfo
{
	float mass;
	float invMass;
	float inertia;
	float invInertia;
};


class Shape
{
public:
	enum EType
	{
		circle = 0,
		polygon = 1,
		size = 2
	};

protected:
	AABB aabb;
	EType type;

public:
	Shape(EType type) : type(type), aabb(ComputeAABB()) {};

	AABB GetAABB() const;

	EType GetType() const;

	virtual AABB ComputeAABB() const = 0;
	
	virtual MassInfo ComputeMass(float density) const = 0;

	virtual Shape* Clone() const = 0;

	virtual ~Shape() = 0;
};

