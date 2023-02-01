#pragma once
#include "Shape.h"

const float PI = 3.141593f;



class Circle : public Shape
{
	float radius;

public:
	Circle(float radius) : Shape(EType::circle), radius(radius) {};

	Circle(const Circle& other);

	float GetRadius() const;

	void SetRadius(float radius);

	virtual AABB ComputeAABB() const override;

	virtual MassInfo ComputeMass(float density) const override;

	virtual Shape* Clone() const override;
};

