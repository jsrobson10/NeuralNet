
#include "vector.hpp"

#include <cmath>

Vector::Vector() noexcept
{
	this->x = 0;
	this->y = 0;
}

Vector::Vector(double x, double y) noexcept
{
	this->x = x;
	this->y = y;
}

Vector Vector::operator+(const Vector& o) noexcept { return Vector(this->x + o.x, this->y + o.y); }
Vector Vector::operator-(const Vector& o) noexcept { return Vector(this->x - o.x, this->y - o.y); }
Vector Vector::operator*(const Vector& o) noexcept { return Vector(this->x * o.x, this->y * o.y); }
Vector Vector::operator/(const Vector& o) { return Vector(this->x / o.x, this->y / o.y); }
Vector Vector::operator+(double o) noexcept { return Vector(this->x + o, this->y + o); }
Vector Vector::operator-(double o) noexcept { return Vector(this->x - o, this->y - o); }
Vector Vector::operator*(double o) noexcept { return Vector(this->x * o, this->y * o); }
Vector Vector::operator/(double o) { return Vector(this->x / o, this->y / o); }
Vector& Vector::operator+=(const Vector& o) noexcept { this->x += o.x; this->y += o.y; return *this; }
Vector& Vector::operator-=(const Vector& o) noexcept { this->x -= o.x; this->y -= o.y; return *this; }
Vector& Vector::operator*=(const Vector& o) noexcept { this->x *= o.x; this->y *= o.y; return *this; }
Vector& Vector::operator/=(const Vector& o) { this->x /= o.x; this->y /= o.y; return *this; }
Vector& Vector::operator+=(double o) noexcept { this->x += o; this->y += o; return *this; }
Vector& Vector::operator-=(double o) noexcept { this->x -= o; this->y -= o; return *this; }
Vector& Vector::operator*=(double o) noexcept { this->x *= o; this->y *= o; return *this; }
Vector& Vector::operator/=(double o) { this->x /= o; this->y /= o; return *this; }
std::ostream& operator<<(std::ostream& o, const Vector& v) { return o << "Vector(" << v.x << ", " << v.y << ")"; }

double Vector::length() noexcept
{
	return std::sqrt(this->x * this->x + this->y * this->y);
}

Vector Vector::normalize()
{
	return *this * length();
}

Vector Vector::rotate(double r) noexcept
{
	double d = length();
	if(d == 0) return *this;
	r += std::atan2(this->y, this->x);
	return Vector(std::sin(r) * d, std::cos(r) * d);
}

Vector Vector::from_radius(double r) noexcept
{
	return Vector(std::sin(r), std::cos(r));
}

