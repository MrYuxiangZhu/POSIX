#include <iostream>
#include <unistd.h>
#include <errno.h> //使用宏ESRCH
#include <pthread.h>

int total_ticket = 10;
//pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_mutex;

/*模拟售票*/
void* ThreadFunction(void* arg)
{
	//printf("current thread id: 0x%u\n", pthread_self());
	int ilock = 0;
	for (int i = 0; i < 10; ++i)
	{
		/*add lock*/
		ilock = pthread_mutex_lock(&m_mutex);
		if (0 == ilock)
		{
			if (total_ticket > 0)
			{
				printf("0x%u sell %d ticket\n", pthread_self(), 10 - total_ticket + 1);
				total_ticket--;
			}
			pthread_mutex_unlock(&m_mutex);
		}
		sleep(1);
	}
	
	return NULL;
}

int main()
{
	int res = 0;
	void* result = NULL;
	pthread_t thread_id[4];
	
	res = pthread_mutex_init(&m_mutex, NULL);
	if (0 != res)
	{
		printf("mutex init failed\n");
		return -1;
	}
	
	for (int i = 0; i < 4; ++i)
	{
		res = pthread_create(&thread_id[i], NULL, &ThreadFunction, NULL);
		if (0 != res)
		{
			printf("create thread failed\n");
			return -1;
		}
	}
	
	//sleep(10);
	
	for (int i = 0; i < 4; ++i)
	{
		res = pthread_join(thread_id[i], &result);
		if (0 != res)
		{
			printf("thread_id = %d wait failed\n", thread_id[i]);
		}
	}
	
	res = pthread_mutex_destroy(&m_mutex);
	if (0 != res)
	{
		printf("destroy mutex failed\n");
		return -1;
	}
	
	return 0;
}