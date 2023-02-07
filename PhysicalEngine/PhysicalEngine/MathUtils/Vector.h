#pragma once

struct Vector
{
	float x;
	float y;
	
public:
	Vector(const float x, const float y) : x(x), y(y) {};

	Vector() : Vector(0.f, 0.f) {};

	Vector(const Vector& v);

	float X() const;

	float Y() const;

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

	Vector operator-(const Vector& other) const;

	Vector operator+(const Vector& other) const;

	Vector operator*(const float val) const;

	Vector operator/(const float val) const;

	static float DotProduct(const Vector& a, const Vector& b);

	static float CrossProduct(const Vector& a, const Vector& b);

	static Vector CrossProduct(float a, const Vector& b);
};

