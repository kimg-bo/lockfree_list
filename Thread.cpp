#include "Thread.h"
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <iostream>

using namespace kimgbo;

pid_t gettid()
{
	return static_cast<pid_t>(::syscall(SYS_gettid));
}

Thread::Thread(const ThreadFunc& func, const std::string& name)
		:m_started(false),
			m_pthread(0),
			m_name(name),
			m_threadid(0),
			m_threadfunc(func)
{
	
}

Thread::~Thread()
{
	std::cout << "Thread " << m_name <<" end.\n";
}
	
void Thread::start()
{
	assert(!m_started);
	m_started = true;
	if(pthread_create(&m_pthread, NULL, startThread, this) != 0)
	{
		perror("pthread_create:");
	}
}

int Thread::join()
{
	assert(m_started);
	m_started = false;
	int ret = 0;
	if((ret = pthread_join(m_pthread, NULL)) != 0)
	{
		perror("pthread_join:");
	}
	
	return ret;
}
	
void* Thread::startThread(void* obj)
{
	Thread* thread = static_cast<Thread*>(obj);
	
	if(thread != NULL)
	{
		thread->runInThread();
	}
	
	return NULL;
}

void Thread::runInThread()
{
	m_threadid = ::gettid();
	m_threadfunc();
}