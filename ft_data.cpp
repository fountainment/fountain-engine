#include <fountain/ft_data.h>

namespace fountain {
	winState mainWin;
};

ftVec2::ftVec2()
{
}

ftVec2::ftVec2(float x, float y)
{
	ftVec2::x = x;
	ftVec2::y = y;
}

ftVec3::ftVec3()
{
}

ftVec3::ftVec3(float x, float y, float z)
{
	ftVec3::xyz[0] = x;
	ftVec3::xyz[1] = y;
	ftVec3::xyz[2] = z;
}

const ftVec2 operator-(const ftVec2 & a, const ftVec2 & b)
{
	ftVec2 ans;
	ans.x = a.x - b.x;
	ans.y = a.y - b.y;
	return ans;
}

const ftVec2 operator+(const ftVec2 & a, const ftVec2 & b)
{
	ftVec2 ans;
	ans.x = a.x + b.x;
	ans.y = a.y + b.y;
	return ans;
}

ftRect::ftRect(float x, float y, float w, float h)
{
	ftRect::x = x;
	ftRect::y = y;
	ftRect::w = w;
	ftRect::h = h;
}

ftVec2 ftRect::getCenter()
{
	ftVec2 p;
	p.x = ftRect::x + ftRect::w / 2.0f;
	p.y = ftRect::y + ftRect::h / 2.0f;
	return p;
}

void ftRect::setCenter(ftVec2 p)
{
	ftRect::x = p.x - ftRect::w / 2.0f;
	ftRect::y = p.y - ftRect::h / 2.0f;
}

void ftRect::setCenter(float x, float y)
{
	ftRect::x = x - ftRect::w / 2.0f;
	ftRect::y = y - ftRect::h / 2.0f;
}
