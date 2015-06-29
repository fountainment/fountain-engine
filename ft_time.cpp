#include <fountain/ft_time.h>

using ftTime::Clock;

static double curTime, lastTime;
static int nbFrames;
static double realFps;
static double msPerFrame;
static double initT;

static double mcFps = 60.0;

namespace fountain {
Clock mainClock(mcFps);
}

#ifdef __linux

// Linux
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#include <sys/time.h>
#include <unistd.h>

const double littleSleepTime = 0.000001;

inline void littleSleep()
{
	usleep(1);
}

void floatTimeInit()
{
	static struct timeval now;
	gettimeofday(&now, NULL);
	initT = (double)now.tv_sec + (double)now.tv_usec / 1000000.0;
}

inline double floatTime()
{
	static struct timeval now;
	gettimeofday(&now, NULL);
	return (double)now.tv_sec + (double)now.tv_usec / 1000000.0 - initT;
}

// Linux end
#endif

#ifdef _WIN32
// Win32
#include <time.h>
#include <windows.h>

const double littleSleepTime = 0.001;

inline void littleSleep()
{
	Sleep(1);
}

void floatTimeInit()
{
	LARGE_INTEGER tickPerSecond, tick;
	QueryPerformanceFrequency(&tickPerSecond);
	QueryPerformanceCounter(&tick);
	initT = (double)tick.QuadPart / (double)tickPerSecond.QuadPart;
}

inline double floatTime()
{
	LARGE_INTEGER tickPerSecond, tick;
	QueryPerformanceFrequency(&tickPerSecond);
	QueryPerformanceCounter(&tick);
	return (double)tick.QuadPart / (double)tickPerSecond.QuadPart - initT;
}

// Win32 end
#endif

bool ftTime::init()
{
	floatTimeInit();
	fountain::mainClock.init();
	curTime = lastTime = 0.0;
	nbFrames = 0;
	realFps = 0.0;
	return true;
}

void ftTime::initPerFrame()
{
	curTime = fountain::mainClock.getTotalT();
	nbFrames++;
	double deltaTime = curTime - lastTime;
	if (deltaTime >= 1.0) {
		realFps = nbFrames / deltaTime;
		mcFps += (60.0 - realFps) * 0.3;
		fountain::mainClock.setFps(mcFps);
		msPerFrame = deltaTime * 1000.0 / nbFrames;
		lastTime = curTime;
		nbFrames = 0;
	}
}


void ftTime::close()
{
}

double ftTime::getFps()
{
	return realFps;
}

double ftTime::getMsPerFrame()
{
	return msPerFrame;
}

double ftTime::getInitTime()
{
	return initT;
}

Clock::Clock(double fps)
{
	setFps(fps);
	slave = false;
	isPaused = true;
	deltaT = 0.0;
	timeScale = 1.0;
	totalT = 0.0;
	frameCount = 0;
}

Clock::Clock(Clock *mClock)
{
	masterClock = mClock;
	secondPerFrame = 0.0;
	perFrameWaitTime = 0.0;
	slave = true;
	isPaused = true;
	deltaT = 0.0;
	timeScale = 1.0;
	totalT = 0.0;
	frameCount = 0;
}

void Clock::init()
{
	firstT = beginT = continueT = pauseT = getCurTime();
	deltaT = 0.0;
	timeScale = 1.0;
	totalT = 0.0;
	frameCount = 0;
	isPaused = false;
}

double Clock::getCurTime()
{
	if (slave) return masterClock->getTotalT();
	else return floatTime();
}

void Clock::tick()
{
	endT = getCurTime();
	while ((deltaT = endT - beginT) < perFrameWaitTime) {
		littleSleep();
		endT = getCurTime();
	}
	if (isPaused == true)
		deltaT = 0.0;
	else {
		//TODO: use a better way to solve debug deltaT
		if (secondPerFrame > 0.0 && deltaT > secondPerFrame * 2.0) {
			deltaT = secondPerFrame;
		}
		deltaT *= timeScale;
	}
	totalT += deltaT;
	beginT = endT;
	frameCount++;
}

double Clock::getDeltaT()
{
	return deltaT;
}

double Clock::getTotalT()
{
	return totalT;
}

long long Clock::getFrameCount()
{
	return frameCount;
}

double Clock::secondsFromInit()
{
	return getCurTime() - firstT;
}

double Clock::secondsFromPause()
{
	return getCurTime() - pauseT;
}

double Clock::secondsFromContinue()
{
	return getCurTime() - continueT;
}

void Clock::pause()
{
	pauseT = getCurTime();
	isPaused = true;
}

void Clock::resume()
{
	beginT = continueT = getCurTime();
	isPaused = false;
}

bool Clock::isPause()
{
	return isPaused;
}

void Clock::setFps(double fps)
{
	if (!slave) {
		if (fps == 0.0)
			secondPerFrame = 0.0;
		else
			secondPerFrame = 1.0 / fps;
		perFrameWaitTime = secondPerFrame - littleSleepTime * 0.5;
	} else {
		masterClock->setFps(fps);
	}
}

