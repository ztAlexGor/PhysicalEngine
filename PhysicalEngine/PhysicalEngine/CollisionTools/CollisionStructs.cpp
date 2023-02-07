#include "CollisionStructs.h"


CollisionPair::CollisionPair(Body& a, Body& b, CollisionManifold manifold) :
	bodyA(a), bodyB(b), manifold(manifold)
{
	props.restitution = 0.f;
	props.df = 0.f;
	props.sf = 0.f;
}


void CollisionPair::InitProperties(float time, Vector gravity)
{
    props.restitution = std::min(bodyA.GetMaterial().restitution, bodyB.GetMaterial().restitution);

    //���������� ���. ����� ������ �� ��������
    props.sf = sqrt(bodyA.GetMaterial().staticFriction * bodyB.GetMaterial().staticFriction);
    props.df = sqrt(bodyA.GetMaterial().dynamicFriction * bodyB.GetMaterial().dynamicFriction);

    for (int i = 0; i < manifold.crossPointsNumber; ++i) {
        Vector ra = manifold.crossPoint[i] - bodyA.GetPosition();
        Vector rb = manifold.crossPoint[i] - bodyB.GetPosition();

        Vector rv = bodyB.GetVelocity() + Vector::CrossProduct(bodyB.GetAngularVelocity(), rb) -
                    bodyA.GetVelocity() - Vector::CrossProduct(bodyA.GetAngularVelocity(), ra);

        //���� �� ��� �� 䳺 ����� ���� ���� ���������,
        //�� �� ������� ��������������� ��������� ������������
        if (rv.SqLength() < (gravity * time).SqLength() + 0.0001f)
            props.restitution = 0.f;

        // TODO
        if (bodyA.GetGravityScale() != 1.f || bodyB.GetGravityScale() != 1.f)throw "gravity scale doesn't considered";
    }
}
