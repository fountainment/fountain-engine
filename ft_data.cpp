#include <fountain/ft_data.h>
#include <fountain/ft_render.h>

namespace fountain {

winState mainWin;

};

//ftVec2
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

//ftVec3
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

//ftRect
ftRect::ftRect(float x, float y, float w, float h)
{
	ftRect::x = x;
	ftRect::y = y;
	ftRect::w = w;
	ftRect::h = h;
}

ftRect::ftRect()
{
	ftRect::x = 0;
	ftRect::y = 0;
	ftRect::w = 0;
	ftRect::h = 0;
}

ftRect::ftRect(ftVec2 pos, ftVec2 rSize)
{
	ftRect::x = pos.x;
	ftRect::y = pos.y;
	ftRect::w = rSize.x;
	ftRect::h = rSize.y;
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

ftVec2 ftRect::getSize()
{
	ftVec2 p(ftRect::w, ftRect::h);
	return p;
}

void ftRect::setSize(ftVec2 sz)
{
	ftRect::w = sz.x;
	ftRect::h = sz.y;
}

ftVec2 ftRect::getXY()
{
	return ftVec2(ftRect::x, ftRect::y);
}

void ftRect::setXY(ftVec2 XY)
{
	ftRect::x = XY.x;
	ftRect::y = XY.y;
}

ftVec2 ftRect::getLB()
{
	return ftVec2(ftRect::x, ftRect::y);
}

ftVec2 ftRect::getLT()
{
	return ftVec2(ftRect::x, ftRect::y + ftRect::h);
}

ftVec2 ftRect::getRT()
{
	return ftVec2(ftRect::x + ftRect::w, ftRect::y + ftRect::h);
}

ftVec2 ftRect::getRB()
{
	return ftVec2(ftRect::x + ftRect::w, ftRect::y);
}

//Shape
Shape::Shape(ftRect rct)
{
	Shape::n = 1;
	Shape::type = FT_Rect;
	Shape::loop = true;
	Shape::data.clear();
	Shape::data.push_back(rct.getSize());
}

Shape::Shape(float r)
{
	Shape::data.clear();
	Shape::setN(0);
	Shape::type = FT_Circle;
	Shape::loop = true;
	Shape::r = r;
}

Shape::Shape(const std::vector<ftVec2> & a, int n, bool loop)
{
	Shape::data.clear();
	Shape::setN(n);
	if (loop == true)
		Shape::type = FT_Polygon;
	else
		Shape::type = FT_Line;
	Shape::loop = loop;
	Shape::setData(a);
}

const std::vector<ftVec2> & Shape::getData()
{
	return Shape::data;
}

void Shape::setData(const std::vector<ftVec2> & a)
{
	Shape::data = a;
}

int Shape::getN()
{
	return Shape::n;
}

void Shape::setN(int n)
{
	Shape::n = n;
}

void Shape::setR(float r)
{
	Shape::r = r;
}

float Shape::getR()
{
	return Shape::r;
}

int Shape::getType()
{
	return Shape::type;
}

//Sprite
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

void Sprite::setRectSize(ftVec2 rts)
{
	Sprite::rectSize = rts;
}

ftVec2 Sprite::getRectSize()
{
	return Sprite::rectSize;
}

void Sprite::setRect(ftRect rct)
{
	Sprite::setPosition(rct.getCenter());
	Sprite::setRectSize(rct.getSize());
}

ftRect Sprite::getRect()
{
	ftRect rct;
	rct.setSize(Sprite::rectSize);
	rct.setCenter(Sprite::position);
	return rct;
}

void Sprite::draw()
{
	ftVec2 pos = Sprite::getPosition();
	ftRender::transformBegin();
	ftRender::ftTranslate(pos.x, pos.y);
	ftRender::drawShape(Sprite::shape, Sprite::getAngle());
	ftRender::transformEnd();
}
