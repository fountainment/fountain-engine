#include "vec2.h"
#include "const.h"
#include <cmath>

using fim::vec2;

vec2::vec2()
:x(0.0),
 y(0.0)
{
}

vec2::vec2(double xy)
:x(xy),
 y(xy)
{
}

vec2::vec2(double tx, double ty)
:x(tx),
 y(ty)
{
}

void vec2::move(double tx, double ty)
{
	x += tx;
	y += ty;
}

void vec2::move(const vec2 & vec)
{
	move(vec.x, vec.y);
}

const vec2 vec2::operator+(const vec2 & rhs) const
{
	vec2 res(*this);
	res.move(rhs);
	return res;
}

const vec2 vec2::operator-() const
{
	return vec2(-x, -y);
}

const vec2 vec2::operator-(const vec2 & rhs) const
{
	vec2 res(*this);
	res.move(-rhs);
	return res;
}

const vec2 vec2::operator*(double rhs) const
{
	return vec2(x * rhs, y * rhs);
}

const vec2 vec2::operator/(double rhs) const
{
	return vec2(x / rhs, y / rhs);
}

const vec2 vec2::normalize() const
{
	return (*this) / length();
}

bool vec2::operator==(const vec2 & rhs) const
{
	return (std::abs(x - rhs.x) < fim::eps) && (std::abs(y - rhs.y) < fim::eps);
}

bool vec2::operator!=(const vec2 & rhs) const
{
	return (std::abs(x - rhs.x) >= fim::eps) || (std::abs(y - rhs.y) >= fim::eps);
}

double vec2::length() const
{
	return std::sqrt(x * x + y * y);
}

double vec2::dotProduct(const vec2 & vec) const
{
	return x * vec.x + y * vec.y;
}

double vec2::crossProduct(const vec2 & vec) const
{
	return x * vec.y - vec.x * y;
}
