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
        if (body.GetAdittionalData() == "key" || body.GetAdittionalData() == "keyCollision")
            body.SetAdittionalData("key");
        else if (body.GetAdittionalData() == "" || body.GetAdittionalData() == "Collision") {
            body.SetAdittionalData("");
        }
        else {
            int r = 0;
        }
    }
    /////////////////////////////////////////////step///////////////////////////////////////////////////////////
    
    //����������� �� �������� ���� (���������, ���� ������) �� ��� = time / 2.f
    for (Body& body : m_bodies)
    {
        body.ApplyForces(time / 2.f, m_gravity);
    }

    // find all collision pairs
    FindCollisions();

    // fix collisions
    FixCollision(time, iterNum);

    //��������� ������� ��� �� ������ ������� ����
    for (Body& body : m_bodies)
    {
        body.CalculatePosition(time);
    }

    // ������ ����������� �� ���� �� ��� = time / 2.f
    for (Body& body : m_bodies)
    {
        body.ApplyForces(time / 2.f, m_gravity);
    }

    //��������� ��������
    for (std::shared_ptr<CollisionPair> collision : collisions)
    {
        collision->PositionalCorrection();
    }



    /////////////////////////////////////////////////////////////////////////////////////////////////////////////
    for (std::shared_ptr<CollisionPair>& collision : collisions)
    {
        collision->GetBodyA().SetAdittionalData((collision->GetBodyA().GetAdittionalData() == "key" || collision->GetBodyA().GetAdittionalData() == "keyCollision") ? "keyCollision" : "Collision");
        collision->GetBodyB().SetAdittionalData((collision->GetBodyB().GetAdittionalData() == "key" || collision->GetBodyB().GetAdittionalData() == "keyCollision") ? "keyCollision" : "Collision");
    }
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////



    //������� ������ �����
    collisions.clear();

    for (Body& body : m_bodies)
    {
        body.ClearForces();
        body.SetTorque(0.f);
    }
}


void World::FindCollisions()
{
    for (int i = 0; i < m_bodies.size() - 1; ++i)
    {
        Body& a = m_bodies[i];

        for (int j = i + 1; j < m_bodies.size(); ++j)
        {
            Body& b = m_bodies[j];

            // �������� ��� �� ��������� ���� �� �����
            if (a.IsStatic() && b.IsStatic())continue;

            // ���������� �� ��������� ���糿 ��� ���� (a, b) �� ��������
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

void World::FixCollision(float time, int iterNum)
{
    // ���������� ���������� ����� �� ����������
    for (std::shared_ptr<CollisionPair>& collision : collisions)
    {
        collision->InitProperties(time, m_gravity);
    }

    //������� ����� ������ ������� ��������
    //���������� �� iterNum ������� ����
    for (int j = 0; j < iterNum; ++j)
    {
        for (std::shared_ptr<CollisionPair>& collision : collisions)
        {
            collision->FixCollision(bIsFrictionEnable);
        }
    }
}