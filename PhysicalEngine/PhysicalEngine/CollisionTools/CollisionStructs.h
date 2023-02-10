#pragma once
#include "Collision.h"


class CollisionPair
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
	
public:

	CollisionPair(Body& a, Body& b, CollisionManifold manifold);

	Body& GetBodyA();

	Body& GetBodyB();

	void InitProperties(float time, Vector gravity);

	void FixCollision(bool isFrictionEnable);

	void PositionalCorrection();
};
