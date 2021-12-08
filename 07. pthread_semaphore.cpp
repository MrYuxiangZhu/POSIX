#include <iostream>
#include <unistd.h>
#include <errno.h> //使用宏ESRCH
#include <pthread.h>
#include <semaphore.h>

int total_ticket = 10;
sem_t m_semaphore;

/*模拟售票*/
void* ThreadFunction(void* arg)
{
	//printf("current thread id: 0x%u\n", pthread_self());
	for (int i = 0; i < 10; ++i)
	{
		/*add lock*/
		int ilock = sem_wait(&m_semaphore);
		if (0 == ilock)
		{
			if (total_ticket > 0)
			{
				printf("0x%u sell %d ticket\n", pthread_self(), 10 - total_ticket + 1);
				total_ticket--;
			}
			sem_post(&m_semaphore);/*add one*/
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
	
	res = sem_init(&m_semaphore, 0, 1);
	if (0 != res)
	{
		printf("init semaphore falied\n");
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
	
	res = sem_destroy(&m_semaphore);
	if (0 != res)
	{
		printf("destroy semaphore failed\n");
		return -1;
	}
	
	return 0;
}