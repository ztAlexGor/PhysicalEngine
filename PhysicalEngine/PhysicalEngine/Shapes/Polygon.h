#pragma once
#include "Shape.h"
#include <vector>

class Polygon : public Shape
{
	std::vector<Vector> vertices;
	std::vector<Vector> edges;
	std::vector<Vector> normals;
	Vector centroid;
	float area;

public:
	Polygon(std::vector<Vector> vertices);

	Polygon(Polygon& other);

	std::vector<Vector>& GetVertices();

	std::vector<Vector>& GetEdges();

	std::vector<Vector>& GetNormals();

	virtual AABB ComputeAABB() const override;

	virtual float ComputeMass() const override;

private:
	void InitializeArea();

	void InitializeCentroid();
};

