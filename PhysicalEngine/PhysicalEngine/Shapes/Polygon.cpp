#include "Polygon.h"
#include <stdexcept>


Polygon::Polygon(std::vector<Vector> listOfVertices) : Shape(EType::polygon), vertices(std::move(listOfVertices)), centroid(Vector(0.f, 0.f))
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
    InitializeCentroid();
}


Polygon::Polygon(Polygon& other): Polygon(other.vertices) {}


std::vector<Vector>& Polygon::GetVertices()
{
    return vertices;
}


std::vector<Vector>& Polygon::GetEdges()
{
    return edges;
}


std::vector<Vector>& Polygon::GetNormals()
{
    return normals;
}


AABB Polygon::ComputeAABB() const
{
    Vector min(INFINITY, INFINITY);
    Vector max(-INFINITY, -INFINITY);

    for (const Vector& dot : vertices) {
        if (min.x > dot.x)min.x = dot.x;
        if (min.y > dot.y)min.y = dot.y;
        if (max.x < dot.x)max.x = dot.x;
        if (max.y < dot.y)max.y = dot.y;
    }
    return AABB(min, max);
}


//void Polygon::ComputeMass(MassInfo ) const
//{
//
//}


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
}

void Polygon::InitializeCentroid()
{
    for (int i = 0; i < vertices.size() - 1; ++i)
    {
        centroid.x += (vertices[i].x + vertices[i + 1].x) * (vertices[i].x * vertices[i + 1].y - vertices[i - 1].x * vertices[i].y);
        centroid.y += (vertices[i].y + vertices[i + 1].y) * (vertices[i].x * vertices[i + 1].y - vertices[i - 1].x * vertices[i].y);
    }

    centroid /= 6 * area;
}


