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

	const std::vector<Vector>& GetVertices() const;

	const std::vector<Vector>& GetEdges() const;

	const std::vector<Vector>& GetNormals() const;

	virtual void InitAABB() override;

	virtual MassInfo ComputeMass(float density) const override;

	virtual Shape* Clone() const override;

	void Rotate(float angle) override;
	
	Vector GetCentroid() const;

private:
	void InitializeArea();

	// void InitializeCentroid();
};

