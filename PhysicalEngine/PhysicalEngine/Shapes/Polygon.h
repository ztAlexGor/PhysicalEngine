#pragma once
#include "Shape.h"
#include <vector>

class Polygon : public Shape
{
	std::vector<Vector> m_vertices;
	std::vector<Vector> m_edges;
	std::vector<Vector> m_normals;
	// Vector centroid;
	float m_area;

public:
	Polygon(std::vector<Vector> vertices);

	Polygon(float width, float height);

	Polygon(Polygon& other);

	const std::vector<Vector>& GetVertices() const;

	const std::vector<Vector>& GetEdges() const;

	const std::vector<Vector>& GetNormals() const;

	virtual void InitAABB() override;

	virtual MassInfo ComputeMass(float density) const override;

	virtual Shape* Clone() const override;

	void Rotate(float angle) override;

	bool IsPointInShape(const Vector point) const override;

	Vector GetSupportPoint(const Vector& normal) const;
	
	Vector GetCentroid() const;
private:
	void InitializeArea();

	// void InitializeCentroid();
};

