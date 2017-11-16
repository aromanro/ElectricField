#include "stdafx.h"
#include "PrecisionTimer.h"


PrecisionTimer::PrecisionTimer(bool start)
{
	times.start.QuadPart = 0;
	times.stop.QuadPart = 0;

	QueryPerformanceFrequency(&freq);

	if (start) Start();
}

double PrecisionTimer::LargeIntToSeconds(const LARGE_INTEGER & val) const
{
	return static_cast<double>(val.QuadPart) / freq.QuadPart;
}


void PrecisionTimer::Start() 
{
	QueryPerformanceCounter(&times.start);
}

void PrecisionTimer::Stop() 
{
	QueryPerformanceCounter(&times.stop);
}

double PrecisionTimer::GetElapsedTime() const
{
	LARGE_INTEGER time;
	time.QuadPart = times.stop.QuadPart - times.start.QuadPart;

	return LargeIntToSeconds(time);
}