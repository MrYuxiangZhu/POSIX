#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <iostream>

using namespace std;

void* ThreadFunction(void *arg)
{
    if (NULL == arg)
    {
        std::cout << "arg is NULL\n";
    }
    else
    {   
        std::cout << (char*)arg;
    }

    return NULL;
}

int main()
{
    int res = 0;
    const char* str1 = "run Thread 01";
    const char* str2 = "run Thread 02";
    pthread_t thread1, thread2;
    res = pthread_create(&thread1, NULL, ThreadFunction, (void*)str1);
    if (res != 0)
    {
        std::cout << "create thread1 failed\n";
    }
    
    sleep(1);

    res = pthread_create(&thread2, NULL, &ThreadFunction, (void*)str2);
    if (res != 0)
    {
        std::cout << "create thread2 failed\n";
    }

    sleep(1);
	
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
	
    return 0;
}