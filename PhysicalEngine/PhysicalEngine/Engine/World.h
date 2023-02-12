#pragma once
#include "Body.h"
#include "../CollisionTools/CollisionStructs.h"
#include <vector>
#include <memory>


class World
{
	Vector m_gravity;
	std::vector<Body>m_bodies;
	std::vector<Vector> m_forces;
	bool bIsFrictionEnable;

	std::vector<std::shared_ptr<CollisionPair>> collisions;

public:
	World() = default;

	void SetGravity(Vector gravity);

	void SetFrictionEnable(bool arg);

	void AddBody(Body body);

	void DeleteAllBodies();

	void AddForce(Vector force);

	void SetForces(std::vector<Vector> forces);

	void ClearForces();

	Vector GetGravity() const;

	bool IsFrictionEnable() const;

	std::vector<Body>& GetBodies();

	const std::vector<Body>& GetBodies() const;

	std::vector<Vector> GetForces();

	const std::vector<Vector> GetForces() const;


	// main engine function
	void Step(float time, int iterNum);

private:
	void FindCollisions();

	void FixCollision(float time, int iterNum);
};

