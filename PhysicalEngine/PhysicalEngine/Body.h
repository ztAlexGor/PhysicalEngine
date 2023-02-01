#pragma once
#include "Shapes/Shape.h"
#include <vector>
#include <string>


struct MaterialInfo
{
	float restitution;
	float staticFriction;
	float dynamicFriction;
	float gravityScale;
};


struct BodyInit
{
	Vector position;
	Vector velocity;
	float angle;
	float angularVelocity;
	bool bIsStatic;
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
	std::vector<Vector> forces;
	std::string adittionalData;

public:
	Body(const BodyInit& init);
	
	void SetShape(Shape* newShape, MaterialInfo material, float density);


};

