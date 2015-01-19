#include <fountain/ft_time.h>

using ftTime::Clock;

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
	return true;
}

void ftTime::close()
{
}

Clock::Clock(float fps)
{
	if (fps == 0.0)
		secondPerFrame = 0.0;
	else
		secondPerFrame = 1.0 / fps;
	isPaused = true;
	slave = false;
}

Clock::Clock(Clock *mClock)
{
	masterClock = mClock;
	secondPerFrame = 0.0;
	isPaused = true;
	slave = true;
}

void Clock::init()
{
	firstT = beginT = continueT = pauseT = getCurTime();
	deltaT = 0.0;
	timeScale = 1.0;
	totalT = 0;
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
	while ((deltaT = endT - beginT) < secondPerFrame) {
		littleSleep();
		endT = getCurTime();
	}
	//TODO: use a better way to solve debug deltaT
	if (isPaused == true)
		deltaT = 0.0;
	else {
		if (secondPerFrame != 0 && deltaT > secondPerFrame * 2) {
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
