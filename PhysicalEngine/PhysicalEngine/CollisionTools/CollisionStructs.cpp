#include "CollisionStructs.h"


CollisionPair::CollisionPair(Body& a, Body& b, CollisionManifold manifold) :
	bodyA(a), bodyB(b), manifold(manifold)
{
	props.restitution = 0.f;
	props.df = 0.f;
	props.sf = 0.f;
}

Body& CollisionPair::GetBodyA()
{
    return bodyA;
}

Body& CollisionPair::GetBodyB()
{
    return bodyB;
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



void CollisionPair::FixCollision(bool isFrictionEnable)
{
    //���� ������ ��� �������, �� ������� �� ����������
    if (bodyA.IsStatic() && bodyB.IsStatic()) {
        bodyA.SetVelocity(Vector(0.f, 0.f));
        bodyB.SetVelocity(Vector(0.f, 0.f));
        return;
    }

    //��� ����� ����� ��������
    for (int i = 0; i < manifold.crossPointsNumber; ++i) {
        Vector ra = manifold.crossPoint[i] - bodyA.GetPosition();
        Vector rb = manifold.crossPoint[i] - bodyB.GetPosition();

        //������ �������� B ������� �
        Vector rv = bodyB.GetVelocity() + Vector::CrossProduct(bodyB.GetAngularVelocity(), rb) -
                    bodyA.GetVelocity() - Vector::CrossProduct(bodyA.GetAngularVelocity(), ra);


        //�������� B ������� � ������ ������ ��������
        float contactVel = Vector::DotProduct(rv, manifold.normal);

        //���� ��� ��������� � ����� ���������, �� �� ������� ����� ��������
        if (contactVel > 0)
        {
            return;
        }
        //else if (contactVel == 0)
        //{
        //    contactVel = manifold.depth;
        //}

        float raCrossN = Vector::CrossProduct(ra, manifold.normal);
        float rbCrossN = Vector::CrossProduct(rb, manifold.normal);

        float invMassSum = bodyA.GetMassInfo().invMass + raCrossN * raCrossN * bodyA.GetMassInfo().invInertia
                         + bodyB.GetMassInfo().invMass + rbCrossN * rbCrossN * bodyB.GetMassInfo().invInertia;

        //���������� �������� ��������
        float j = -(1.0f + props.restitution) * contactVel;
        j /= invMassSum;
        j /= (float)manifold.crossPointsNumber;

        //����������� ������� �� ��� A �� B
        Vector impulse = manifold.normal * j;
        bodyA.ApplyImpulse(-impulse, ra);
        bodyB.ApplyImpulse( impulse, rb);

        //��������� ���� �����
        if (isFrictionEnable) {
            rv = bodyB.GetVelocity() + Vector::CrossProduct(bodyB.GetAngularVelocity(), rb) -
                 bodyA.GetVelocity() - Vector::CrossProduct(bodyA.GetAngularVelocity(), ra);

            //�������� ��������������� ��������
            Vector t = rv - (manifold.normal * Vector::DotProduct(rv, manifold.normal));
            t.Normalize();

            //�������� ��������������� ��������
            float jt = -Vector::DotProduct(rv, t);
            jt /= invMassSum;
            jt /= (float)manifold.crossPointsNumber;

            //���� �������� �������� ���� ����, �� ������ �������� ��
            if (abs(jt) < 0.000001)return;

            //� ��������� �� ���� ����� ������� ���� �����
            Vector tangentImpulse;
            if (abs(jt) < j * props.sf)
                tangentImpulse = t * jt;
            else
                tangentImpulse = t * (-j) * props.df;

            //����������� ������� �����
            bodyA.ApplyImpulse(-tangentImpulse, ra);
            bodyB.ApplyImpulse( tangentImpulse, rb);
        }
    }
}

void CollisionPair::PositionalCorrection()
{
    const float k_slop = 0.08f; //������ ����������� (0.01; 0.1)
    const float percent = 0.5f; //������� �����������, ��� �������� (0.2; 0.8)

    float t = manifold.depth - k_slop;
    if (t > 0) {
        Vector correction = (t / (bodyA.GetMassInfo().invMass + bodyB.GetMassInfo().invMass)) * manifold.normal * percent;

        if (!bodyA.IsStatic())bodyA.SetPosition(Vector(bodyA.GetPosition()) - correction * bodyA.GetMassInfo().invMass);
        if (!bodyB.IsStatic())bodyB.SetPosition(Vector(bodyB.GetPosition()) + correction * bodyB.GetMassInfo().invMass);
    }
}

