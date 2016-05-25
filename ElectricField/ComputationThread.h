#pragma once

class ComputationThread
{
public:
	ComputationThread();

	void Start();

protected:
	virtual ~ComputationThread();

	virtual void Calculate() = 0;
};

