#include "World.h"

World::World()
{
    bIsFrictionEnable = true;
}

void World::SetGravity(const Vector& gravity)
{
	m_gravity = gravity;
}

void World::SetFrictionEnable(bool arg)
{
    bIsFrictionEnable = arg;
}

void World::AddBody(Body& body)
{
	m_bodies.emplace_back(body);
}

void World::DeleteAllBodies()
{
	m_bodies.clear();
}

void World::AddForce(const Vector& force)
{
	m_forces.emplace_back(force);
}

void World::SetForces(std::vector<Vector> forces)
{
    m_forces = forces;
    //for (Vector& force : forces)
    //{
    //    m_forces.emplace_back(force);
    //}
}

void World::ClearForces()
{
	m_forces.clear();
}

Vector World::GetGravity() const
{
	return m_gravity;
}

bool World::IsFrictionEnable() const
{
    return bIsFrictionEnable;
}

std::vector<Body>& World::GetBodies()
{
    return m_bodies;
}

const std::vector<Body>& World::GetBodies() const
{
    return m_bodies;
}

std::vector<Vector>& World::GetForces()
{
    return m_forces;
}

const std::vector<Vector>& World::GetForces() const
{
    return m_forces;
}


void World::Step(float time, size_t iterNum)
{
    //застосовуємо усі додаткові сили (наприклад, силу тяжіння) за час = time / 2.f
    for (Body& body : m_bodies)
    {
        body.ApplyForces(time / 2.f, m_gravity);
    }

    // find all collision pairs
    FindCollisions();

    // fix collisions
    FixCollision(time, iterNum);

    //оновлюємо позицію тіла за певний проміжок часу
    for (Body& body : m_bodies)
    {
        body.CalculatePosition(time);
    }

    // вдруге застосовуємо усі сили за час = time / 2.f
    for (Body& body : m_bodies)
    {
        body.ApplyForces(time / 2.f, m_gravity);
    }

    //позиційна корекція
    for (std::shared_ptr<CollisionPair>& collision : collisions)
    {
        collision->PositionalCorrection();
    }



    /////////////////////////////////////////////step///////////////////////////////////////////////////////////
    //for (Body& body : m_bodies)
    //{
    //    if (body.GetAdittionalData() == "key" || body.GetAdittionalData() == "keyCollision")
    //        body.SetAdittionalData("key");
    //    else if (body.GetAdittionalData() == "" || body.GetAdittionalData() == "Collision") {
    //        body.SetAdittionalData("");
    //    }
    //    else {
    //        int r = 0;
    //    }
    //}
    //
    //for (std::shared_ptr<CollisionPair>& collision : collisions)
    //{
    //    collision->GetBodyA().SetAdittionalData((collision->GetBodyA().GetAdittionalData() == "key" || collision->GetBodyA().GetAdittionalData() == "keyCollision") ? "keyCollision" : "Collision");
    //    collision->GetBodyB().SetAdittionalData((collision->GetBodyB().GetAdittionalData() == "key" || collision->GetBodyB().GetAdittionalData() == "keyCollision") ? "keyCollision" : "Collision");
    //}
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////



    //очищуємо список колізій
    collisions.clear();

    for (Body& body : m_bodies)
    {
        body.ClearForces();
        body.SetTorque(0.f);
    }
}


void World::FindCollisions()
{
    for (size_t i = 0; i < m_bodies.size() - 1; ++i)
    {
        Body& a = m_bodies[i];

        for (size_t j = i + 1; j < m_bodies.size(); ++j)
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
        }
    }

    //підраховуємо загальну кількість колізій
    //numOfColl += collisions.size();
}

void World::FixCollision(float time, size_t iterNum)
{
    // обраховуємо коєфіцієнти тертя та відновлення
    for (std::shared_ptr<CollisionPair>& collision : collisions)
    {
        collision->InitProperties(time, m_gravity);
    }

    //вирішуємо колізію шляхом надання імпульсу
    //повторюємо дію iterNum кількість разів
    for (size_t j = 0; j < iterNum; ++j)
    {
        for (std::shared_ptr<CollisionPair>& collision : collisions)
        {
            collision->FixCollision(bIsFrictionEnable);
        }
    }
}
