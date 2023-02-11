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
    //�������� ��� �����
    const Circle* c1 = dynamic_cast<const Circle*>(bodyA.GetShape());
    const Circle* c2 = dynamic_cast<const Circle*>(bodyB.GetShape());

    //������� ��������
    Vector normal = bodyB.GetPosition() - bodyA.GetPosition();

    //������� ��������� �� �������� �� ���� ������
    float dist = normal.SqLength();
    float sumOfRadii = c1->GetRadius() + c2->GetRadius();

    CollisionManifold manifold;

    //��� ���������� �� ������������� sqrt(), � ��������� �������� ��������
    if (dist >= sumOfRadii * sumOfRadii) {//�������� ����
        manifold.crossPointsNumber = 0;
    }
    else if (dist == 0) {//������� ���� ������ �� ����������
        manifold.normal = Vector(1, 0);
        manifold.depth = c1->GetRadius();
        manifold.crossPointsNumber = 1;
        manifold.crossPoint[0] = bodyA.GetPosition();
    }
    else {//�������������
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
    //�������� ���� �� �����������
    const Circle* circle = dynamic_cast<const Circle*>(bodyA.GetShape());
    const Polygon* polygon = dynamic_cast<const Polygon*>(bodyB.GetShape());

    //�������� ������ �������� �� ������ ������������
    const std::vector<Vector>& normals = polygon->GetNormals();
    const std::vector<Vector>& vertecies = polygon->GetVertices();

    CollisionManifold manifold{};
    manifold.crossPointsNumber = 0;

    //������������ ���������� ������ ���� � ������� ��������� ������������
    Vector center = bodyA.GetPosition();
    center = polygon->GetMatrix().Transpose() * (center - bodyB.GetPosition());

    //��������� "�������" �����
    //������� ������ �� ������������ ������� �����
    float separation = -9999999;
    int faceNormal = 0;
    for (int i = 0; i < normals.size(); ++i) {
        float s = Vector::DotProduct(normals[i], Vector(center - vertecies[i]));

        //���� ����� ������� �� ������� �� ������ �� �����,
        //�� ��'���� �� �������������
        if (s > circle->GetRadius())
            return manifold;

        if (s > separation) {
            separation = s;
            faceNormal = i;
        }
    }

    //��������� �� ������� ��������� "���������" �����
    Vector v1 = vertecies[faceNormal];
    int i2 = faceNormal + 1 < vertecies.size() ? faceNormal + 1 : 0;
    Vector v2 = vertecies[i2];

    //���������� �� ����������� ����� �������� ������������
    //���� separation ��'���� ��� ������� �� ����, �� ����� �������� ��� �� ����
    if (separation < 0.0001) {
        manifold.crossPointsNumber = 1;
        manifold.normal = -(polygon->GetMatrix() * normals[faceNormal]);
        manifold.crossPoint[0] = manifold.normal * circle->GetRadius() + bodyA.GetPosition();
        manifold.depth = circle->GetRadius();
        return manifold;
    }

    //��������� � ��� ������ ����������� ����� ����
    float dot1 = Vector::DotProduct(Vector(center - v1), Vector(v2 - v1));
    float dot2 = Vector::DotProduct(Vector(center - v2), Vector(v1 - v2));
    manifold.depth = circle->GetRadius() - separation;

    //����� ���� ������ �� ������� V1
    if (dot1 <= 0.0f) {
        if (Vector(center - v1).SqLength() > circle->GetRadius() * circle->GetRadius())
            return manifold;

        manifold.crossPointsNumber = 1;
        Vector n(v1 - center);
        manifold.depth = circle->GetRadius() - n.Length();
        n = polygon->GetMatrix() * n;
        n.Normalize();
        manifold.normal = n;
        v1 = polygon->GetMatrix() * v1 + bodyB.GetPosition();
        manifold.crossPoint[0] = v1;

    }

    //����� ���� ������ �� ������� V2
    else if (dot2 <= 0.0f) {
        if (Vector(center - v2).SqLength() > circle->GetRadius() * circle->GetRadius())
            return manifold;

        manifold.crossPointsNumber = 1;
        Vector n(v2 - center);
        manifold.depth = circle->GetRadius() - n.Length();
        v2 = polygon->GetMatrix() * Vector(v2) + Vector(bodyB.GetPosition());
        manifold.crossPoint[0] = v2;
        n = polygon->GetMatrix() * n;
        n.Normalize();
        manifold.normal = n;
    }

    //����� ���� ����������� �� V1 �� V2
    else {
        Vector n = normals[faceNormal];
        if (Vector::DotProduct(Vector(center - v1), n) > circle->GetRadius())
            return manifold;

        n = polygon->GetMatrix() * n;
        manifold.normal = -n;
        manifold.crossPoint[0] = manifold.normal * circle->GetRadius() + Vector(bodyA.GetPosition());
        manifold.crossPointsNumber = 1;
    }
    return manifold;
}


CollisionManifold Collision::PolygonWithCircle(const Body& bodyA, const Body& bodyB)
{
    return CircleWithPolygon(bodyB, bodyA);
}


CollisionManifold Collision::PolygonWithPolygon(const Body& bodyA, const Body& bodyB)
{
    return CollisionManifold();
}




