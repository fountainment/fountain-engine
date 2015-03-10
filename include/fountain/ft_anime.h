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
	float totalTime;
	float curTime;
	float state;
	int totalFrame;
	int curFrame;

public:
	FrameAnime();
	FrameAnime(ftRender::SubImagePool sip, float fps = 60.0f);
	void play(ftTime::Clock *playClock = NULL);
	void pause();
	void resume();
	void stop();
	bool isPlay();
	void setState(float st);
	float getState();
	void update();
	void draw();
};

}

#endif