#pragma once
#include "../Engine/Body.h"



struct CollisionManifold
{
	static constexpr size_t MAX_SIZE = 2;

	float depth;
	Vector normal;
	Vector crossPoint[MAX_SIZE];
	int crossPointsNumber;
};



struct CollisionPair
{
	struct Properties
	{
		float restitution;
		float df;
		float sf;
	};

	static bool bIsFrictionEnable;

	Body& bodyA;
	Body& bodyB;
	CollisionManifold manifold;
	Properties props;
	

	CollisionPair(Body& a, Body& b, CollisionManifold manifold);

	void InitProperties(float time, Vector gravity);
};
