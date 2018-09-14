#include "condition.h"

Condition::Condition(const pthread_mutexattr_t *mutexattr,
                        const pthread_condattr_t *condattr)
{
    pthread_mutex_init(&mutex,mutexattr);//初始化互池量
    pthread_cond_init(&cond,condattr);//初始化条件变量
}

Condition::~Condition()
{
    pthread_mutex_destroy(&mutex);//销毁互池量
    pthread_cond_destroy(&cond);//销毁条件变量
}

int Condition::signal()
{
    return pthread_cond_signal(&cond);
}

int Condition::broadcast()
{
    return pthread_cond_broadcast(&cond);
}

int Condition::wait()
{
    return pthread_cond_wait(&cond,&mutex);
}

int Condition::timedwait(int seconds)
{
    struct timespec abstime;
    clock_gettime(CLOCK_REALTIME,&abstime);//获取当前时间

    abstime.tv_sec+=seconds;//将当前时间加上需要等待的秒数，构成绝对时间
    
    return pthread_cond_timedwait(&cond,&mutex,&abstime);
}

int Condition::lock()
{
    return pthread_mutex_lock(&mutex);
}

int Condition::trylock()
{
    return pthread_mutex_trylock(&mutex);
}

int Condition::unlock()
{
    return pthread_mutex_unlock(&mutex);
}

