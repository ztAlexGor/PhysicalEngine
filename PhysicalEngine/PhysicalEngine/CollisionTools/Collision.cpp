#include "Collision.h"


inline bool BiasGreaterThan(float a, float b)
{
    const float k_biasRelative = 0.95f;
    const float k_biasAbsolute = 0.01f;
    return a >= b * k_biasRelative + a * k_biasAbsolute;
}



CollisionManifold::CollisionManifold() : depth(-INFINITY), crossPointsNumber(0) {}


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

bool Collision::BroadPhase(const Body& bodyA, const Body& bodyB)
{
    AABB aabb_A = bodyA.GetShape()->GetAABB() + bodyA.GetPosition();
    AABB aabb_B = bodyB.GetShape()->GetAABB() + bodyB.GetPosition();

    return AABB::IsIntersect(aabb_A, aabb_B);
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
    else if (abs(dist) < FLT_EPSILON) {//������� ���� ������ �� ����������
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
    float separation = -INFINITY;
    size_t faceNormal = 0;
    for (size_t i = 0; i < normals.size(); ++i)
    {
        float s = Vector::DotProduct(normals[i], center - vertecies[i]);

        //���� ����� ������� �� ������� �� ������ �� �����,
        //�� ��'���� �� �������������
        if (s > circle->GetRadius())
            return manifold;

        if (s > separation)
        {
            separation = s;
            faceNormal = i;
        }
    }

    //��������� �� ������� ��������� "���������" �����
    Vector v1 = vertecies[faceNormal];
    size_t i2 = faceNormal + 1 < vertecies.size() ? faceNormal + 1 : 0;
    Vector v2 = vertecies[i2];

    //���������� �� ����������� ����� �������� ������������
    //���� separation ��'���� ��� ������� �� ����, �� ����� �������� ��� �� ����
    if (separation < 0.0001)
    {
        manifold.crossPointsNumber = 1;
        manifold.normal = -(polygon->GetMatrix() * normals[faceNormal]);
        manifold.crossPoint[0] = manifold.normal * circle->GetRadius() + bodyA.GetPosition();
        manifold.depth = circle->GetRadius();
        return manifold;
    }

    //��������� � ��� ������ ����������� ����� ����
    float dot1 = Vector::DotProduct(center - v1, v2 - v1);
    float dot2 = Vector::DotProduct(center - v2, v1 - v2);
    manifold.depth = circle->GetRadius() - separation;

    //����� ���� ������ �� ������� V1
    if (dot1 <= 0.0f)
    {
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
    else if (dot2 <= 0.0f)
    {
        if (Vector(center - v2).SqLength() > circle->GetRadius() * circle->GetRadius())
            return manifold;

        manifold.crossPointsNumber = 1;
        Vector n(v2 - center);
        manifold.depth = circle->GetRadius() - n.Length();
        v2 = polygon->GetMatrix() * v2 + bodyB.GetPosition();
        manifold.crossPoint[0] = v2;
        n = polygon->GetMatrix() * n;
        n.Normalize();
        manifold.normal = n;
    }

    //����� ���� ����������� �� V1 �� V2
    else
    {
        Vector n = normals[faceNormal];
        if (Vector::DotProduct(center - v1, n) > circle->GetRadius())
            return manifold;

        n = polygon->GetMatrix() * n;
        manifold.normal = -n;
        manifold.crossPoint[0] = manifold.normal * circle->GetRadius() + bodyA.GetPosition();
        manifold.crossPointsNumber = 1;
    }
    return manifold;
}


CollisionManifold Collision::PolygonWithCircle(const Body& bodyA, const Body& bodyB)
{
    CollisionManifold manifold = CircleWithPolygon(bodyB, bodyA);
    manifold.normal = -manifold.normal;
    return manifold;
}


CollisionManifold Collision::PolygonWithPolygon(const Body& bodyA, const Body& bodyB)
{
    //��� ��� �����
    CollisionManifold manifold;

    //�������� ��� ������������
    const Polygon* r1 = dynamic_cast<const Polygon*>(bodyA.GetShape());
    const Polygon* r2 = dynamic_cast<const Polygon*>(bodyB.GetShape());

    //���������� �������� �������� �� �� ����� �������� r1
    //������ "�������" ����� �� ������� ����������� ��� r1
    int faceA = 0;
    float penetrationA = FindAxisLeastPenetration(&faceA, r1, r2, &bodyA, &bodyB);

    if (penetrationA >= 0.0f)
        return manifold;

    //���������� �������� �������� �� �� ����� �������� r2
    //������ "�������" ����� �� ������� ����������� ��� r2
    int faceB = 0;
    float penetrationB = FindAxisLeastPenetration(&faceB, r2, r1, &bodyB, &bodyA);

    if (penetrationB >= 0.0f)
        return manifold;


    size_t referenceIndex;
    bool flip; //����� ����� �� �������� �� r1 �� r2

    const Polygon* RefPoly; //����������� �����������
    const Polygon* IncPoly; //����������� �����������
    const Body* RefPolyBody;
    const Body* IncPolyBody;

    //��������� ����������� �� ����������� �����������
    if (BiasGreaterThan(penetrationA, penetrationB)) {
        RefPoly = r1;
        IncPoly = r2;
        RefPolyBody = &bodyA;
        IncPolyBody = &bodyB;
        referenceIndex = faceA;
        flip = false;
    }
    else {
        RefPoly = r2;
        IncPoly = r1;
        RefPolyBody = &bodyB;
        IncPolyBody = &bodyA;
        referenceIndex = faceB;
        flip = true;
    }

    //������ ������� ������������ ����� � ���������� �����������
    Vector incidentFace[2];
    FindIncidentFace(incidentFace, RefPoly, IncPoly, RefPolyBody, IncPolyBody, RefPoly->GetNormals()[referenceIndex]);

    //�������� �� ������� ������������ ������������
    Vector v1 = RefPoly->GetVertices()[referenceIndex];
    referenceIndex = referenceIndex + 1 == RefPoly->GetVertices().size() ? 0 : referenceIndex + 1;
    Vector v2 = RefPoly->GetVertices()[referenceIndex];

    //������������ �� �� ��������� ��
    v1 = RefPoly->GetMatrix() * Vector(v1) + Vector(RefPolyBody->GetPosition());
    v2 = RefPoly->GetMatrix() * Vector(v2) + Vector(RefPolyBody->GetPosition());

    //������������ ������ ������������ ����� � ��������� ��
    Vector sidePlaneNormal = Vector(v2 - v1);
    sidePlaneNormal.Normalize();

    //�������� ���������� ������� ������� ������ �������� �� 90 �������
    Vector refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

    // ax + by = c
    // c - ������� �� ������� ��������� ��
    float refC = Vector::DotProduct(refFaceNormal, Vector(v1));
    float negSide = -Vector::DotProduct(sidePlaneNormal, Vector(v1));
    float posSide = Vector::DotProduct(sidePlaneNormal, Vector(v2));

    //������� ���������� ����� ������������� ����������� �����
    if (Clip(-sidePlaneNormal, negSide, incidentFace) < 2)
        return manifold; //����� ������� ���������� ����� � float ����� ���� �� ����

    if (Clip(sidePlaneNormal, posSide, incidentFace) < 2)
        return manifold; //����� ������� ���������� ����� � float ����� ���� �� ����

    //���� �������, ����������� �������
    manifold.normal = flip ? -refFaceNormal : refFaceNormal;

    //��������� �����, �� ����������� �������� ������������
    int cp = 0; //�������� �����
    float separation = Vector::DotProduct(refFaceNormal, incidentFace[0]) - refC;
    if (separation <= 0.0f) {
        manifold.crossPoint[cp] = incidentFace[0];
        manifold.depth = -separation;
        ++cp;
    }
    else
        manifold.depth = 0;

    separation = Vector::DotProduct(refFaceNormal, incidentFace[1]) - refC;
    if (separation <= 0.0f) {
        manifold.crossPoint[cp] = incidentFace[1];

        manifold.depth -= separation;
        ++cp;

        //����������� ������ �����������
        manifold.depth /= (float)cp;
    }

    manifold.crossPointsNumber = cp;

    return manifold;
}


float Collision::FindAxisLeastPenetration(int* faceIndex, const Polygon* polygonA, const Polygon* polygonB, const Body* bodyA, const Body* bodyB)
{
    float bestDistance = -INFINITY;
    int bestIndex = -1;

    //�������� ������ �������� �� ������ ������������
    const std::vector<Vector>& norm = polygonA->GetNormals();
    const std::vector<Vector>& vert = polygonA->GetVertices();

    //��� ������� ����� ��������� ������� "�����������"
    for (int i = 0; i < vert.size(); ++i)
    {
        //�������� ������� �������
        Vector n = norm[i];
        //������������ �� � ��������� ������� ���������
        Vector nw = polygonA->GetMatrix() * n;

        //������������ �� � ������� ��������� ������� ������������
        Matrix buT = polygonB->GetMatrix().Transpose();
        n = buT * nw;

        //������ ������ ����� � �������� ������������ �� �������� ������
        Vector s(polygonB->GetSupportPoint(-n));

        //�������� ������� �� �������� ����� "�" ��
        //������������ �� � ������� ��������� ������������ B
        Vector v(vert[i]);
        v = polygonA->GetMatrix() * v + Vector(bodyA->GetPosition());
        v -= Vector(bodyB->GetPosition());
        v = buT * v;

        //���������� "�������" ������� �� ��������� � �������� ������
        float d = Vector::DotProduct(n, s - v);

        //�������� ��������� ���������
        if (d > bestDistance)
        {
            bestDistance = d;
            bestIndex = i;
        }
    }

    *faceIndex = bestIndex;
    return bestDistance;
}


void Collision::FindIncidentFace(Vector* v, const Polygon* RefPoly, const Polygon* IncPoly, const Body* Ab, const Body* Bb, Vector referenceNormal)
{
    //�������� ������ �� ������� ������������ ������������
    const std::vector<Vector>& norm = IncPoly->GetNormals();
    const std::vector<Vector>& vert = IncPoly->GetVertices();

    //������������ ������� reference �� ������� ��������� incident
    referenceNormal = RefPoly->GetMatrix() * referenceNormal; //�� ��������� ��
    referenceNormal = IncPoly->GetMatrix().Transpose() * referenceNormal; //�� �� incident

    //������ ������������ � ������������ �� ������ �������� ����� ������������ incident
    size_t incidentFace = 0;
    float minDot = INFINITY;
    for (size_t i = 0; i < vert.size(); ++i)
    {
        float dot = Vector::DotProduct(referenceNormal, norm[i]);

        if (dot < minDot)
        {
            minDot = dot;
            incidentFace = i;
        }
    }

    //�������� �� ������� ���������� ����� � ��������� ��
    v[0] = IncPoly->GetMatrix() * Vector(vert[incidentFace]) + Vector(Bb->GetPosition());
    incidentFace = incidentFace + 1 < vert.size() ? incidentFace + 1 : 0;
    v[1] = IncPoly->GetMatrix() * Vector(vert[incidentFace]) + Vector(Bb->GetPosition());
}

int Collision::Clip(const Vector& n, float c, Vector* face) {
    int sp = 0;
    Vector out[2] = {
        face[0],
        face[1]
    };

    //�������� ������ �� ����� ������ ����� �� ��
    // d = ax + by - c
    float d1 = Vector::DotProduct(n, face[0]) - c;
    float d2 = Vector::DotProduct(n, face[1]) - c;

    //���� ��'���� �������, �� ����� �� � ����� ������������. �������
    if (d1 <= 0.0f) out[sp++] = face[0];
    if (d2 <= 0.0f) out[sp++] = face[1];

    if (sp == 3)throw "Wrong points count";

    //���� ����� �� ��� ������ �� �����
    if (d1 * d2 < 0.0f) {
        //������ ����� �������� �� ������
        float alpha = d1 / (d1 - d2);
        out[sp] = face[0] + alpha * (face[1] - face[0]);
        sp++;
    }

    //�������� ������� ��������
    face[0] = out[0];
    face[1] = out[1];

    return sp;
}

