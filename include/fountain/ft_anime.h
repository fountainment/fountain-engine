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
	void init(ftRender::SubImagePool sip, double fps = 60.0);
	void update();
	void draw();
};

#define FT_MAXANIME 100
#define FT_MAXSIGNAL 100
#define FT_MAXLAYER 5

class AFSM : public Anime
{
private:
	Anime* anime[FT_MAXANIME];
	int curAnime[FT_MAXLAYER];
	int use[FT_MAXANIME];
	int layer[FT_MAXANIME];
	int fsm[FT_MAXANIME][FT_MAXSIGNAL];
public:
	AFSM();
	void regAnime(int index, Anime* ani, bool lp = true, int lay = 0);
	void unregAnime(int index);
	void addConnection(int a, int signal, int b);
	int getConnection(int a, int signal);
	void delConnection(int a, int signal);
	void setLoop(int index, bool loop);
	bool isLoop(int index);
	void setLayer(int index, int lay);
	int getLayer(int index);
	void inputSignal(int signal, int lay = 0);
	void update();
	void draw();
	void startWith(int index);
	void saveFSM(const char* filename);
	void loadFSM(const char* filename);
};

}

#endif
