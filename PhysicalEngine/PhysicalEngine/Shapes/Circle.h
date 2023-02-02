#pragma once
#include "Shape.h"

const float PI = 3.141593f;



class Circle : public Shape
{
	float radius;

public:
	Circle(float radius);

	Circle(const Circle& other) : Circle(other.radius) {};

	float GetRadius() const;

	void SetRadius(float radius);

	virtual void InitAABB() override;

	virtual MassInfo ComputeMass(float density) const override;

	virtual Shape* Clone() const override;
};

