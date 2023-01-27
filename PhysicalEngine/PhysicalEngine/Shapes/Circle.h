#pragma once
#include "Shape.h"

const float PI = 3.141593f;



class Circle : public Shape
{
	float radius;

public:
	Circle(float radius) : Shape(EType::circle), radius(radius) {};

	Circle(Circle& other);

	float GetRadius() const;

	void SetRadius(float radius);

	virtual AABB ComputeAABB() const override;

	virtual float ComputeMass() const override;
};

