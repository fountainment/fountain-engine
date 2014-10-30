#ifndef _FT_DATA_H_
#define _FT_DATA_H_

#include <string>

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
	} void setH(int hv) {
		h = hv;
		h4 = h / 4.0f;
	}

} winState;

class ftVec2 {
public:
	float x, y;
	 ftVec2();
	 ftVec2(float x, float y);
};

extern const ftVec2 operator-(const ftVec2 & a, const ftVec2 & b);

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
	ftVec2 getCenter();
	void setCenter(ftVec2 p);
	void setCenter(float x, float y);
};

template < typename _tp, int _size > class container {
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
	void add(_tp node);
	void update();
	void draw();
	void doWith(void (*func) (_tp));
};

template < typename _tp, int _size > container < _tp, _size >::container()
{
	container < _tp, _size >::head = -1;
	container < _tp, _size >::tail = -1;
	for (int i = 0; i < _size; i++) {
		container < _tp, _size >::avail[i] = i;
	}
	container < _tp, _size >::availN = _size;
}

template < typename _tp, int _size >
    void container < _tp, _size >::add(_tp node)
{
	int aN = container < _tp, _size >::availN;
	int tail = container < _tp, _size >::tail;
	if (!aN)
		return;
	int cur = container < _tp, _size >::avail[aN - 1];
	container < _tp, _size >::list[cur] = node;
	if (container < _tp, _size >::head == -1) {
		container < _tp, _size >::head = cur;
	} else {
		container < _tp, _size >::next[tail] = cur;
	}
	container < _tp, _size >::prev[cur] = tail;
	container < _tp, _size >::next[cur] = -1;
	container < _tp, _size >::tail = cur;
	container < _tp, _size >::availN--;
}

template < typename _tp, int _size > void container < _tp, _size >::update()
{
	int cur = container < _tp, _size >::head;
	while (cur != -1) {
		container < _tp, _size >::list[cur].update();
		cur = container < _tp, _size >::next[cur];
	}
}

template < typename _tp, int _size > void container < _tp, _size >::draw()
{
	int cur = container < _tp, _size >::head;
	while (cur != -1) {
		container < _tp, _size >::list[cur].draw();
		cur = container < _tp, _size >::next[cur];
	}
}

template < typename _tp, int _size >
    void container < _tp, _size >::doWith(void (*func) (_tp))
{
	int cur = container < _tp, _size >::head;
	while (cur != -1) {
		func(container < _tp, _size >::list[cur]);
		cur = container < _tp, _size >::next[cur];
	}
}

namespace fountain {
	extern winState mainWin;
};

#endif
