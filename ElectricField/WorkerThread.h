#pragma once

#include <atomic>
#include <thread>

template<class WorkerThreadsPool, class Job> class WorkerThread
{
public:
	explicit WorkerThread(WorkerThreadsPool *threadsPool, bool autostart = false)
		: m_ThreadsPool(threadsPool), m_Stop(false)
	{
		if (autostart) Start();
	}

	~WorkerThread()
	{
		Stop();
	}

	void Start()
	{
		{
			std::lock_guard<std::mutex> lock(m_ThreadsPool->m_Mutex);
			m_Stop = false;
		}
		m_Thread = std::thread(&WorkerThread::Calculate, this);
	}

	void Stop()
	{
		bool notify = false;
		{
			std::lock_guard<std::mutex> lock(m_ThreadsPool->m_Mutex);
			if (!m_Stop)
			{
				m_Stop = true;
				notify = true;
			}
		}
		if (notify) m_ThreadsPool->NotifyAll();
		Join();
	}

	void SetStopUnlocked()
	{
		m_Stop = true;
	}

	inline void Join()
	{
		if (m_Thread.joinable()) m_Thread.join();
	}

private:
	inline bool TerminateWait() const
	{
		return m_ThreadsPool->HasWork() || m_Stop;
	}

	void Calculate()
	{
		for (;;)
		{
			std::unique_lock<std::mutex> lock(m_ThreadsPool->m_Mutex);

			if (!TerminateWait())
				m_ThreadsPool->m_Condition.wait(lock, [this] { return TerminateWait(); });

			while (!m_ThreadsPool->m_JobsQueue.empty() && !m_Stop)
			{
				Job job = m_ThreadsPool->m_JobsQueue.front();
				m_ThreadsPool->m_JobsQueue.pop();
				lock.unlock();

				job->Calculate();

				lock.lock();
			} 

			if (m_Stop) break;
		}
	}

	WorkerThreadsPool* m_ThreadsPool;

	std::thread m_Thread;

	bool m_Stop;
};

