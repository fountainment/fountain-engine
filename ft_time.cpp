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

inline double floatTime()
{
	static struct timeval now;
	gettimeofday(&now, NULL);
	return (1000000 * now.tv_sec + now.tv_usec) / 1000000.0;
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

inline double floatTime()
{
	LARGE_INTEGER tickPerSecond, tick;
	QueryPerformanceFrequency(&tickPerSecond);
	QueryPerformanceCounter(&tick);
	return (double)tick.QuadPart / (double)tickPerSecond.QuadPart;
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

Clock::Clock(double fps)
{
	if (fps == 0.0)
		secondPerFrame = 0.0;
	else
		secondPerFrame = 1.0 / fps;
	isPaused = true;
	deltaT = 0.0;
	timeScale = 1.0;
	totalT = 0;
	frameCount = 0;
}

void Clock::init()
{
	firstT = beginT = continueT = pauseT = floatTime();
	isPaused = false;
}

void Clock::tick()
{
	endT = floatTime();
	while ((deltaT = endT - beginT) < secondPerFrame) {
		littleSleep();
		endT = floatTime();
	}
	//TODO: use a better way to solve debug deltaT
	if (secondPerFrame != 0 && deltaT > secondPerFrame * 2)
		deltaT = secondPerFrame;
	deltaT *= timeScale;
	if (isPaused == true)
		deltaT = 0.0;
	totalT += deltaT;
	beginT = endT;
	frameCount++;
}

double Clock::getDeltaT()
{
	return deltaT;
}

double Clock::secondsFromInit()
{
	return floatTime() - firstT;
}

double Clock::secondsFromPause()
{
	return floatTime() - pauseT;
}

double Clock::secondsFromContinue()
{
	return floatTime() - continueT;
}

void Clock::Pause()
{
	pauseT = floatTime();
	isPaused = true;
}

void Clock::Continue()
{
	beginT = continueT = floatTime();
	isPaused = false;
}

bool Clock::isPause()
{
	return isPaused;
}
