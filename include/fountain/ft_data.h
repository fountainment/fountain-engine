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

	void setSize(int w, int h) {
		setW(w);
		setH(h);
	}

} winState;

class ftVec2
{
public:
	float x, y;
	ftVec2();
	ftVec2(float x, float y);
	void move(float x, float y);
	float length();
	float getDegree();
	const ftVec2 operator-(const ftVec2 & v) const;
	void operator-=(const ftVec2 & v);
	const ftVec2 operator+(const ftVec2 & v) const;
	void operator+=(const ftVec2 & v);
	const ftVec2 operator*(float k) const;
	void operator*=(float k);
	const ftVec2 operator/(float k) const;
	const ftVec2 operator/(const ftVec2 & v) const;
	void unitize();
	const ftVec2 getVectorVertical();
};


class ftVec3
{
public:
	float xyz[3];
	ftVec3();
	ftVec3(float x, float y, float z);
	float length();
	void unitize();
	void output(float *data);
	void operator+=(const ftVec3 & v);
	const ftVec3 operator/(float k);
	const ftVec3 crossProduct(const ftVec3 & v);
};

bool operator<(const ftVec3 & v1, const ftVec3 & v2);

class ftRect
{
private:
	float x;
	float y;
	float w;
	float h;
public:
	ftRect(float x, float y, float w, float h);
	ftRect();
	ftRect(const ftVec2 & pos, const ftVec2 & rSize);
	ftVec2 getCenter();

	void setCenter(const ftVec2 & p);
	ftVec2 getXY();
	void setXY(const ftVec2 & XY);
	ftVec2 getSize();
	void setSize(const ftVec2 & sz);

	float getX();
	float getY();
	float getW();
	float getH();

	const ftVec2 getLB();
	const ftVec2 getLT();
	const ftVec2 getRT();
	const ftVec2 getRB();
	void getFloatVertex(float *v);

	void move(float x, float y);
	void normalize();
	void inflate(float x, float y);
	bool collidePoint(const ftVec2 & p);
	bool collideRect(const ftRect & r);
	std::vector<ftVec2> collideSegment(const ftVec2 & pa, const ftVec2 & pb);
	ftVec2 distanceToPoint(const ftVec2 & p);
};

class ftShape
{
private:
	float data[64];
	float r;
	int n;
	bool loop;
	int type;

	void setN(int n);
public:
	ftShape(ftRect rct);
	ftShape(float r = 0.1f);
	ftShape(const std::vector<ftVec2> & a, int n, bool loop = true);
	~ftShape();

	const float * getData();
	void setData(const std::vector<ftVec2> & a);

	int getN();

	void setR(float r);
	float getR();

	int getType();

	static ftShape makeRegularPolygonShape(int edgeN, float r);
};

class ftColor
{
private:
	float r, g, b, a;
	float checkValue(float v);
public:
	ftColor();
	ftColor(std::string s, float a = 1.0f);
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

class ftSprite
{
private:
	ftVec2 position;
	ftVec2 rectSize;
	float angle;
	ftColor color;
	float scale;
	int tag;
public:
	bool die;
	bool enable;
	ftSprite();
	void setPosition(const ftVec2 & pos);
	void setPosition(float x, float y);
	ftVec2 getPosition();
	void setAngle(float agl);
	float getAngle();
	void setScale(float scl);
	float getScale();
	void setRectSize(const ftVec2 & rts);
	void setRectSize(float x, float y);
	ftVec2 getRectSize();
	void setRect(ftRect rct);
	ftRect getRect();
	void setColor(const ftColor & c);
	const ftColor & getColor();
	void draw();
	void update();
	void move(const ftVec2 & x);
	void move(float x, float y);
	void rotate(float aSpeed);
	void setTag(int tag);
	int getTag();
};

class ftFile
{
private:
	char name[260];
	char *str;
	char state;
	std::FILE* fp;
public:
	ftFile();
	~ftFile();
	ftFile(const char *filename);
	bool isLoad();
	void free();
	void open(const char *filename);
	bool exist();
	void close();
	void read(const char *fmt, ...);
	void write(const char *fmt, ...);
	bool load(const char *filename);
	bool reload();
	const char* getStr();
};

template <typename _tp, int _size>
class container : public ftSprite
{
private:
	int tail;
	int avail[_size];
	int availN;
	int prev[_size];

protected:
	_tp list[_size];
	int head;
	int next[_size];

public:
	container();
	_tp getHead();
	bool add(_tp node);
	bool del(int x);
	bool delHead();
	void update();
	void draw();
	void doWith(void(*func)(_tp &node));
	bool empty();
	void clear();
	int getAvailN();
	virtual bool willLive(_tp &node);
};

template <typename _tp, int _size>
container<_tp, _size>::container()
{
	clear();
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
	bool kill;
	int kn;
	while (cur != -1) {
		list[cur].update();
		kill = false;
		if (!willLive(list[cur])) {
			kill = true;
			kn = cur;
		}
		cur = next[cur];
		if (kill) del(kn);
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
void container<_tp, _size>::doWith(void (*func) (_tp &node))
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
void container<_tp, _size>::clear()
{
	head = -1;
	tail = -1;
	for (int i = 0; i < _size; i++) {
		avail[i] = i;
	}
	availN = _size;
}

template <typename _tp, int _size>
int container<_tp, _size>::getAvailN()
{
	return availN;
}

template <typename _tp, int _size>
bool container<_tp, _size>::willLive(_tp &node)
{
	return true;
}

//pointer specification
template <typename _tp, int _size>
class ptrContainer : public container<_tp, _size>
{
public:
	void update();
	void draw();
};

template <typename _tp, int _size>
void ptrContainer<_tp, _size>::update()
{
	int cur = this->head;
	bool kill;
	int kn;
	while (cur != -1) {
		this->list[cur]->update();
		kill = false;
		if (!this->willLive(this->list[cur])) {
			kill = true;
			kn = cur;
		}
		cur = this->next[cur];
		if (kill) this->del(kn);
	}
}

template <typename _tp, int _size>
void ptrContainer<_tp, _size>::draw()
{
	int cur = this->head;
	while (cur != -1) {
		this->list[cur]->draw();
		cur = this->next[cur];
	}
}

namespace fountain {

extern winState mainWin;

}

#endif
