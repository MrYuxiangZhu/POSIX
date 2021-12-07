#include <iostream>
#include <unistd.h>
#include <pthread.h>

void* ThreadFunction(void* arg)
{
	std::cout << "run thread\n";
	sleep(5);
	pthread_exit(NULL);
}

int main()
{
	pthread_t thread_id;
	int res = 0;
	/*创建线程*/
	res = pthread_create(&thread_id, NULL, ThreadFunction, NULL);
	if (0 != res)
	{
		std::cout << "Create thread failed\n";
		return -1;
	}
	sleep(1);
	
	/*向线程发送cancel信号*/
	res = pthread_cancel(thread_id);
	if (0 != res)
	{
		std::cout << "Cancel thread failed\n";
		return -1;
	}
	
	/*获取已终止线程的返回值*/
	void* result = NULL;
	res = pthread_join(thread_id, &result);
	if (0 != res)
	{
		std::cout << "wait thread faild\n";
		return -1;
	}
	
	/*如果线程被强制终止，其返回值为PTHREAD_CANCELED*/
	if (result == PTHREAD_CANCELED)
	{
		std::cout << "terminate thread\n";
	}
	else 
	{
		std::cout << "error\n";
	}
	
	return 0;
}