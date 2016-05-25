#pragma once

class PrecisionTimer
{
protected:
	struct times {
		LARGE_INTEGER start;
		LARGE_INTEGER stop;
	} times;

	LARGE_INTEGER freq;

	double LargeIntToSeconds(const LARGE_INTEGER & val) const;
public:
	PrecisionTimer(bool start = false);

	void Start();
	void Stop();
	double GetElapsedTime() const;
};

