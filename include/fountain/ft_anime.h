#ifndef _FT_ANIME_H_
#define _FT_ANIME_H_
#include <fountain/ft_time.h>
#include <fountain/ft_render.h>

namespace ftAnime {

bool init();
void close();

class Anime
{
private:
	ftTime::Clock animeClock;
	ftTime::Clock *masterClock;
	double state;
	double curTime;
	double deltaT;
	bool loop;
public:
	ftTime::Clock* getMasterClock();
	void setMasterClock(ftTime::Clock *clock);
	void play();
	void pause();
	void resume();
	void stop();
	bool isPlay();
	void setState(double st);
	double getState();
	double getDeltaT();
	void updateTime();
	virtual void update();
	virtual void draw();
	void setLoop(bool lp);
	bool isLoop();
};

class FrameAnime : public Anime
{
private:
	ftRender::SubImagePool animeImage;
	double totalTime;
	int totalFrame;
	int curFrame;
public:
	FrameAnime();
	FrameAnime(ftRender::SubImagePool sip, double fps = 60.0);
	void update();
	void draw();
};

}

#endif
