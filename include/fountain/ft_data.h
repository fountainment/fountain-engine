#ifndef _FT_DATA_H_
#define _FT_DATA_H_

#include <string>
#include <vector>

//Shape
#define FT_Circle 1
#define FT_Polygon 2
#define FT_Line 3
#define FT_Rect 4

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

	ftVec2 getLB();
	ftVec2 getLT();
	ftVec2 getRT();
	ftVec2 getRB();

};

class Shape {
private:
	std::vector<ftVec2> data;
	float r;
	int n;
	bool loop;
	int type;
public:
	Shape(ftRect rct);
	Shape(float r = 0.1f);
	Shape(const std::vector<ftVec2> & a, int n, bool loop);

	const std::vector<ftVec2> & getData();
	void setData(const std::vector<ftVec2> & a);

	int getN();
	void setN(int n);

	void setR(float r);
	float getR();

	int getType();
};

class Sprite {
private:
	ftVec2 position;
	ftVec2 rectSize;
	float angle;
public:
	Shape shape;
	Sprite();
	void setPosition(ftVec2 pos);
	void setPosition(float x, float y);
	ftVec2 getPosition();
	void setAngle(float agl);
	float getAngle();
	void setRectSize(ftVec2 rts);
	ftVec2 getRectSize();
	void setRect(ftRect rct);
	ftRect getRect();
	void draw();
	void update();
};

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
};

template <typename _tp, int _size>
container<_tp, _size>::container()
{
	container<_tp, _size>::head = -1;
	container<_tp, _size>::tail = -1;
	for (int i = 0; i < _size; i++) {
		container<_tp, _size>::avail[i] = i;
	}
	container<_tp, _size>::availN = _size;
}

template <typename _tp, int _size>
_tp container<_tp, _size>::getHead()
{
	return container<_tp, _size>::list[container<_tp, _size>::head];
}

template <typename _tp, int _size>
bool container<_tp, _size>::add(_tp node)
{
	int aN = container<_tp, _size>::availN;
	int tail = container<_tp, _size>::tail;
	if (!aN)
		return false;
	int cur = container<_tp, _size>::avail[aN - 1];
	container<_tp, _size>::list[cur] = node;
	if (container<_tp, _size>::head == -1) {
		container<_tp, _size>::head = cur;
	} else {
		container<_tp, _size>::next[tail] = cur;
	}
	container<_tp, _size>::prev[cur] = tail;
	container<_tp, _size>::next[cur] = -1;
	container<_tp, _size>::tail = cur;
	container<_tp, _size>::availN--;
	return true;
}

template <typename _tp, int _size>
bool container<_tp, _size>::del(int x)
{
	if (container<_tp, _size>::head == -1) return false;
	container<_tp, _size>::avail[container<_tp, _size>::availN] = x;
	container<_tp, _size>::availN++;
	int prev = container<_tp, _size>::prev[x];
	int next = container<_tp, _size>::next[x];
	if (prev != -1)
		container<_tp, _size>::next[prev] = next;
	else
		container<_tp, _size>::head = next;
	if (next != -1)
		container<_tp, _size>::prev[next] = prev;
	else
		container<_tp, _size>::tail = prev;
	return true;
}

template <typename _tp, int _size>
bool container<_tp, _size>::delHead()
{
	return container<_tp, _size>::del(container<_tp, _size>::head);
}

template <typename _tp, int _size>
void container<_tp, _size>::update()
{
	int cur = container<_tp, _size>::head;
	while (cur != -1) {
		container<_tp, _size>::list[cur].update();
		cur = container<_tp, _size>::next[cur];
	}
}

template <typename _tp, int _size>
void container<_tp, _size>::draw()
{
	int cur = container<_tp, _size>::head;
	while (cur != -1) {
		container<_tp, _size>::list[cur].draw();
		cur = container<_tp, _size>::next[cur];
	}
}

template <typename _tp, int _size>
void container<_tp, _size>::doWith(void (*func) (_tp))
{
	int cur = container<_tp, _size>::head;
	while (cur != -1) {
		func(container<_tp, _size>::list[cur]);
		cur = container<_tp, _size>::next[cur];
	}
}

namespace fountain {

extern winState mainWin;

};

#endif
