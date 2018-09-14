#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "condition.h"
#include "header.h"

void *thread_routine(void *arg);

class Threadpool
{
    friend void *thread_routine(void *arg);

    private:
        typedef void *(*callback_t)(void *);//回调的函数类型
        struct task_t                       //任务结构
        {
            callback_t run;//任务回调函数
            void *arg;//任务函数参数
        };

        void starttask();

        Condition ready;//任务准备就绪或线程池销毁通知
        queue<task_t *> taskqueue;//任务队列

        unsigned int maxthreads;//线程池最多允许的线程数
        unsigned int counter;//线程池当前线程数
        unsigned int idle;//线程池空闲线程数
        unsigned int waitseconds;//线程可以等待的秒数
        bool quit;//线程销毁标志

    public:
        Threadpool(int _maxthreads=36,unsigned int _waitseconds=2);
        ~Threadpool();
        void addtask(callback_t run,void *arg);
};
#endif
