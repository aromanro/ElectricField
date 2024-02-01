#pragma once

class PrecisionTimer
{
private:
	struct times {
		LARGE_INTEGER start;
		LARGE_INTEGER stop;
	};

	times times;

	LARGE_INTEGER freq;

	double LargeIntToSeconds(const LARGE_INTEGER & val) const;
public:
	explicit PrecisionTimer(bool start = false);

	void Start();
	void Stop();
	double GetElapsedTime() const;
};

