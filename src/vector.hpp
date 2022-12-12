
#pragma once

class Vector;

#include <ostream>

class Vector
{
public:

	double x, y;

	Vector() noexcept;
	Vector(double x, double y) noexcept;
	Vector rotate(double r) noexcept;
	Vector normalize();
	double length() noexcept;
	
	Vector operator+(const Vector& o) noexcept;
	Vector operator-(const Vector& o) noexcept;
	Vector operator*(const Vector& o) noexcept;
	Vector operator/(const Vector& o);
	Vector operator+(double o) noexcept;
	Vector operator-(double o) noexcept;
	Vector operator*(double o) noexcept;
	Vector operator/(double o);
	Vector& operator+=(const Vector& o) noexcept;
	Vector& operator-=(const Vector& o) noexcept;
	Vector& operator*=(const Vector& o) noexcept;
	Vector& operator/=(const Vector& o);
	Vector& operator+=(double o) noexcept;
	Vector& operator-=(double o) noexcept;
	Vector& operator*=(double o) noexcept;
	Vector& operator/=(double o);

	friend std::ostream& operator<<(std::ostream& o, const Vector&);
	static Vector from_radius(double r) noexcept;
};

