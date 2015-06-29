#include "vec2.h"
#include "const.h"
#include <cmath>

fim::vec2::vec2()
:x(0.0),
 y(0.0)
{
}

fim::vec2::vec2(double xy)
:x(xy),
 y(xy)
{
}

fim::vec2::vec2(double tx, double ty)
:x(tx),
 y(ty)
{
}

void fim::vec2::move(double tx, double ty)
{
	x += tx;
	y += ty;
}

void fim::vec2::move(const vec2 & vec)
{
	move(vec.x, vec.y);
}

const fim::vec2 fim::vec2::operator+(const vec2 & rhs) const
{
	vec2 res(*this);
	res.move(rhs);
	return res;
}

const fim::vec2 fim::vec2::operator-() const
{
	return vec2(-x, -y);
}

const fim::vec2 fim::vec2::operator-(const vec2 & rhs) const
{
	vec2 res(*this);
	res.move(-rhs);
	return res;
}

const fim::vec2 fim::vec2::operator*(double rhs) const
{
	return vec2(x * rhs, y * rhs);
}

const fim::vec2 fim::vec2::operator/(double rhs) const
{
	return vec2(x / rhs, y / rhs);
}

const fim::vec2 fim::vec2::normalize() const
{
	return (*this) / length();
}

bool fim::vec2::operator==(const vec2 & rhs) const
{
	return (std::abs(x - rhs.x) < fim::eps) && (std::abs(y - rhs.y) < fim::eps);
}

bool fim::vec2::operator!=(const vec2 & rhs) const
{
	return (std::abs(x - rhs.x) >= fim::eps) || (std::abs(y - rhs.y) >= fim::eps);
}

double fim::vec2::length() const
{
	return std::sqrt(x * x + y * y);
}

double fim::vec2::dotProduct(const vec2 & vec) const
{
	return x * vec.x + y * vec.y;
}

double fim::vec2::crossProduct(const vec2 & vec) const
{
	return x * vec.y - vec.x * y;
}
