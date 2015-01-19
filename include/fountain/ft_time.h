#ifndef _FT_TIME_H_
#define _FT_TIME_H_

namespace ftTime {

bool init();
void initPerFrame();
void close();
float getFps();

class Clock
{
private:
	float secondPerFrame;
	float perFrameWaitTime;
	float beginT;
	float endT;
	float deltaT;
	bool isPaused;
	float timeScale;
	float firstT;
	float pauseT;
	float continueT;
	float totalT;
	long long frameCount;
	Clock *masterClock;
	bool slave;
	float getCurTime();
public:
	explicit Clock(float fps = 0.0);
	explicit Clock(Clock *mClock);
	void init();
	void tick();
	void pause();
	void resume();
	bool isPause();
	void setFps(float fps);
	float getDeltaT();
	float getTotalT();
	long long getFrameCount();
	float secondsFromInit();
	float secondsFromPause();
	float secondsFromContinue();
};

}

namespace fountain {
extern ftTime::Clock mainClock;
}

#endif
