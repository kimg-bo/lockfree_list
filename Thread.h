#ifndef _THREAD_H_
#define _THREAD_H_

#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/unistd.h>
#include <functional>

namespace kimgbo
{
	class Thread
	{
	public:
		typedef std::function<void ()> ThreadFunc;
		
		explicit Thread(const ThreadFunc& func, const std::string& name = std::string());
		~Thread();
	
		void start();
		int join();
	
		bool started() const {return m_started;}
		std::string name() const {return m_name;}
		pid_t tid() const {return m_threadid;}
	
	private:
		static void* startThread(void* obj);
		void runInThread();
	
	private:
		bool m_started;
		pthread_t m_pthread;
		std::string m_name;
		pid_t m_threadid;
		ThreadFunc m_threadfunc;
	};
}

#endif //_THREAD_H_