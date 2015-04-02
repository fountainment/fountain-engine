#ifndef _FT_ANIME_H_
#define _FT_ANIME_H_
#include <fountain/ft_time.h>
#include <fountain/ft_render.h>

namespace ftAnime {

bool init();
void close();

class FrameAnime
{
private:
	ftTime::Clock animeClock;
	ftRender::SubImagePool animeImage;
	double totalTime;
	double curTime;
	double state;
	int totalFrame;
	int curFrame;
	bool loop;

public:
	FrameAnime();
	FrameAnime(ftRender::SubImagePool sip, double fps = 60.0);
	void play(ftTime::Clock *playClock = NULL);
	void pause();
	void resume();
	void stop();
	bool isPlay();
	void setState(double st);
	double getState();
	void update();
	void draw();
	void setLoop(bool lp);
};

}

#endif
