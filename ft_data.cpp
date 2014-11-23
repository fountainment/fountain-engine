#include <fountain/ft_data.h>
#include <fountain/ft_render.h>

namespace fountain {

winState mainWin;

};

//ftVec2
ftVec2::ftVec2()
{
	x = 0;
	y = 0;
}

ftVec2::ftVec2(float x, float y)
{
	this->x = x;
	this->y = y;
}

const ftVec2 ftVec2::operator-(const ftVec2 & v)
{
	return ftVec2(x - v.x, y - v.y);
}

const ftVec2 ftVec2::operator+(const ftVec2 & v)
{
	return ftVec2(x + v.x, y + v.y);
}

//ftVec3
ftVec3::ftVec3()
{
	xyz[0] = 0;
	xyz[1] = 0;
	xyz[2] = 0;
}

ftVec3::ftVec3(float x, float y, float z)
{
	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;
}

//ftRect
ftRect::ftRect(float x, float y, float w, float h)
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

ftRect::ftRect()
{
	x = 0;
	y = 0;
	w = 0;
	h = 0;
}

ftRect::ftRect(ftVec2 pos, ftVec2 rSize)
{
	x = pos.x;
	y = pos.y;
	w = rSize.x;
	h = rSize.y;
}

ftVec2 ftRect::getCenter()
{
	ftVec2 p;
	p.x = x + w / 2.0f;
	p.y = y + h / 2.0f;
	return p;
}

void ftRect::setCenter(ftVec2 p)
{
	x = p.x - ftRect::w / 2.0f;
	y = p.y - ftRect::h / 2.0f;
}

ftVec2 ftRect::getSize()
{
	ftVec2 p(ftRect::w, ftRect::h);
	return p;
}

void ftRect::setSize(ftVec2 sz)
{
	w = sz.x;
	h = sz.y;
}

ftVec2 ftRect::getXY()
{
	return ftVec2(ftRect::x, ftRect::y);
}

void ftRect::setXY(ftVec2 XY)
{
	x = XY.x;
	y = XY.y;
}

ftVec2 ftRect::getLB()
{
	return ftVec2(x, y);
}

ftVec2 ftRect::getLT()
{
	return ftVec2(x, y + h);
}

ftVec2 ftRect::getRT()
{
	return ftVec2(x + w, y + h);
}

ftVec2 ftRect::getRB()
{
	return ftVec2(x + w, y);
}

//Shape
ftShape::ftShape(ftRect rct)
{
	n = 1;
	type = FT_Rect;
	loop = true;
	data.clear();
	data.push_back(rct.getSize());
}

ftShape::ftShape(float r)
{
	data.clear();
	setN(0);
	type = FT_Circle;
	loop = true;
	this->r = r;
}

ftShape::ftShape(const std::vector<ftVec2> & a, int n, bool loop)
{
	data.clear();
	setN(n);
	if (loop == true)
		type = FT_Polygon;
	else
		type = FT_Line;
	this->loop = loop;
	setData(a);
}

const std::vector<ftVec2> & ftShape::getData()
{
	return data;
}

void ftShape::setData(const std::vector<ftVec2> & a)
{
	data = a;
}

int ftShape::getN()
{
	return n;
}

void ftShape::setN(int n)
{
	this->n = n;
}

void ftShape::setR(float r)
{
	this->r = r;
}

float ftShape::getR()
{
	return r;
}

int ftShape::getType()
{
	return type;
}

//Sprite
ftSprite::ftSprite()
{
	position.x = 0;
	position.y = 0;
}

void ftSprite::setPosition(ftVec2 pos)
{
	position = pos;
}

void ftSprite::setPosition(float x, float y)
{
	position = ftVec2(x, y);
}

ftVec2 ftSprite::getPosition()
{
	return position;
}

void ftSprite::setAngle(float agl)
{
	agl *= 180.0f / 3.14159f;
	angle = agl;
}

float ftSprite::getAngle()
{
	return angle;
}

void ftSprite::setRectSize(ftVec2 rts)
{
	rectSize = rts;
}

ftVec2 ftSprite::getRectSize()
{
	return rectSize;
}

void ftSprite::setRect(ftRect rct)
{
	setPosition(rct.getCenter());
	setRectSize(rct.getSize());
}

ftRect ftSprite::getRect()
{
	ftRect rct;
	rct.setSize(rectSize);
	rct.setCenter(position);
	return rct;
}

void ftSprite::draw()
{
	ftVec2 pos = getPosition();
	ftRender::transformBegin();
	ftRender::ftTranslate(pos.x, pos.y);
	ftRender::drawShape(shape, getAngle());
	ftRender::transformEnd();
}
