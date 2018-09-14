#ifndef CONDITION_H
#define CONDITION_H

#include <pthread.h>
#include <time.h>
class Condition
{
    public:
        Condition(const pthread_mutexattr_t *mutexattr=NULL,
                    const pthread_condattr_t *condattr=NULL);
        ~Condition();
        
        //条件变量函数
        int signal();
        int broadcast();
        int wait();
        int timedwait(int seconds);

        //互chi量函数
        int lock();
        int trylock();
        int unlock();

    private:
        pthread_mutex_t mutex;
        pthread_cond_t cond;
};

#endif
