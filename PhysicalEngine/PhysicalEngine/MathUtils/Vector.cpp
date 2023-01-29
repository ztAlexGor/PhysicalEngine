#include "Vector.h"

Vector::Vector(const Vector& v)
{
	this->x = v.x;
	this->y = v.y;
}

void Vector::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}

void Vector::SetZero()
{
	x = 0.0f;
	y = 0.0f;
}

float Vector::Length() const
{
	return sqrt(x*x + y*y);
}

float Vector::SqLength() const
{
	return x * x + y * y;
}

Vector& Vector::Normalize()
{
	float length = Length();
	x /= length;
	y /= length;
	return *this;
}

Vector& Vector::operator=(Vector other)
{
	x = other.x;
	y = other.y;
	return *this;
}

bool Vector::operator==(const Vector& other)
{
	return x == other.x && y == other.y;
}

bool Vector::operator!=(const Vector& other)
{
	return !(*this==other);
}

Vector& Vector::operator-=(const Vector& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

Vector& Vector::operator+=(const Vector& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vector& Vector::operator*=(const float val)
{
	x *= val;
	y *= val;
	return *this;
}

Vector& Vector::operator/=(const float val)
{
	x /= val;
	y /= val;
	return *this;
}

//Vector Vector::operator-(const Vector& other) const
//{
//	Vector copy = *this;
//	return copy -= other;
//}
//
//Vector Vector::operator+(const Vector& other) const
//{
//	Vector copy = *this;
//	return copy += other;
//}

Vector Vector::operator*(const float val) const
{
	Vector copy = *this;
	return copy *= val;
}

Vector Vector::operator/(const float val) const
{
	Vector copy = *this;
	return copy /= val;
}

Vector operator+(const Vector a, const Vector b)
{
	Vector copy = a;
	return copy += b;
}

Vector operator-(const Vector a, const Vector b)
{
	Vector copy = a;
	return copy += b;
}
