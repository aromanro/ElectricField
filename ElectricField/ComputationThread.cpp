#include "stdafx.h"
#include "ComputationThread.h"

#include <thread>

ComputationThread::ComputationThread()
{
}


ComputationThread::~ComputationThread()
{
}



void ComputationThread::Start()
{
	std::thread([this]() {
		Calculate(); // will delete this
	}).detach();
}