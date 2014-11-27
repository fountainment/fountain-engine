#ifndef _FTTIME_H_
#define _FTTIME_H_
namespace ftTime {
void init();

class Clock {
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
public:
	explicit Clock(double fps = 0.0);
	void init();
	void tick();
	void Pause();
	void Continue();
	bool isPause();
	double getDeltaT();
	double secondsFromInit();
	double secondsFromPause();
	double secondsFromContinue();
};
}
#endif
