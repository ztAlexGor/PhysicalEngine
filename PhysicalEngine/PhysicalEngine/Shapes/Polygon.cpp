#include "Polygon.h"
#include <stdexcept>
#include <numbers>


Polygon::Polygon(std::vector<Vector> listOfVertices) : Shape(EType::polygon), vertices(std::move(listOfVertices))/*, centroid(Vector(0.f, 0.f))*/
{
    if (vertices.size() < 3)throw std::logic_error("List of polygon vertices is incorrect");

    // calculate edges
    for (int i = 0; i < vertices.size() - 1; ++i)
    {
        edges.emplace_back(vertices[i + 1] - vertices[i]);
    }
    edges.emplace_back(vertices[0] - vertices[vertices.size() - 1]);

    // calculate normals
    for (const Vector& edge : edges)
    {
        normals.emplace_back(Vector(edge.y, -edge.x).Normalize());
    }

    
    // calculate area and mass center
    InitializeArea();
    // InitializeCentroid();

    // aling with centroid
    Vector centroid = GetCentroid();

    for (Vector& vertex : vertices) {
        vertex -= centroid;
    }

    InitAABB();
}


Polygon::Polygon(float width, float height) : Shape(EType::polygon)
{
    if (width <= 0 || height <= 0)throw std::logic_error("Incorrect rectangle size");

    float halfW = width / 2.f;
    float halfH = height / 2.f;


    // calculate vertices
    vertices.push_back(Vector(-halfW, -halfH));
    vertices.push_back(Vector( halfW, -halfH));
    vertices.push_back(Vector( halfW,  halfH));
    vertices.push_back(Vector(-halfW,  halfH));

    // calculate edges
    for (int i = 0; i < vertices.size() - 1; ++i)
    {
        edges.emplace_back(vertices[i + 1] - vertices[i]);
    }
    edges.emplace_back(vertices[0] - vertices[vertices.size() - 1]);

    // calculate normals
    for (const Vector& edge : edges)
    {
        normals.emplace_back(Vector(edge.y, -edge.x).Normalize());
    }

    // calculate area and AABB
    area = width * height;

    aabb = AABB(Vector(-halfW, -halfH), Vector(halfW, halfH));
}


Polygon::Polygon(Polygon& other): Polygon(other.vertices) {}


const std::vector<Vector>& Polygon::GetVertices() const
{
    return vertices;
}


const std::vector<Vector>& Polygon::GetEdges() const
{
    return edges;
}


const std::vector<Vector>& Polygon::GetNormals() const
{
    return normals;
}


void Polygon::InitAABB()
{
    Vector min(INFINITY, INFINITY);
    Vector max(-INFINITY, -INFINITY);

    for (const Vector& dot : vertices) {
        if (min.x > dot.x)min.x = dot.x;
        if (min.y > dot.y)min.y = dot.y;
        if (max.x < dot.x)max.x = dot.x;
        if (max.y < dot.y)max.y = dot.y;
    }
    aabb = AABB(min, max);
}


MassInfo Polygon::ComputeMass(float density) const
{
    float inertiaCoef = 0.0f;

    for (int i = 0; i < vertices.size(); ++i)
    {
        const Vector& p1 = vertices[i];
        const Vector& p2 = vertices[i + 1 < vertices.size() ? i + 1 : 0];

        float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
        float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
        inertiaCoef += (Vector::CrossProduct(p1, p2)) / 12.0f * (intx2 + inty2);
    }

    float mass = area * density;
    float inertia = inertiaCoef * density;
    
    MassInfo massInfo{ .mass{mass}, .invMass{1.f / mass}, .inertia{inertia}, .invInertia{1.f / inertia} };
    return massInfo;
}


Shape* Polygon::Clone() const
{
    return new Polygon(std::vector<Vector>(vertices));
}


void Polygon::Rotate(float angle)
{
    matrix.Set(angle);
}

Vector Polygon::GetSupportPoint(const Vector& normal) const
{
    double bestProjection = -INFINITY;
    Vector bestVertex;

    //��� ����� ������� ������������ ��������� �� ������� � �������� ��������
    for (const Vector& vertex : vertices)
    {
        double projection = Vector::DotProduct(vertex, normal);

        //�����'������� ������������� �������
        if (projection > bestProjection)
        {
            bestProjection = projection;
            bestVertex = vertex;
        }
    }
    return bestVertex;
}


void Polygon::InitializeArea()
{
    area = 0.0f;

    // Calculate value of shoelace formula
    int j = vertices.size() - 1;
    for (int i = 0; i < vertices.size(); ++i)
    {
        area += (vertices[j].x + vertices[i].x) * (vertices[j].y - vertices[i].y);

        j = i;
    }
    area /= -2;
}


Vector Polygon::GetCentroid() const
{
    Vector centroid(0.f, 0.f);
    for (int i = 0; i < vertices.size(); ++i)
    {
        int v1 = i;
        int v2 = i + 1 < vertices.size() ? i + 1 : 0;

        centroid += (vertices[v1] + vertices[v2]) * Vector::CrossProduct(vertices[v1], vertices[v2]);
    }                                                                                                      

    return centroid /= 6 * area;
}


//void Polygon::InitializeCentroid()
//{
//    for (int i = 0; i < vertices.size() - 1; ++i)
//    {
//        centroid.x += (vertices[i].x + vertices[i + 1].x) * (vertices[i].x * vertices[i + 1].y - vertices[i - 1].x * vertices[i].y);
//        centroid.y += (vertices[i].y + vertices[i + 1].y) * (vertices[i].x * vertices[i + 1].y - vertices[i - 1].x * vertices[i].y);
//    }
//
//    centroid /= 6 * area;
//}