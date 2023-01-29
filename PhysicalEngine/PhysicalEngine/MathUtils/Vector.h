#pragma once

#include <cmath>

struct Vector
{
	float x;
	float y;
	
public:
	Vector(const float x, const float y) : x(x), y(y) {};

	Vector() : Vector(0.0, 0.0) {};

	Vector(const Vector& v);

	void Set(float x, float y);

	void SetZero();

	float Length() const;

	float SqLength() const;

	Vector& Normalize();

	Vector& operator=(Vector other);

	bool operator==(const Vector& other);

	bool operator!=(const Vector& other);

	Vector& operator-=(const Vector& other);

	Vector& operator+=(const Vector& other);

	Vector& operator*=(const float val);

	Vector& operator/=(const float val);

	//Vector operator-(const Vector& other) const;

	//Vector operator+(const Vector& other) const;

	Vector operator*(const float val) const;

	Vector operator/(const float val) const;

	friend Vector operator+(const Vector a, const Vector b);

	friend Vector operator-(const Vector a, const Vector b);
};

