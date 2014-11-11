#include <fountain/ft_data.h>

namespace fountain {

winState mainWin;

};

ftVec2::ftVec2()
{
	ftVec2::x = 0;
	ftVec2::y = 0;
}

ftVec2::ftVec2(float x, float y)
{
	ftVec2::x = x;
	ftVec2::y = y;
}

const ftVec2 ftVec2::operator-(const ftVec2 & v)
{
	ftVec2 ans;
	ans.x = ftVec2::x - v.x;
	ans.y = ftVec2::y - v.y;
	return ans;
}

const ftVec2 ftVec2::operator+(const ftVec2 & v)
{
	ftVec2 ans;
	ans.x = ftVec2::x + v.x;
	ans.y = ftVec2::y + v.y;
	return ans;
}

ftVec3::ftVec3()
{
	ftVec3::xyz[0] = 0;
	ftVec3::xyz[1] = 0;
	ftVec3::xyz[2] = 0;
}

ftVec3::ftVec3(float x, float y, float z)
{
	ftVec3::xyz[0] = x;
	ftVec3::xyz[1] = y;
	ftVec3::xyz[2] = z;
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

Sprite::Sprite()
{
	Sprite::position.x = 0;
	Sprite::position.y = 0;
}

void Sprite::setPosition(ftVec2 pos)
{
	Sprite::position = pos;
}

void Sprite::setPosition(float x, float y)
{
	Sprite::position = ftVec2(x, y);
}

ftVec2 Sprite::getPosition()
{
	return Sprite::position;
}

void Sprite::setAngle(float agl)
{
	agl *= 180.0f / 3.14159f;
	Sprite::angle = agl;
}

float Sprite::getAngle()
{
	return Sprite::angle;
}

void Sprite::draw()
{
}
