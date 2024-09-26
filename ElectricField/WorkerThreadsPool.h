#pragma once

#include <vector>
#include <queue>
#include <mutex>

#include "WorkerThread.h"

template <class Job> class WorkerThreadsPool
{
	using JobWorkerThread = typename WorkerThread<WorkerThreadsPool<Job>, Job>;
	friend class JobWorkerThread;
public:
	explicit WorkerThreadsPool(int nrThreads = 4)		
	{
		if (nrThreads <= 0) nrThreads = 1;

		m_Threads.reserve(nrThreads * 2);

		for (int i = 0; i < nrThreads; ++i)
			m_Threads.emplace_back(std::make_unique<JobWorkerThread>(this, true));
	}

	~WorkerThreadsPool()
	{
		{
			std::lock_guard<std::mutex> lock(m_Mutex);

			for (std::unique_ptr<JobWorkerThread>& worker : m_Threads)
			{
				worker->SetStopUnlocked();
			}
		}
		NotifyAll();
		for (std::unique_ptr<JobWorkerThread>& worker : m_Threads)
			worker->Join();
	}


	void AddJob(const Job job)
	{
		{
			std::lock_guard<std::mutex> lock(m_Mutex);
			m_JobsQueue.push(job);
		}

		NotifyOne();
	}

	inline void NotifyOne()
	{
		m_Condition.notify_one();
	}

	inline void NotifyAll()
	{
		m_Condition.notify_all();
	}

	inline bool HasWork() const
	{
		return !m_JobsQueue.empty();
	}

	void Resize(int nrThreads)
	{
		if (nrThreads <= 0) nrThreads = 1;

		int oldSize = static_cast<int>(m_Threads.size());

		if (oldSize == nrThreads) return;

		if (oldSize < nrThreads)
		{
			for (int i = oldSize; i < nrThreads; ++i)
				m_Threads.emplace_back(std::make_unique<JobWorkerThread>(this, true));
		}
		else
		{
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
				for (int i = oldSize; i < nrThreads; ++i)
					m_Threads[i]->SetStopUnlocked();
			}
			NotifyAll();
			for (int i = oldSize; i < nrThreads; ++i)
				m_Threads[i]->Join();
			m_Threads.resize(nrThreads);
		}
	}

private:
	std::condition_variable m_Condition;
	std::mutex m_Mutex;
	std::queue<Job> m_JobsQueue;

	std::vector<std::unique_ptr<JobWorkerThread>> m_Threads;
};

