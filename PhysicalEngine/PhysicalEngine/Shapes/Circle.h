#pragma once
#include "Shape.h"

const float PI = 3.141593f;



class Circle : public Shape
{
	float m_radius;

public:
	Circle(float radius);

	Circle(const Circle& other) : Circle(other.m_radius) {};

	float GetRadius() const;

	void SetRadius(float radius);

	virtual MassInfo ComputeMass(float density) const override;

	virtual Shape* Clone() const override;

	void Rotate(float angle) override {};

	bool IsPointInShape(const Vector point) const override;

protected:
	virtual void InitAABB() override;
};

