#ifndef _FIM_VEC2_H_
#define _FIM_VEC2_H_

namespace fim {

class vec2
{
public:
	vec2();
	explicit vec2(double xy);
	vec2(double tx, double ty);
	void move(double tx, double ty);
	void move(const vec2 & vec);

	const vec2 operator+(const vec2 & rhs) const;
	const vec2 operator-() const;
	const vec2 operator-(const vec2 & rhs) const;
	const vec2 operator*(double rhs) const;
	const vec2 operator/(double rhs) const;
	const vec2 normalize() const;

	bool operator==(const vec2 & rhs) const;
	bool operator!=(const vec2 & rhs) const;

	double length() const;
	double dotProduct(const vec2 & vec) const;
	double crossProduct(const vec2 & vec) const;

	double x, y;
};

}

#endif
