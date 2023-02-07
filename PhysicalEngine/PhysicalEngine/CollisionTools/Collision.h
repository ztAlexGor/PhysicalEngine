#pragma once
#include "CollisionStructs.h"


class Collision
{
	static CollisionManifold(* JumpTable[static_cast<int>(Shape::EType::size)][static_cast<int>(Shape::EType::size)])(const Body& bodyA, const Body& bodyB);

public:
	static CollisionManifold CheckCollision(const Body& bodyA, const Body& bodyB);

	static CollisionManifold CircleWithCircle(const Body& bodyA, const Body& bodyB);

	static CollisionManifold CircleWithPolygon(const Body& bodyA, const Body& bodyB);

	static CollisionManifold PolygonWithCircle(const Body& bodyA, const Body& bodyB);

	static CollisionManifold PolygonWithPolygon(const Body& bodyA, const Body& bodyB);


};

