#include "ThreadPool.h"
#include "ConnObj.h"

using namespace kimgbo;

int main(int argc, char* argv[])
{
	ThreadPool<int> threadpool(100);
	threadpool.start(1);
	
	for(int i=0; i<100000; i++)
	{
		threadpool.run(i);
	}
	
	sleep(1);
	
	threadpool.stop();
	
	return 0;
}