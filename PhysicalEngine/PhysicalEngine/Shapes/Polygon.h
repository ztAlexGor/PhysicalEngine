#pragma once
#include "Shape.h"
#include <vector>

class Polygon : public Shape
{
	std::vector<Vector> vertices;
	std::vector<Vector> edges;
	std::vector<Vector> normals;
	// Vector centroid;
	float area;

public:
	Polygon(std::vector<Vector> vertices);

	Polygon(Polygon& other);

	std::vector<Vector>& GetVertices();

	std::vector<Vector>& GetEdges();

	std::vector<Vector>& GetNormals();

	virtual void InitAABB() override;

	virtual MassInfo ComputeMass(float density) const override;

	virtual Shape* Clone() const override;
	Vector GetCentroid();
private:
	

	void InitializeArea();

	// void InitializeCentroid();
};

