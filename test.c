#include "philo.h"
void* thread1(void* ptr)
{
    int *i = (int*)ptr;
    while((*i)--)
        printf("1");
    return (NULL);
}
void* thread2()
{
    int i = 0;
    while (i++ < 10)
        printf("2");
    return (NULL);
}
int main()
{
    pthread_t   thread;
    int i = 100;

    pthread_create(&thread, NULL, thread1, (void*) &i);
    thread2();
    pthread_join(thread, NULL);
    return (0);
}
