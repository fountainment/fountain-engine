#ifndef _FT_TIME_H_
#define _FT_TIME_H_

namespace ftTime {

bool init();
void close();

class Clock
{
private:
	float secondPerFrame;
	bool isPaused;
	float firstT;
	float beginT;
	float pauseT;
	float continueT;
	float endT;
	float deltaT;
	float timeScale;
	//TODO: use frameCount to calculate real fps
	long long frameCount;
	//TODO: use totalT to add slave mode
	float totalT;
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
	float getDeltaT();
	float getTotalT();
	float secondsFromInit();
	float secondsFromPause();
	float secondsFromContinue();
};

}

namespace fountain {
extern ftTime::Clock mainClock;
}

#endif
