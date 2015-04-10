#ifndef _FT_TIME_H_
#define _FT_TIME_H_

namespace ftTime {

bool init();
void initPerFrame();
void close();
double getFps();
double getMsPerFrame();
double getInitTime();

class Clock
{
private:
	double secondPerFrame;
	double perFrameWaitTime;
	double beginT;
	double endT;
	double deltaT;
	bool isPaused;
	double timeScale;
	double firstT;
	double pauseT;
	double continueT;
	double totalT;
	long long frameCount;
	Clock *masterClock;
	bool slave;
	double getCurTime();
public:
	explicit Clock(double fps = 0.0);
	explicit Clock(Clock *mClock);
	void init();
	void tick();
	void pause();
	void resume();
	bool isPause();
	void setFps(double fps);
	double getDeltaT();
	double getTotalT();
	long long getFrameCount();
	double secondsFromInit();
	double secondsFromPause();
	double secondsFromContinue();
};

}

namespace fountain {
extern ftTime::Clock mainClock;
}

#endif
