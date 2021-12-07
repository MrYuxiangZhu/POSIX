#include <iostream>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void* ThreadFunction(void* arg)
{
    const char* result = "thread exit";
    pthread_exit((void*)result);
}

int main()
{
    int res = 0;
    void* thread_result = NULL;
    pthread_t thread_id;

    /*创建线程*/
    res = pthread_create(&thread_id, NULL, ThreadFunction, NULL);
    if (res != 0)
    {
        std::cout << "Create thread failed\n";
    }

    /*等待线程执行完成*/
    res = pthread_join(thread_id, &thread_result);
    if (res != 0)
    {
        std::cout << "wait thread faild\n";
    }
    std::cout << (char*)thread_result << std::endl;

    return 0;
}