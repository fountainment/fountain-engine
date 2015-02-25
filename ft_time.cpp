#include <fountain/ft_time.h>

using ftTime::Clock;

static float curTime, lastTime;
static float realFps;
static double initT;

namespace fountain {
Clock mainClock(60.0f);
}

#ifdef linux

// Linux
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#include <sys/time.h>
#include <unistd.h>

const float littleSleepTime = 0.000001f;

inline void littleSleep()
{
	usleep(1);
}

void floatTimeInit()
{
	static struct timeval now;
	gettimeofday(&now, NULL);
	initT = (1000000 * now.tv_sec + now.tv_usec) / 1000000.0;
}

inline float floatTime()
{
	static struct timeval now;
	gettimeofday(&now, NULL);
	return (float)(((1000000 * now.tv_sec + now.tv_usec) / 1000000.0) - initT);
}

// Linux end
#endif

#ifdef _WIN32
// Win32
#include <time.h>
#include <windows.h>

const float littleSleepTime = 0.001f;
static double freq = 1.0;

inline void littleSleep()
{
	Sleep(1);
}

void floatTimeInit()
{
	LARGE_INTEGER tickPerSecond, tick;
	QueryPerformanceFrequency(&tickPerSecond);
	QueryPerformanceCounter(&tick);
	freq = 1.0 / (double)tickPerSecond.QuadPart;
	initT = (double)tick.QuadPart * freq;
}

inline float floatTime()
{
	LARGE_INTEGER tick;
	QueryPerformanceCounter(&tick);
	return (float)((double)tick.QuadPart * freq - initT);
}

// Win32 end
#endif

bool ftTime::init()
{
	floatTimeInit();
	fountain::mainClock.init();
	curTime = lastTime = 0.0f;
	realFps = 0.0f;
	return true;
}

void ftTime::initPerFrame()
{
	if (fountain::mainClock.getFrameCount() % 128 == 0) {
		curTime = fountain::mainClock.getTotalT();
		float deltaTime = curTime - lastTime;
		if (deltaTime > littleSleepTime)
			realFps = 128.0f / deltaTime;
		lastTime = curTime;
	}
}


void ftTime::close()
{
}

float ftTime::getFps()
{
	return realFps;
}

Clock::Clock(float fps)
{
	setFps(fps);
	isPaused = true;
	slave = false;
}

Clock::Clock(Clock *mClock)
{
	masterClock = mClock;
	secondPerFrame = 0.0f;
	perFrameWaitTime = 0.0f;
	isPaused = true;
	slave = true;
}

void Clock::init()
{
	firstT = beginT = continueT = pauseT = getCurTime();
	deltaT = 0.0f;
	timeScale = 1.0f;
	totalT = 0.0f;
	frameCount = 0;
	isPaused = false;
}

float Clock::getCurTime()
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
		deltaT = 0.0f;
	else {
		//TODO: use a better way to solve debug deltaT
		if (secondPerFrame > 0.0f && deltaT > secondPerFrame * 2.0f) {
			deltaT = secondPerFrame;
		}
		deltaT *= timeScale;
	}
	totalT += deltaT;
	beginT = endT;
	frameCount++;
}

float Clock::getDeltaT()
{
	return deltaT;
}

float Clock::getTotalT()
{
	return totalT;
}

long long Clock::getFrameCount()
{
	return frameCount;
}

float Clock::secondsFromInit()
{
	return getCurTime() - firstT;
}

float Clock::secondsFromPause()
{
	return getCurTime() - pauseT;
}

float Clock::secondsFromContinue()
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

void Clock::setFps(float fps)
{
	if (!slave) {
		if (fps == 0.0f)
			secondPerFrame = 0.0f;
		else
			secondPerFrame = 1.0f / fps;
		perFrameWaitTime = secondPerFrame - littleSleepTime * 0.5f;
	} else {
		masterClock->setFps(fps);
	}
}
