#include "Collision.h"


CollisionManifold(*Collision::JumpTable[static_cast<int>(Shape::EType::size)][static_cast<int>(Shape::EType::size)])(const Body& bodyA, const Body& bodyB) =
{
    Collision::CircleWithCircle,
    Collision::CircleWithPolygon,
    Collision::PolygonWithCircle,
    Collision::PolygonWithPolygon
};


CollisionManifold Collision::CheckCollision(const Body& bodyA, const Body& bodyB)
{
    return JumpTable[static_cast<int>(bodyA.GetShapeType())][static_cast<int>(bodyB.GetShapeType())](bodyA, bodyB);
}


CollisionManifold Collision::CircleWithCircle(const Body& bodyA, const Body& bodyB)
{
    //отримуємо два круга
    const Circle* c1 = dynamic_cast<const Circle*>(bodyA.GetShape());
    const Circle* c2 = dynamic_cast<const Circle*>(bodyB.GetShape());

    //нормаль зіткнення
    Vector normal = bodyB.GetPosition() - bodyA.GetPosition();

    //квадрат дистанції між центрами та сума радіусів
    float dist = normal.SqLength();
    float sumOfRadii = c1->GetRadius() + c2->GetRadius();

    CollisionManifold manifold;

    //для оптимізації не використовуємо sqrt(), а порівнюємо квадрати відстаней
    if (dist >= sumOfRadii * sumOfRadii) {//перетину немає
        manifold.crossPointsNumber = 0;
    }
    else if (dist == 0) {//випадок коли центри кіл співпадають
        manifold.normal = Vector(1, 0);
        manifold.depth = c1->GetRadius();
        manifold.crossPointsNumber = 1;
        manifold.crossPoint[0] = bodyA.GetPosition();
    }
    else {//перетинаються
        dist = normal.Length();

        manifold.normal = normal / dist;
        manifold.depth = sumOfRadii - dist;
        manifold.crossPointsNumber = 1;
        manifold.crossPoint[0] = manifold.normal * c1->GetRadius() + bodyA.GetPosition();
    }
    return manifold;    
}


CollisionManifold Collision::CircleWithPolygon(const Body& bodyA, const Body& bodyB)
{
    return CollisionManifold();
}


CollisionManifold Collision::PolygonWithCircle(const Body& bodyA, const Body& bodyB)
{
    return CircleWithPolygon(bodyB, bodyA);
}


CollisionManifold Collision::PolygonWithPolygon(const Body& bodyA, const Body& bodyB)
{
    return CollisionManifold();
}




