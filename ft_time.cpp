#include <fountain/ft_time.h>

using ftTime::Clock;

static float curTime, lastTime;
static float realFps;

namespace fountain {
Clock mainClock(60.0);
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

inline float floatTime()
{
	static struct timeval now;
	gettimeofday(&now, NULL);
	return (1000000 * now.tv_sec + now.tv_usec) / 1000000.0f;
}

// Linux end
#endif

#ifdef _WIN32
// Win32
#include <time.h>
#include <windows.h>

const float littleSleepTime = 0.001f;

inline void littleSleep()
{
	Sleep(1);
}

inline float floatTime()
{
	LARGE_INTEGER tickPerSecond, tick;
	QueryPerformanceFrequency(&tickPerSecond);
	QueryPerformanceCounter(&tick);
	return (float)tick.QuadPart / (float)tickPerSecond.QuadPart;
}

// Win32 end
#endif

bool ftTime::init()
{
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
		if (secondPerFrame > 0.0f && deltaT > secondPerFrame * 2) {
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
		perFrameWaitTime = secondPerFrame - littleSleepTime * 0.5;
	} else {
		masterClock->setFps(fps);
	}
}