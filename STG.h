#ifndef _STG_H_
#define _STG_H_
#include <fountain/fountaindef.h>

namespace STG {


class Bullet : public ftSprite
{
private:
	ftVec2 speed;
	int type;
	ftColor color;
public:
	void update();
	void draw();
	void setSpeed(ftVec2 v);
	void setColor(ftColor c);
};

class BulletCon : public container<Bullet, 1000>
{
	bool willLive(Bullet & b);
};

class MainCharactor : public ftSprite
{
private:
	ftTime::Clock charClock;
	BulletCon bulletCon;
	ftVec2 speed;
	ftRender::SubImagePool animeTest;
	ftAnime::FrameAnime breath;
	ftAnime::FrameAnime *curAnime;
	void setCurAnime(ftAnime::FrameAnime *ca);
public:
	void init(ftTime::Clock *masterClock);
	void attack();
	void setSpeed(ftVec2 v);
	void update();
	void draw();
};

class Enemy : public ftSprite
{
private:
	ftVec2 speed;
	ftAnime::FrameAnime *curAnime;
	void setCurAnime(ftAnime::FrameAnime *ca);
public:
	void init();
	void attack();
	void setSpeed(ftVec2 v);
	void update();
	void draw();
};

}

#endif
