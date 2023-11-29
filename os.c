#include "os.h"

int OS_CreateThread(pthread_t *thread, const pthread_attr_t *attr,
                    t_osThreadhandler func,
                    void *arg)
{
    int res = pthread_create(thread, attr, func, arg);

    return (res);
}


void    OS_ShowThreadInfo(void)
{
    cpu_set_t   cpuSet;
    pthread_t   tid;
    int         res;

    tid = pthread_self();
    res = pthread_getaffinity_np(tid, sizeof(cpuSet), &cpuSet);
    if (res != 0) PERROR("pthread_getaffinity_np", -3);

    writ


}