#include <fountain/ft_time.h>

static double allStartT = 0.0;

#ifndef _WIN32

// Linux
#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif
#include <sys/time.h>
#include <unistd.h>
void littleSleep()
{
	usleep(1);
}

double floatTime()
{
	static struct timeval now;
	gettimeofday(&now, NULL);
	return (1000000 * now.tv_sec + now.tv_usec) / 1000000.0 - allStartT;
}

// Linux end

#else

// Win32
#include <time.h>
#include <windows.h>
void littleSleep()
{
	Sleep(1);
}

double floatTime()
{
	LARGE_INTEGER tickPerSecond, tick;
	QueryPerformanceFrequency(&tickPerSecond);
	QueryPerformanceCounter(&tick);
	return tick.QuadPart / (double)tickPerSecond.QuadPart - allStartT;
}

// Win32 end

#endif

void ftTime::init()
{
	allStartT = 0.0;
	allStartT = floatTime();
}

using namespace ftTime;

Clock::Clock(double fps)
{
	if (fps == 0.0)
		Clock::secondPerFrame = 0.0;
	else
		Clock::secondPerFrame = 1.0 / fps;
	Clock::isPaused = 1;
	Clock::deltaT = 0.0;
	Clock::timeScale = 1.0;
}

void Clock::init()
{
	Clock::firstT = Clock::beginT = Clock::continueT = Clock::pauseT =
	    floatTime();
	Clock::isPaused = 0;
}

void Clock::tick()
{
	Clock::endT = floatTime();
	while ((Clock::deltaT =
		Clock::endT - Clock::beginT) < Clock::secondPerFrame) {
		littleSleep();
		Clock::endT = floatTime();
	}
	if (Clock::secondPerFrame != 0 && Clock::deltaT > Clock::secondPerFrame)
		Clock::deltaT = Clock::secondPerFrame;
	Clock::deltaT *= Clock::timeScale;
	if (Clock::isPaused == 1)
		Clock::deltaT = 0.0;
	Clock::beginT = Clock::endT;
}

double Clock::getDeltaT()
{
	return Clock::deltaT;
}

double Clock::secondsFromInit()
{
	return floatTime() - Clock::firstT;
}

double Clock::secondsFromPause()
{
	return floatTime() - Clock::pauseT;
}

double Clock::secondsFromContinue()
{
	return floatTime() - Clock::continueT;
}

void Clock::Pause()
{
	Clock::pauseT = floatTime();
	Clock::isPaused = 1;
}

void Clock::Continue()
{
	Clock::beginT = Clock::continueT = floatTime();
	Clock::isPaused = 0;
}
