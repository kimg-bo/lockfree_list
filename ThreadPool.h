#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "lockfree_list.h"
#include "Thread.h"
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <functional>
#include <string>
#include <vector>
#include <iostream>

namespace kimgbo
{
	template<typename T>
	class ThreadPool
	{
	public:
		ThreadPool(const int tasks, const std::string& name = std::string()): m_name(name), m_runing(false), m_Task(tasks)
		{
		}
		
		~ThreadPool()
		{
			if(m_runing)
			{
				stop();
			}
		}
	
		void start(int threadnum)
		{
			assert(m_threads.empty());
			m_runing = true;
			
			for(int i=0; i<threadnum; i++)
			{
				char id[32];
				memset(id, 0, sizeof(id));
				snprintf(id, sizeof(id), "%d", i);
				Thread thread(std::bind(&ThreadPool::runInThread, this), id);
				m_threads.push_back(thread);
				m_threads[i].start();
			}
		}
		
		void stop()
		{
			m_runing = false;
			
			for(int i=0; i<m_threads.size(); i++)
			{
				m_threads[i].join();
			}
		}
		
		void run(const T& task)
		{
			m_Task.EnQueue(task);
		}
		
	private:
		void runInThread()
		{
			while(m_runing)
			{
				T task = take();
				if(task > 0)
				{
					execute(task);
				}
			}
		}
		
		T take()
		{
			T task = m_Task.DeQueue();
			
			return task;
		}
		
		void execute(const T& task)
		{
			std::cout << "execute " << task <<"...\n";
		}
		
private:
		std::string m_name;
		bool m_runing;
		LockFreeQueue<T> m_Task;
		std::vector<Thread> m_threads;
	};
}

#endif //_THREADPOOL_H_