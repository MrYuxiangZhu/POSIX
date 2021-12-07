#include <iostream>
#include <unistd.h>
#include <errno.h> //使用宏ESRCH
#include <pthread.h>

void* ThreadFunction(void* arg)
{
	const char* result = "thread exit";
	pthread_exit((void*)result);
}

int main()
{
	int res = 0;
	pthread_t thread_id;
	void* result = NULL;
	
	/*创建线程*/
	res = pthread_create(&thread_id, NULL, ThreadFunction, NULL);
	if (0 != res)
	{
		std::cout << "Create thread\n";
		return -1;
	}
	
	/*阻塞主线程，等待子线程结束*/
	res = pthread_join(thread_id, &result);
	if (0 != res)
	{
		std::cout << "wait thread failed\n";
	}
	std::cout << "thread result: " << (char*)result << std::endl;
	
	/*尝试再次获取线程返回值*/
	res = pthread_join(thread_id, &result);
	if (ESRCH == res)
	{
		std::cout << "thread is not exist\n";
	}
	
	return 0;
}