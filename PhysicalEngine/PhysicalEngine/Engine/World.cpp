#include "World.h"

void World::SetGravity(Vector gravity)
{
	m_gravity = std::move(gravity);
}

void World::AddBody(Body body)
{
	m_bodies.emplace_back(body);
}

void World::DeleteAllBodies()
{
	m_bodies.clear();
}

void World::AddForce(Vector force)
{
	m_forces.emplace_back(force);
}

void World::ClearForces()
{
	m_forces.clear();
}

Vector World::GetGravity() const
{
	return m_gravity;
}

std::vector<Body>& World::GetBodies()
{
    return m_bodies;
}


void World::Step(float time, int iterNum)
{
    for (Body& body : m_bodies)
    {
        if (body.GetAdittionalData() != "key")
            body.SetAdittionalData("");
    }


    //����������� �� �������� ���� (���������, ���� ������)
    for (Body& body : m_bodies)
    {
        body.ApplyForces(time / 2.f, m_gravity);
    }

    // find all collision pairs
    FindCollisions();

    // ���������� ���������� ����� �� ����������
    for (std::shared_ptr<CollisionPair>& collision : collisions) 
    {
        collision->InitProperties(time, m_gravity);
    }

    for (std::shared_ptr<CollisionPair>& collision : collisions)
    {
        if (collision->bodyA.GetAdittionalData() != "key")collision->bodyA.SetAdittionalData("Collision");
        if (collision->bodyB.GetAdittionalData() != "key")collision->bodyB.SetAdittionalData("Collision");
    }

    ////������� ����� ������ ������� ��������
    ////���������� �� iterNum ������� ����
    //for (int j = 0; j < iterNum; ++j)
    //    for (int i = 0; i < collisions.size(); ++i)
    //        collisions[i]->FixCollision();

    ////��������� ������� ��� �� ������ ������� ����
    //for (int i = 0; i < m_bodies.size(); ++i)
    //    m_bodies[i].CalculatePos(time);

    ////��������� ��������
    //for (int i = 0; i < collisions.size(); ++i)
    //    collisions[i]->PositionalCorrection();

    //������� ������ �����
    collisions.clear();
}


void World::FindCollisions()
{
    for (int i = 0; i < m_bodies.size() - 1; ++i)
    {
        Body& a = m_bodies[i];

        for (int j = i + 1; j < m_bodies.size(); ++j)
        {
            Body& b = m_bodies[j];

            // ������� ��� �� ��������� ���� �� �����
            if (a.IsStatic() && b.IsStatic())continue;

            // ���������� �� �������� ���糿 ��� ���� (a, b) �� ��������
            // ���������� ��� �� �� ��'���� CollisionManifold
            CollisionManifold manifold = Collision::CheckCollision(a, b);

            // ���� ������� ����� ��������, ������� ����� ��������
            if (manifold.crossPointsNumber)
            {
                collisions.push_back(std::make_shared<CollisionPair>(CollisionPair(a, b, manifold)));
            }
            //else
            //{
            //    // ���� ���糿 ����, �� ��������� �� ����
            //    delete collisionPair;
            //}
        }
    }

    //���������� �������� ������� �����
    //numOfColl += collisions.size();
}