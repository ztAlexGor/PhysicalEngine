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


    //застосовуємо усі додаткові сили (наприклад, силу тяжіння)
    for (Body& body : m_bodies)
    {
        body.ApplyForces(time / 2.f, m_gravity);
    }

    // find all collision pairs
    FindCollisions();

    // обраховуємо коєфіцієнти тертя та відновлення
    for (std::shared_ptr<CollisionPair>& collision : collisions) 
    {
        collision->InitProperties(time, m_gravity);
    }

    for (std::shared_ptr<CollisionPair>& collision : collisions)
    {
        if (collision->bodyA.GetAdittionalData() != "key")collision->bodyA.SetAdittionalData("Collision");
        if (collision->bodyB.GetAdittionalData() != "key")collision->bodyB.SetAdittionalData("Collision");
    }

    ////вирішуємо колізію шляхом надання імпульсу
    ////повторюємо дію iterNum кількість разів
    //for (int j = 0; j < iterNum; ++j)
    //    for (int i = 0; i < collisions.size(); ++i)
    //        collisions[i]->FixCollision();

    ////оновлюємо позицію тіла за певний проміжок часу
    //for (int i = 0; i < m_bodies.size(); ++i)
    //    m_bodies[i].CalculatePos(time);

    ////позиційна корекція
    //for (int i = 0; i < collisions.size(); ++i)
    //    collisions[i]->PositionalCorrection();

    //очищуємо список колізій
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

            // статичні тіла не взаємодіють один із одним
            if (a.IsStatic() && b.IsStatic())continue;

            // перевіряємо на наявність колізії для пари (a, b) та зберігаємо
            // інформацію про це до об'єкту CollisionManifold
            CollisionManifold manifold = Collision::CheckCollision(a, b);

            // якщо інсують точки перетину, значить колізія відбулась
            if (manifold.crossPointsNumber)
            {
                collisions.push_back(std::make_shared<CollisionPair>(CollisionPair(a, b, manifold)));
            }
            //else
            //{
            //    // якщо колізії немає, то видаляємо цю пару
            //    delete collisionPair;
            //}
        }
    }

    //підраховуємо загальну кількість колізій
    //numOfColl += collisions.size();
}