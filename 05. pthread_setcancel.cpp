#include <iostream>
#include <unistd.h>
#include <errno.h> //使用宏ESRCH
#include <pthread.h>

void* ThreadFunction(void* arg)
{
	const char* result = "thread runing";
	int res = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	if (0 != res)
	{
		std::cout << "set cancel state failed\n";
		return NULL;
	}
	
	/*recive signal, terminate*/
	res = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	if (0 != res)
	{
		std::cout << "set cancel type failed\n";
		return NULL;
	}
	
	while (1);
	
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
	
	sleep(1);
	
	res = pthread_cancel(thread_id);
	if (0 != res)
	{
		std::cout << "cancel thread failed\n";
		return 0;
	}
	
	res = pthread_join(thread_id, &result);
	if (0 != res)
	{
		std::cout << "wait thread failed\n";
	}
	
	if (PTHREAD_CANCELED == result)
	{
		std::cout << "thread be terminated\n";
	}
	else
	{
		std::cout << "error\n";
	}
	
	return 0;
}