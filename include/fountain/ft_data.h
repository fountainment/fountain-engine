#ifndef _FT_DATA_H_
#define _FT_DATA_H_

#include <string>
#include <vector>

//ftShape::type
#define FT_Circle 1
#define FT_Polygon 2
#define FT_Line 3
#define FT_Rect 4

//ftColor
#define FT_Red    (ftColor(1.0f,0.0f,0.0f))
#define FT_Green  (ftColor(0.0f,1.0f,0.0f))
#define FT_Blue   (ftColor(0.0f,0.0f,1.0f))
#define FT_Grey   (ftColor(0.5f,0.5f,0.5f))
#define FT_White  (ftColor(1.0f,1.0f,1.0f))
#define FT_Black  (ftColor(0.0f,0.0f,0.0f))
#define FT_Yellow (ftColor(1.0f,1.0f,0.0f))
#define FT_Orange (ftColor(1.0f,0.5f,0.0f))

typedef struct {
	int w, h;
	float w4, h4;
	std::string title;
	std::string icon;
	bool isFullScreen;
	bool hideCursor;

	void setW(int wv) {
		w = wv;
		w4 = w / 4.0f;
	}

	void setH(int hv) {
		h = hv;
		h4 = h / 4.0f;
	}

} winState;

class ftVec2 {
public:
	float x, y;
	ftVec2();
	ftVec2(float x, float y);
	const ftVec2 operator-(const ftVec2 & v);
	const ftVec2 operator+(const ftVec2 & v);
	const ftVec2 operator*(float k);
	const ftVec2 operator/(float k);
};


class ftVec3 {
public:
	float xyz[3];
	ftVec3();
	ftVec3(float x, float y, float z);
};

class ftRect {
private:
	float x;
	float y;
	float w;
	float h;
public:
	ftRect(float x, float y, float w, float h);
	ftRect();
	ftRect(ftVec2 pos, ftVec2 rSize);
	ftVec2 getCenter();

	void setCenter(ftVec2 p);
	ftVec2 getXY();
	void setXY(ftVec2 XY);
	ftVec2 getSize();
	void setSize(ftVec2 sz);

	float getX();
	float getY();
	float getW();
	float getH();

	ftVec2 getLB();
	ftVec2 getLT();
	ftVec2 getRT();
	ftVec2 getRB();
	void getFloatVertex(float *v);

	void move(float x, float y);
	void normalize();
	void inflate(float x, float y);
	bool collidePoint(ftVec2 p);
};

class ftShape {
private:
	float data[32];
	float r;
	int n;
	bool loop;
	int type;

	void setN(int n);
public:
	ftShape(ftRect rct);
	//ftShape(const ftShape & shape);
	~ftShape();
	ftShape(float r = 0.1f);
	ftShape(const std::vector<ftVec2> & a, int n, bool loop = true);

	const float * getData();
	void setData(const std::vector<ftVec2> & a);

	int getN();

	void setR(float r);
	float getR();

	int getType();
};

class ftColor
{
private:
	float r, g, b, a;
	float checkValue(float v);
public:
	ftColor();
	ftColor(std::string);
	ftColor(float r, float g, float b, float a = 1.0f);
	void inverse();
	void setR(float r);
	void setG(float g);
	void setB(float b);
	void setAlpha(float a);
	float getR();
	float getG();
	float getB();
	float getAlpha();
};

class ftSprite {
private:
	ftVec2 position;
	ftVec2 rectSize;
	float angle;
	ftColor color;
public:
	ftShape shape;
	ftSprite();
	void setPosition(ftVec2 pos);
	void setPosition(float x, float y);
	ftVec2 getPosition();
	void setAngle(float agl);
	float getAngle();
	void setRectSize(ftVec2 rts);
	ftVec2 getRectSize();
	void setRect(ftRect rct);
	ftRect getRect();
	void setColor(const ftColor & c);
	void setShape(const ftShape & shape);
	const ftColor & getColor();
	void draw();
	void update();
};

class ftFile
{
private:
	char name[260];
	char *str;
public:
	ftFile();
	~ftFile();
	ftFile(const char *filename);
	bool isLoad();
	void free();
	bool load(const char *filename);
	bool reload();
	const char* getStr();
};

//TODO: add bool container::willLive(_tp & node)
template <typename _tp, int _size>
class container {
private:
	_tp list[_size];
	int head;
	int tail;
	int avail[_size];
	int availN;
	int prev[_size];
	int next[_size];
public:
	container();
	_tp getHead();
	bool add(_tp node);
	bool del(int x);
	bool delHead();
	void update();
	void draw();
	void doWith(void(*func)(_tp));
	bool empty();
	int getAvailN();
};

template <typename _tp, int _size>
container<_tp, _size>::container()
{
	head = -1;
	tail = -1;
	for (int i = 0; i < _size; i++) {
		avail[i] = i;
	}
	availN = _size;
}

template <typename _tp, int _size>
_tp container<_tp, _size>::getHead()
{
	return list[head];
}

template <typename _tp, int _size>
bool container<_tp, _size>::add(_tp node)
{
	int aN = availN;
	int tailT = tail;
	if (!aN)
		return false;
	int cur = avail[aN - 1];
	list[cur] = node;
	if (head == -1) {
		head = cur;
	} else {
		next[tailT] = cur;
	}
	prev[cur] = tailT;
	next[cur] = -1;
	tail = cur;
	availN--;
	return true;
}

template <typename _tp, int _size>
bool container<_tp, _size>::del(int x)
{
	if (head == -1) return false;
	avail[availN] = x;
	availN++;
	int prevT = prev[x];
	int nextT = next[x];
	if (prevT != -1)
		next[prevT] = nextT;
	else
		head = nextT;
	if (nextT != -1)
		prev[nextT] = prevT;
	else
		tail = prevT;
	return true;
}

template <typename _tp, int _size>
bool container<_tp, _size>::delHead()
{
	return del(head);
}

template <typename _tp, int _size>
void container<_tp, _size>::update()
{
	int cur = head;
	while (cur != -1) {
		list[cur].update();
		cur = next[cur];
	}
}

template <typename _tp, int _size>
void container<_tp, _size>::draw()
{
	int cur = head;
	while (cur != -1) {
		list[cur].draw();
		cur = next[cur];
	}
}

template <typename _tp, int _size>
void container<_tp, _size>::doWith(void (*func) (_tp))
{
	int cur = head;
	while (cur != -1) {
		func(list[cur]);
		cur = next[cur];
	}
}

template <typename _tp, int _size>
bool container<_tp, _size>::empty()
{
	if (_size == availN)
		return true;
	else
		return false;
}

template <typename _tp, int _size>
int container<_tp, _size>::getAvailN()
{
	return availN;
}

namespace fountain {

extern winState mainWin;

};

#endif
