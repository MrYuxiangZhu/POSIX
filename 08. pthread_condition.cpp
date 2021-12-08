#include <iostream>
#include <unistd.h>
#include <errno.h> //使用宏ESRCH
#include <pthread.h>
#include <semaphore.h>

int total_ticket = 0;
pthread_mutex_t m_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t m_cond = PTHREAD_COND_INITIALIZER;

void* WaitForSellFinished(void* args)
{
	int res = pthread_mutex_lock(&m_mutex);
	if (0 != res)
	{
		printf("wait for lock failed\n");
		return NULL;
	}
	
	printf("wait for tickets is 10\n");
	
	if (pthread_cond_wait(&m_cond, &m_mutex) == 0)
	{
		printf("current ticket is %d\n", total_ticket);
	}
	
	pthread_mutex_unlock(&m_mutex);
	
	return NULL;
}

/*模拟售票*/
void* ThreadFunction(void* arg)
{
	//printf("current thread id: 0x%u\n", pthread_self());
	for (int i = 0; i < 10; ++i)
	{
		/*add lock*/
		int ilock = pthread_mutex_lock(&m_mutex);
		if (0 == ilock)
		{
			if (total_ticket < 10)
			{
				total_ticket++;
				printf("0x%u sell %d ticket\n", pthread_self(), total_ticket);
			}
			pthread_mutex_unlock(&m_mutex);
		}
		sleep(1);
	}
	
	/*send signal*/
	int res = pthread_cond_signal(&m_cond);
	if (0 != res)
	{
		printf("send signal failed\n");
	}
	
	return NULL;
}

int main()
{
	int res = 0;
	void* result = NULL;
	pthread_t thread_id;
	pthread_t thread_cond;
	
	res = pthread_create(&thread_id, NULL, &ThreadFunction, NULL);
	if (0 != res)
	{
		printf("create thread failed\n");
		return -1;
	}
	
	res = pthread_create(&thread_cond, NULL, &WaitForSellFinished, NULL);
	if (0 != res)
	{
		printf("create thread failed\n");
		return -1;
	}
	
	//sleep(10);
	
	res = pthread_join(thread_id, &result);
	if (0 != res)
	{
		printf("thread_id = %d wait failed\n", thread_id);
	}
	
	res = pthread_join(thread_cond, &result);
	if (0 != res)
	{
		printf("thread_id = %d wait failed\n", thread_cond);
	}
	
	pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
	
	return 0;
}