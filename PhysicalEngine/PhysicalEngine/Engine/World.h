#pragma once
#include "Body.h"
#include "../CollisionTools/Collision.h"
#include <vector>
#include <memory>


class World
{
	Vector m_gravity;
	std::vector<Body>m_bodies;
	std::vector<Vector> m_forces;


	std::vector<std::shared_ptr<CollisionPair>> collisions;

public:
	World() = default;

	void SetGravity(Vector gravity);

	void AddBody(Body body);

	void DeleteAllBodies();

	void AddForce(Vector force);

	void SetForces(std::vector<Vector> forces);

	void ClearForces();

	Vector GetGravity() const;

	std::vector<Body>& GetBodies();

	const std::vector<Body>& GetBodies() const;

	std::vector<Vector> GetForces();

	const std::vector<Vector> GetForces() const;


	// main engine function
	void Step(float time, int iterNum);

private:
	void FindCollisions();
};

