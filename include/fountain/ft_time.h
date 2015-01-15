#ifndef _FT_TIME_H_
#define _FT_TIME_H_

namespace ftTime {

bool init();
void close();

class Clock
{
private:
	double secondPerFrame;
	bool isPaused;
	double firstT;
	double beginT;
	double pauseT;
	double continueT;
	double endT;
	double deltaT;
	double timeScale;
	//TODO: use frameCount to calculate real fps
	long long frameCount;
	//TODO: use totalT to add slave mode
	double totalT;
public:
	explicit Clock(double fps = 0.0);
	void init();
	void tick();
	void pause();
	void resume();
	bool isPause();
	double getDeltaT();
	double secondsFromInit();
	double secondsFromPause();
	double secondsFromContinue();
};

}

namespace fountain {
extern ftTime::Clock mainClock;
}

#endif
